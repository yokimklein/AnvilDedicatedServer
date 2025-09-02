#include "network_session_parameters_session.h"
#include <simulation\simulation.h>
#include <iostream>
#include <game\game.h>
#include <anvil\backend\cache.h>
#include <anvil\config.h>

long c_network_session_parameter_session_size::get_max_player_count() const
{
	long max_player_count = k_network_maximum_players_per_session;

	if (game_is_dedicated_server())
	{
		std::string playlist_id = g_anvil_configuration["playlist_id"];
		if (g_backend_data_cache.m_playlists.contains(playlist_id))
		{
			s_cached_playlist& playlist = g_backend_data_cache.m_playlists[playlist_id];
			max_player_count = MIN(playlist.maximum_players, k_network_maximum_players_per_session);
		}
	}

	if (get_allowed())
	{
		max_player_count = m_data.maximum_player_count;
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_session_size::get_max_player_count: [%s] failed to get max player count, unavailable\n",
			get_session_description());
	}
	return max_player_count;
}

bool c_network_session_parameter_session_size::set_max_player_count(long player_count)
{
	return DECLFUNC(0x2D670, bool, __thiscall, c_network_session_parameter_session_size*, long)(this, player_count);
}

long c_network_session_parameter_session_size::get_max_peer_count() const
{
	long max_peer_count = k_network_maximum_machines_per_session;
	if (get_allowed())
	{
		max_peer_count = m_data.maximum_peer_count;
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_session_size::get_max_peer_count: [%s] failed to get max peer count, unavailable\n",
			get_session_description());
	}
	return max_peer_count;
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	return DECLFUNC(0x2D820, bool, __thiscall, c_network_session_parameter_session_mode*, e_network_session_mode)(this, session_mode);
}

bool c_network_session_parameter_lobby_vote_set::set(s_network_session_parameter_lobby_vote_set* vote_set)
{
	if (csmemcmp(&m_data.vote_options, &vote_set->vote_options, sizeof(s_network_session_parameter_lobby_vote_set::vote_options)) != 0
		|| m_data.winning_vote_index != vote_set->winning_vote_index
		|| !get_allowed())
	{
		csmemcpy(m_data.vote_options, vote_set->vote_options, sizeof(m_data.vote_options));
		m_data.winning_vote_index = vote_set->winning_vote_index;
		set_update_required();
	}
	return true;
}

void c_network_session_parameter_lobby_vote_set::get(s_network_session_parameter_lobby_vote_set* output)
{
	csmemcpy(output->vote_options, m_data.vote_options, sizeof(m_data.vote_options));
	output->winning_vote_index = m_data.winning_vote_index;
}