#include "simulation_game_statborg.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <stdio.h>

void simulation_action_game_statborg_update(c_flags<long, ulong64, 64>* update_flags)
{
	if (game_is_server() && game_is_distributed() && !game_is_playback())
	{
		datum_index gamestate_index = game_engine_globals_get_statborg_gamestate_index();
		if (gamestate_index == NONE)
		{
			if (game_is_available())
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_statborg_update: statborg does not have valid gamestate index to update\n");
		}
		else
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == NONE)
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_statborg_update: statborg has invalid entity, can't update (gamestate 0x%8X)\n", gamestate_index);
			else
				simulation_entity_update(entity_index, NONE, update_flags);
		}
	}
}

// TODO: ASSERTION IS CRASHING HERE
void simulation_action_game_statborg_update(long update_flag)
{
	//assert(update_flag >= 0 && update_flag < k_simulation_statborg_update_flag_count);
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set(update_flag, true);
	simulation_action_game_statborg_update(&update_flags);
}