#include "simulation_game_action.h"
#include "..\..\game\game.h"
#include "..\simulation_gamestate_entities.h"
#include "simulation_game_entities.h"
#include "..\simulation_world.h"
#include "assert.h"
#include <stdio.h>

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
		if (entity_index != -1)
		{
			c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
			s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
			assert(entity->gamestate_index != NONE);
			simulation_entity_database->entity_capture_creation_data(entity_index);
		}
	}
}

void simulation_action_game_engine_player_create(short player_absolute_index)
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_player_gamestate_index(player_absolute_index) == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_player_gamestate_index(player_absolute_index, gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, -1);
		if (!game_is_playback())
		{
			long entity_index = simulation_entity_create(_simulation_entity_type_game_engine_player, -1, gamestate_index);
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

void simulation_action_breakable_surfaces_create()
{
	// there is code for this in ms23, but it doesn't seem like it would function?
	// TODO: revisit this
}

void simulation_action_game_statborg_update(c_flags<long, ulong64, 64>* update_mask)
{
	if (game_is_server() && game_is_distributed() && !game_is_playback())
	{
		datum_index gamestate_index = game_engine_globals_get_statborg_gamestate_index();
		if (gamestate_index == -1)
		{
			if (game_is_available())
				printf("simulation_action_game_statborg_update: statborg does not have valid gamestate index to update\n");
		}
		else
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == -1)
				printf("simulation_action_game_statborg_update: statborg has invalid entity, can't update (gamestate 0x%8X)\n", gamestate_index);
			else
				simulation_entity_update(entity_index, -1, update_mask);
		}
	}
}

void simulation_action_game_engine_player_update(short absolute_player_index, ulong update_mask)
{

}