#include "simulation_game_action.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_entity_database.h>
#include <simulation\simulation_world.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_units.h>


void simulation_action_game_engine_globals_create()
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_gamestate_index() == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_gamestate_index(gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, NONE);
		if (!game_is_playback())
		{
			e_simulation_entity_type entity_type = simulation_entity_type_from_game_engine();
			if (entity_type != NONE)
			{
				long entity_index = simulation_entity_create(entity_type, NONE, gamestate_index);
				if (entity_index != NONE)
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

void simulation_action_game_engine_globals_delete()
{
	if (game_is_server() && game_is_distributed())
	{
		datum_index gamestate_index = game_engine_globals_get_gamestate_index();
		if (gamestate_index != NONE)
		{
			if (!game_is_playback())
			{
				long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
				if (entity_index != NONE)
					simulation_entity_delete(entity_index, NONE, gamestate_index);
				else
					printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_globals_delete: game engine globals [gamestate index 0x%8X] has no entity to delete\n", gamestate_index);
			}
		}
		else
		{
			printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_globals_delete: game engine globals has invalid gamestate index, can't delete\n");
		}
		game_engine_globals_set_gamestate_index(NONE);
	}
}

void simulation_action_game_statborg_create()
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_statborg_gamestate_index() == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_statborg_gamestate_index(gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, NONE);
		if (!game_is_playback())
		{
			long entity_index = simulation_entity_create(_simulation_entity_type_game_engine_statborg, NONE, gamestate_index);
			if (entity_index != NONE)
			{
				c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
				s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
				assert(entity->gamestate_index != NONE);
				simulation_entity_database->entity_capture_creation_data(entity_index);
			}
		}
	}
}

void simulation_action_game_statborg_delete()
{
	if (game_is_server() && game_is_distributed())
	{
		datum_index gamestate_index = game_engine_globals_get_statborg_gamestate_index();
		if (gamestate_index != NONE)
		{
			if (!game_is_playback())
			{
				long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
				if (entity_index != NONE)
					simulation_entity_delete(entity_index, NONE, gamestate_index);
				else
					printf("MP/NET/SIMULATION,ACTION: simulation_action_game_statborg_delete: statborg gamestate index 0x%8X not attached to entity\n", gamestate_index);
			}
			simulation_gamestate_entity_delete(gamestate_index);
		}
		else
		{
			printf("MP/NET/SIMULATION,ACTION: simulation_action_game_statborg_delete: statborg has invalid gamestate index, cannot delete\n");
		}
		game_engine_globals_set_statborg_gamestate_index(NONE);
	}
}

void simulation_action_game_ai_create()
{
	// this function is empty in every build i've seen
	// is this for syncing ai over distributed/asynchronous games?
	// seems like it was planned at one point but unfinished & stubbed
}

void simulation_action_game_ai_delete()
{
	// ditto above
}

// only called for the sandbox engine (forge)
// TODO: test this
void simulation_action_game_map_variant_create_all()
{
	if (game_is_server() && game_is_distributed())
	{
		for (long i = 0; i < k_number_of_map_variant_simulation_entities; i++)
			simulation_action_game_map_variant_create_for_chunk(i);
	}
}

void simulation_action_game_map_variant_create_for_chunk(long chunk_index)
{
	c_map_variant* map_variant = game_engine_get_runtime_map_variant();
	assert(map_variant->get_chunk_gamestate_index(chunk_index) == NONE);
	datum_index gamestate_index = simulation_gamestate_entity_create();
	map_variant->simulation_gamestate_indices[chunk_index] = gamestate_index;
	simulation_gamestate_entity_set_object_index(gamestate_index, chunk_index);
	if (!game_is_playback())
	{
		long entity_index = simulation_entity_create(_simulation_entity_type_map_variant, chunk_index, gamestate_index);
		if (entity_index != NONE)
		{
			c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
			s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
			assert(entity->gamestate_index != NONE);
			simulation_entity_database->entity_capture_creation_data(entity_index);
		}
	}
}

void simulation_action_game_map_variant_delete()
{
	if (game_is_server() && game_is_distributed())
	{
		for (long i = 0; i < k_number_of_map_variant_simulation_entities; i++)
		{
			c_map_variant* map_variant = game_engine_get_runtime_map_variant();
			datum_index gamestate_index = map_variant->simulation_gamestate_indices[i];
			if (gamestate_index != NONE)
			{
				if (!game_is_playback())
				{
					long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
					if (entity_index != NONE)
						simulation_entity_delete(entity_index, i, gamestate_index);
					else
						printf("MP/NET/SIMULATION,ACTION: simulation_action_game_map_variant_delete: map variant [gamestate index 0x%8X] has no entity to delete\n", gamestate_index);
				}
				simulation_gamestate_entity_delete(gamestate_index);
			}
			else
			{
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_map_variant_delete: map variant has invalid gamestate index, can't delete\n");
			}
			map_variant->simulation_gamestate_indices[i] = NONE;
		}
	}
}

void simulation_action_breakable_surfaces_create()
{
	// there is code for this in ms23, but it doesn't seem like it would function?
	// TODO: revisit this
}

//void simulation_action_actor_create(datum_index unit_index)
//{
//	if (game_is_server() && game_is_distributed())
//	{
//		unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
//		assert(unit->object.gamestate_index == NONE);
//		simulation_action_object_create(unit_index);
//		assert(unit->unit.simulation_actor_index == NONE);
//		unit->unit.simulation_actor_index = simulation_get_world()->create_actor(NONE, unit_index); // TODO
//		unit_set_actively_controlled(unit_index, true);
//		simulation_action_object_update(unit_index, _simulation_unit_update_control);
//	}
//}