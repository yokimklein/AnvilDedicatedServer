#include "life_cycle_handler_end_game_write_stats.h"
#include <stdio.h>
#include <networking\session\network_session.h>
#include <game\game.h>
#include <game\game_results.h>
#include <data_mining\data_mining.h>
#include <networking\network_time.h>
#include <networking\network_configuration.h>
#include <networking\logic\network_session_interface.h>
#include <anvil\backend\services\private_service.h>
#include <anvil\backend\cache.h>

char const* k_stats_write_desire_strings[]
{
	"no-reason",
	"not-live",
	"game-options-not-valid",
	"game-is-campaign",
	"game-is-film",
	"results-not-finalized",
	"can't-get-final-stats",
	"stats-uninitialized",
	"stats-unreliable",
	"simulation-aborted",
	"not-enough-machines",
	"write-desired"
};
static_assert(NUMBEROF(k_stats_write_desire_strings) == k_end_game_stats_write_desire_count);

void c_life_cycle_state_handler_end_game_write_stats::update_()
{
	c_life_cycle_state_manager* manager = get_manager();
	c_network_session* session = manager->get_active_squad_session();
	if (!m_flags.test(_end_game_write_stats_initiated_bit))
	{
		bool not_writing_stats = true;
		if (m_flags.test(_end_game_write_stats_bypassed_bit) || !game_is_dedicated_server()) // No need to write stats outside of dedi
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": not writing stats, bypassing\n");
			not_writing_stats = true;
		}
		else
		{
			e_end_game_stats_write_desire stats_write_desire = game_get_stats_write_desire();
			if (stats_write_desire != _end_game_stats_write_desire_write_desired)
			{
				printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": should not write stats for this game, not writing stats [reason %s]\n",
					k_stats_write_desire_strings[stats_write_desire]);
				not_writing_stats = true;
			}
			else
			{
				not_writing_stats = false;
			}
		}
		if (not_writing_stats)
		{
			m_flags.set(_end_game_write_stats_suppressed_bit, true);
		}
		else
		{
			m_flags.set(_end_game_write_stats_suppressed_bit, false);
		}
		m_flags.set(_end_game_write_stats_initiated_bit, true);
	}

	if (!m_flags.test(_end_game_write_stats_completed_bit))
	{
		if (!m_flags.test(_end_game_write_stats_session_start_completed_bit))
		{
			update_session_start();
		}
		else
		{
			if (!m_flags.test(_end_game_write_stats_write_completed_bit) && !m_flags.test(_end_game_write_stats_suppressed_bit))
			{
				update_write_stats();
			}
			else
			{
				if (!m_flags.test(_end_game_write_stats_session_end_completed_bit))
				{
					update_session_end();
				}
				else
				{
					if (!m_flags.test(_end_game_write_stats_webstats_submitted_bit) && session->is_host())
					{
						update_submit_webstats();
					}
					else if (!m_flags.test(_end_game_write_stats_data_mine_upload_completed_bit))
					{
						update_data_mine_upload();
					}
					else
					{
						network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_game_stats_written_bit, true);
						m_flags.set(_end_game_write_stats_completed_bit, true);
					}
				}
			}
		}
	}

	if (session->is_host())
	{
		if (all_peers_have_main_menu_ready(session))
		{
			if (m_flags.test(_end_game_write_stats_completed_bit))
			{
				const c_network_session_membership* membership = session->get_session_membership();
				if (membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_game_stats_written_bit))
				{
					c_network_session_parameters* parameters = session->get_session_parameters();
					parameters->m_parameters.session_mode.set(_network_session_mode_idle);
				}
			}
		}
	}

	if (!data_mine_enabled())
	{
		if (network_time_since(m_end_game_write_stats_start_time) > 1000 * g_network_configuration.life_cycle.matchmaking.end_match_write_stats_boot_threshold_seconds)
		{
			if (!m_flags.test(_end_game_write_stats_completed_bit))
			{
				printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": we exceeded the grief timer, and had not written stats, force completing\n");
				network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_game_stats_written_bit, true);
				m_flags.set(_end_game_write_stats_completed_bit, true);
			}
			if (session->is_host() && all_peers_have_main_menu_ready(session))
			{
				const c_network_session_membership* membership = session->get_session_membership();

				// if the host hasn't written stats, bail out of session
				if (!membership->get_peer(membership->local_peer_index())->properties.flags.test(_network_session_peer_properties_status_match_stats_written_bit))
				{
					printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": host is taking too long to write stats, bailing!\n");
					session->host_boot_machine(membership->local_peer_index(), _network_session_boot_blocking_stats_write);
					session->get_session_parameters()->m_parameters.session_mode.set(_network_session_mode_idle);
				}
				// otherwise find and boot any peer which hasn't yet written stats
				else
				{
					c_static_flags<k_maximum_machines> peers_without_stats_mask{};
					for (long peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
					{
						if (!membership->get_peer(peer_index)->properties.flags.test(_network_session_peer_properties_status_match_stats_written_bit))
						{
							peers_without_stats_mask.set(peer_index, true);
						}
					}
					if (!peers_without_stats_mask.is_clear())
					{
						for (long peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
						{
							if (peers_without_stats_mask.test(peer_index))
							{
								printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": peer %d is taking too long to write stats, kicking!\n", peer_index);
								session->host_boot_machine(peer_index, _network_session_boot_blocking_stats_write);
							}
						}
					}
				}
			}
		}
	}
}

