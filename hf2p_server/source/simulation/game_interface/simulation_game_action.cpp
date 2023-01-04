#include "simulation_game_action.h"
#include "..\..\game\game.h"
#include "..\simulation_gamestate_entities.h"
#include "simulation_game_entities.h"
#include "..\simulation_world.h"
#include "assert.h"

void simulation_action_game_engine_globals_create()
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_gamestate_index() == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_gamestate_index(gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, -1);
		if (!game_is_playback())
		{
			e_simulation_entity_type entity_type = simulation_entity_type_from_game_engine();
			if (entity_type != -1)
			{
				long entity_index = simulation_entity_create(entity_type, -1, gamestate_index);
				if (entity_index != -1)
				{
					c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
					s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
					assert(entity->gamestate_index != NONE);
					simulation_entity_database->entity_capture_creation_data(entity_index);
				}
			}
		}
	}
}

void simulation_action_game_statborg_create()
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_statborg_gamestate_index() == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_statborg_gamestate_index(gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, -1);
		if (!game_is_playback())
		{
			long entity_index = simulation_entity_create(_simulation_entity_type_game_engine_statborg, -1, gamestate_index);
			if (entity_index != -1)
			{
				c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
				s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
				assert(entity->gamestate_index != NONE);
				simulation_entity_database->entity_capture_creation_data(entity_index);
			}
		}
	}
}

void simulation_action_game_ai_create()
{
	// this function is empty in every build i've seen
	// is this for syncing ai over distributed/asynchronous games?
}

void simulation_action_game_map_variant_create_all()
{
	// TODO
}

void simulation_action_game_engine_player_create(long player_index)
{
	// TODO
}

void simulation_action_breakable_surfaces_create()
{
	// TODO
}