#include "simulation_game_engine_globals.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_world.h>

void simulation_action_game_engine_globals_update(c_flags<long, ulong64, 64>* update_flags)
{
	if (game_is_server() && game_is_distributed() && !game_is_playback())
	{
		datum_index gamestate_index = game_engine_globals_get_gamestate_index();
		if (gamestate_index == NONE)
		{
			if (game_is_available())
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_globals_update: game engine globals does not have gamestate to update?\n");
		}
		else
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == NONE)
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_globals_update: game engine globals has invalid entity index (gamestate 0x%8X) can't update\n", gamestate_index);
			else
				simulation_entity_update(entity_index, NONE, update_flags);
		}
	}
}

void simulation_action_game_engine_globals_update(long update_flag)
{
	assert(update_flag >= 0 && update_flag < 30); // roughly accurate, vip has the most flags w/ 30 total
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set(update_flag, true);
	simulation_action_game_engine_globals_update(&update_flags);
}