// $TODO: This likely won't track your stats if you quit the game early which would be very annoying if you disconnect
void c_life_cycle_state_handler_end_game_write_stats::update_write_stats()
{
	c_network_session* session = get_manager()->get_active_squad_session();
	const c_network_session_membership* membership = session->get_session_membership();
	if (m_flags.test(_end_game_write_stats_write_initiated_bit))
	{
		return;
	}

	c_game_results* final_results = game_results_get_final_results();
	if (!final_results)
	{
		return;
	}
	m_flags.set(_end_game_write_stats_write_initiated_bit, true);


	s_transport_secure_identifier lobby_identifier = {};
	s_transport_secure_address server_identifier = {};
	if (!transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_windows, &lobby_identifier, &server_identifier))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to retrieve LobbyID and DedicatedServerID! stats will not be submitted!\n");
		m_flags.set(_end_game_write_stats_write_completed_bit, true);
		return;
	}

	c_backend::private_service::submit_game_statistics::s_request request;
	request.LobbyID = transport_secure_identifier_get_string(&lobby_identifier);
	request.DedicatedServerID = transport_secure_address_get_string(&server_identifier);
	for (long player_index = membership->get_first_player(); player_index != NONE; player_index = membership->get_next_player(player_index))
	{
		c_backend::private_service::submit_game_statistics::s_request::s_player request_player;
		const s_network_session_player* player = membership->get_player(player_index);
	
		// UserID
		request_player.UserID = player->configuration.host.user_xuid;
		// Kills
		request_player.Kills = final_results->statistics.player[player_index].statistics[_game_results_statistic_kills].statistic;
		// Deaths
		request_player.Deaths = final_results->statistics.player[player_index].statistics[_game_results_statistic_deaths].statistic;
		// Assists
		request_player.Assists = final_results->statistics.player[player_index].statistics[_game_results_statistic_assists].statistic;
		// Suicides
		request_player.Suicides = final_results->statistics.player[player_index].statistics[_game_results_statistic_suicides].statistic;
		// GamesWon
		request_player.GamesWon = final_results->statistics.player[player_index].statistics[_game_results_statistic_games_won].statistic;
		// Total WP
		request_player.TotalWP = final_results->statistics.player[player_index].statistics[_game_results_statistic_total_wp].statistic;
	
		// Time played (in seconds)
		dword time_played = final_results->game_description.finish_time - final_results->game_description.start_time;
		// If the player joined after the game started
		if (final_results->game_description.start_time < player->join_time) // $TODO: I don't think single player players are having their join time set
		{
			// Remove time in which the player was not in the game from their total playtime
			time_played -= (player->join_time - final_results->game_description.start_time);
		}
		request_player.TimePlayed = time_played;
	
		// WP Medals
		for (ulong event_index = 0; event_index < g_backend_data_cache.m_earned_wp_events[player_index].size(); event_index++)
		{
			long value = g_backend_data_cache.m_earned_wp_events[player_index][event_index];
			if (value <= 0)
			{
				continue;
			}
	
			c_backend::private_service::submit_game_statistics::s_request::s_medal medal;
			medal.ID = event_index;
			medal.Total = value;
			request_player.Medals.push_back(medal);
		}
	
		request.Players.push_back(request_player);
	}
	
	c_backend::private_service::submit_game_statistics::request(request);
	m_flags.set(_end_game_write_stats_write_completed_bit, true);
}

