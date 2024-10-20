#pragma once
#include <cseries\cseries.h>

enum e_simulation_game_engine_globals_update_flag
{
	_simulation_game_engine_globals_update_initial_teams, // initial teams & team designators
	_simulation_game_engine_globals_update_team_lives_per_round,
	_simulation_game_engine_globals_update_engine_state, // game_engine_globals->current_state
	_simulation_game_engine_globals_update_game_finished,
	_simulation_game_engine_globals_update_round_index,
	_simulation_game_engine_globals_update_round_timer,
	_simulation_game_engine_globals_update_round_condition,

	k_simulation_game_engine_globals_flag_count
};

void simulation_action_game_engine_globals_update(c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_engine_globals_update(long update_flag);