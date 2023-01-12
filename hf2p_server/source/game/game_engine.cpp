#include "game_engine.h"
#include "..\simulation\simulation.h"
#include "..\simulation\game_interface\simulation_game_action.h"
#include "..\memory\tls.h"
#include "game_engine_util.h"
#include "game_engine_event_definitions.h"
#include "game.h"
#include "game_engine_team.h"
#include "game_results.h"

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

void game_engine_player_added(datum_index player_index)
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
		c_game_engine* game_engine = current_game_engine();
		game_engine->player_added(player_index);
		c_game_statborg* statborg = game_engine_get_statborg();
		statborg->player_added(player_index);

		if (game_ticks_to_seconds(game_time_get()) > 1.0)
		{
			s_game_engine_event_data event_data;
			game_engine_initialize_event(_game_engine_event_type_general, 0x40047, &event_data); // TODO: string_id for general_event_player_joined
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
			update_flags.set(_simulation_entity_type_game_engine_player, true);
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
		//game_results_statistic_set(player_index, _game_team_none, 0, 1); // TODO REWRITE THIS
	}
}