#include "game_engine.h"
#include <simulation\simulation.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <memory\tls.h>
#include <game\game_engine_util.h>
#include <game\game_engine_event_definitions.h>
#include <game\game.h>
#include <game\game_engine_team.h>
#include <game\game_results.h>
#include <tag_files\string_ids.h>
#include "assert.h"
#include <game\game_time.h>
#include <game\players.h>
#include <game\game_engine_spawning.h>

void game_engine_attach_to_simulation()
{
	simulation_action_game_engine_globals_create();
	simulation_action_game_statborg_create();
	simulation_action_game_ai_create();
	if (game_engine_is_sandbox())
		simulation_action_game_map_variant_create_all();
	for (short i = 0; i < k_maximum_multiplayer_players; i++)
		simulation_action_game_engine_player_create(i);
	simulation_action_breakable_surfaces_create();
}

void __fastcall game_engine_player_added(datum_index player_index)
{
	if (current_game_engine())
	{
		if (game_is_authoritative() && game_engine_has_teams())
		{
			s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
			game_engine_recompute_active_teams();
			game_engine_adjust_team_score_for_composition(player_data->configuration.host.team_index);
		}

		game_engine_assemble_player_traits(player_index);
		current_game_engine()->player_added(player_index);
		game_engine_get_statborg()->player_added(player_index);

		if (game_ticks_to_seconds(game_time_get()) > 1.0)
		{
			s_game_engine_event_data event_data;
			game_engine_initialize_event(_game_engine_event_type_general, STRING_ID(game_engine, general_event_player_joined), &event_data);
			game_engine_send_event(&event_data);
		}

		if (game_is_authoritative())
		{
			s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
			c_game_variant* game_variant = current_game_variant();
			c_game_engine_base_variant* active_variant = game_variant->get_active_variant();
			c_game_engine_respawn_options* respawn_options = active_variant->get_respawn_options();
			byte lives_per_round = respawn_options->get_lives_per_round();
			player_data->lives = lives_per_round;
			c_flags<long, ulong64, 64> update_flags = {};
			update_flags.set(10, true);
			simulation_action_game_engine_player_update((word)player_index, &update_flags);
		}

		s_game_engine_globals* engine_globals = get_tls()->game_engine_globals;
		for (long i = -1; ; engine_globals->fade_to_black_active_user_mask |= 1 << i)
		{
			if (player_index != -1)
				i = player_mapping_get_next_output_user((word)player_index, i);
			if (i == -1)
				break;
			engine_globals->fade_to_black_amount[i] = 1.0;
		}

		game_results_notify_player_indices_changed();
		game_results_statistic_set((word)player_index, _game_team_none, 0, 1);
	}
}

long game_engine_round_time_get()
{
	assert(game_is_authoritative());
	long game_time = game_time_get();
	if (game_time - get_tls()->game_engine_globals->round_timer < 0)
		return 0;
	else
		return game_time - get_tls()->game_engine_globals->round_timer;
}

void game_engine_update_round_conditions()
{
	if (game_is_authoritative())
	{
		s_game_engine_globals* game_engine_globals = get_tls()->game_engine_globals;
		long round_time = game_engine_round_time_get();
		long game_over_timer = game_engine_globals->game_over_timer;
		c_flags<e_game_engine_round_condition, byte, k_game_engine_round_condition_count> condition;
		condition.clear();
		condition.set(_game_engine_round_condition_unknown0, round_time < 5);
		condition.set(_game_engine_round_condition_unknown1, round_time < game_seconds_integer_to_ticks(1));
		condition.set(_game_engine_round_condition_unknown2, round_time < game_seconds_integer_to_ticks(3));
		condition.set(_game_engine_round_condition_unknown3, round_time < game_seconds_integer_to_ticks(4)); // team voice announcer
		condition.set(_game_engine_round_condition_unknown5, round_time < game_seconds_integer_to_ticks(k_pre_game_camera_seconds - 4)); // player control (ORIGINALLY 11)
		condition.set(_game_engine_round_condition_unknown6, round_time < game_seconds_integer_to_ticks(5)); // team intro widget
		condition.set(_game_engine_round_condition_unknown4, round_time <= game_seconds_integer_to_ticks(k_pre_game_camera_seconds)); // camera position (ORIGINALLY 18)
		condition.set(_game_engine_round_condition_unknown7, game_over_timer >= game_seconds_integer_to_ticks(4));
		if (game_engine_globals->round_condition_flags != condition)
		{
			if (game_engine_globals->round_condition_flags.test(_game_engine_round_condition_unknown3) && !condition.test(_game_engine_round_condition_unknown3))
			{
				c_player_in_game_iterator player_iterator(get_tls()->players);
				player_iterator.begin(get_tls()->players);
				while (player_iterator.next())
				{
					long player_index = player_iterator.get_index();
					current_game_engine()->emit_game_start_event(player_index);
				}
			}
			simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_condition);
			game_engine_globals->round_condition_flags = condition;
		}
	}
}

bool game_engine_round_condition_test(e_game_engine_round_condition condition)
{
	assert(VALID_INDEX(condition, k_game_engine_round_condition_count));
	return get_tls()->game_engine_globals->round_condition_flags.test(condition);
}

// I started rewriting this before inserting x86 into the existing function to add back the sim update calls
void game_engine_update_time()
{
	/*
	if (game_engine_in_round() && !game_is_predicted() && !game_engine_round_condition_test(_game_engine_round_condition_unknown4))
	{
		if (game_engine_get_round_time_limit_seconds())
		{
			s_game_engine_globals* game_engine_globals = get_tls()->game_engine_globals;
			long round_time_limit_tps = game_engine_globals->game_variant_round_time_limit_ticks_per_second;
			if (round_time_limit_tps > 0)
				game_engine_globals->game_variant_round_time_limit_ticks_per_second = round_time_limit_tps - 1;
			long time_left_in_ticks = game_engine_get_time_left_in_ticks(true);
			float time_left_in_seconds = game_ticks_to_seconds(time_left_in_ticks);
			// ??

			simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_timer);
		}
	}
	*/
}

bool game_engine_in_round()
{
	if (current_game_engine() && get_tls()->game_engine_globals->current_state == 1 && (game_is_predicted() || get_tls()->game_engine_globals->desired_state == 1))
		return true;
	else
		return false;
}

bool game_engine_player_is_playing(datum_index player_index)
{
	return DECLFUNC(base_address(0xC77A0), bool, __fastcall, datum_index)(player_index);
}

void game_engine_player_set_spawn_timer(datum_index player_index, long timer_ticks)
{
	DECLFUNC(base_address(0xC7700), void, __fastcall, datum_index, long)(player_index, timer_ticks);
}