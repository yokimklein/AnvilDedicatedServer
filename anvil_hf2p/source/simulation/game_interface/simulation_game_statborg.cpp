#include "simulation_game_statborg.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <stdio.h>
#include <cseries\cseries_events.h>

void simulation_action_game_statborg_update(c_simulation_object_update_flags& update_flags)
{
	if (!game_is_server() || !game_is_distributed() || game_is_playback())
	{
		return;
	}

	datum_index gamestate_index = game_engine_globals_get_statborg_gamestate_index();
	if (gamestate_index == NONE)
	{
		if (game_is_available())
		{
			event(_event_error, "networking:simulation:action: statborg does not have valid gamestate index to update");
		}
		return;
	}

	long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
	if (entity_index == NONE)
	{
		event(_event_warning, "networking:simulation:action: statborg has invalid entity, can't update (gamestate 0x%08X)", gamestate_index);
		return;
	}
	
	simulation_entity_update(entity_index, NONE, update_flags);
}

// $TODO: ASSERTION IS CRASHING HERE
void simulation_action_game_statborg_update(e_simulation_statborg_update_flag update_flag)
{
	//ASSERT(update_flag >= 0 && update_flag < k_simulation_statborg_update_flag_count);
	c_simulation_object_update_flags update_flags(update_flag);
	simulation_action_game_statborg_update(update_flags);
}