e_end_game_stats_write_desire c_life_cycle_state_handler_end_game_write_stats::game_get_stats_write_desire() const
{
	c_life_cycle_state_manager* manager = get_manager();
	c_network_session* session = manager->get_active_squad_session();

	// Removed this as in Anvil we want to write stats regardless of legacy live sessions
	//if (session->session_class() != _network_session_class_xbox_live)
	//{
	//	return _end_game_stats_write_desire_not_live;
	//}

	if (!game_options_valid())
	{
		return _end_game_stats_write_desire_game_options_not_valid;
	}

	if (game_is_campaign())
	{
		return _end_game_stats_write_desire_game_is_campaign;
	}

	if (game_is_playback())
	{
		return _end_game_stats_write_desire_game_is_film;
	}

	if (!game_results_get_game_finalized())
	{
		return _end_game_stats_write_desire_results_not_finalized;
	}

	c_game_results* final_results = game_results_get_final_results();
	
	if (!final_results)
	{
		return _end_game_stats_write_desire_cant_get_final_stats;
	}

	if (!final_results->finish_reason)
	{
		return _end_game_stats_write_desire_stats_uninitialized;
	}

	if (final_results->finalized)
	{
		return _end_game_stats_write_desire_stats_unreliable;
	}

	if (final_results->game_description.simulation_aborted)
	{
		return _end_game_stats_write_desire_simulation_aborted;
	}
	
	// Removed this as in Anvil we want to submit game stats regardless of whether everyone in the session has disconnected
	//long machine_count = 0;
	//for (long machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//{
	//	s_game_results_machine_data& machine = final_results->machines[machine_index];
	//	if (machine.machine_exists && machine.machine_connected && !machine.machine_voluntary_quit)
	//	{
	//		machine_count++;
	//	}
	//}
	//if (machine_count <= 1)
	//{
	//	return _end_game_stats_write_desire_not_enough_machines;
	//}

	return _end_game_stats_write_desire_write_desired;
}

void c_life_cycle_state_handler_end_game_write_stats::update_session_start()
{
	INVOKE_CLASS_MEMBER(0x4CB50, c_life_cycle_state_handler_end_game_write_stats, update_session_start);
}

void c_life_cycle_state_handler_end_game_write_stats::update_session_end()
{
	INVOKE_CLASS_MEMBER(0x4CBC0, c_life_cycle_state_handler_end_game_write_stats, update_session_end);
}

void c_life_cycle_state_handler_end_game_write_stats::update_submit_webstats()
{
	c_game_results* final_results = game_results_get_final_results();
	ASSERT(!m_flags.test(_end_game_write_stats_webstats_submitted_bit));

	if (final_results)
	{
		if (!final_results->initialized)
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": can't submit webstats, final game results not finalized?\n");
		}
		else
		{
			//game_lsp_submit_multiplayer_result(final_results);
		}
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": can't submit webstats, can't get final game results\n");
	}

	m_flags.set(_end_game_write_stats_webstats_submitted_bit, true);
}

void c_life_cycle_state_handler_end_game_write_stats::update_data_mine_upload()
{
	if (data_mine_enabled())
	{
		// $TODO: we don't use the datamine anyway, so can just leave this as is
		// second test range added as a result of adding back stats write flags
		// I'm also not sure if this if check is correct, but it should never be used anyway
		if (m_flags.test_range(_end_game_write_stats_session_start_initiated_bit, _end_game_write_stats_data_mine_upload_initiated_bit) ||
			m_flags.test_range(_end_game_write_stats_write_initiated_bit, _end_game_write_stats_write_completed_bit))
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": initiated data mine upload\n");
			//m_manager->get_observer()->observer_prioritize_upload_bandwidth(true);
			//data_mine_upload_no_dialog();
			m_flags.set(_end_game_write_stats_data_mine_upload_initiated_bit, true);
		}
		if (m_flags.test(_end_game_write_stats_data_mine_upload_initiated_bit) &&
			!m_flags.test(_end_game_write_stats_data_mine_upload_completed_bit)/* &&
			!data_mine_uploading_files()*/)
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": data mine upload complete\n");
			m_flags.set(_end_game_write_stats_data_mine_upload_completed_bit, true);
		}
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": data mining not enabled, marking upload completed\n");
		m_flags.set(_end_game_write_stats_data_mine_upload_initiated_bit, true);
		m_flags.set(_end_game_write_stats_data_mine_upload_completed_bit, true);
	}
}
