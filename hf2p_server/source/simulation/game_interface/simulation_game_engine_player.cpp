#include "simulation_game_engine_player.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_world.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <networking\replication\replication_entity_manager.h>

void simulation_action_game_engine_player_create(short player_absolute_index)
{
	if (game_is_server() && game_is_distributed())
	{
		assert(game_engine_globals_get_player_gamestate_index(player_absolute_index) == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		game_engine_globals_set_player_gamestate_index(player_absolute_index, gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, NONE);
		if (!game_is_playback())
		{
			long entity_index = simulation_entity_create(_simulation_entity_type_game_engine_player, NONE, gamestate_index);
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

void simulation_action_game_engine_player_delete(short player_absolute_index)
{
	if (game_is_server() && game_is_distributed())
	{
		datum_index gamestate_index = game_engine_globals_get_player_gamestate_index(player_absolute_index);
		if (gamestate_index != NONE)
		{
			if (!game_is_playback())
			{
				long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
				if (entity_index != NONE)
					simulation_entity_delete(entity_index, NONE, gamestate_index);
				else
					printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_player_delete: global player %d gamestate index 0x%8X not attached to entity\n", player_absolute_index, gamestate_index);
			}
			simulation_gamestate_entity_delete(gamestate_index);
		}
		else
		{
			printf("MP/NET/SIMULATION,ACTION: simulation_action_game_map_variant_delete: global player %d has no gamestate representation\n", player_absolute_index);
		}
		game_engine_globals_set_player_gamestate_index(player_absolute_index, NONE);
	}
}

void simulation_action_game_engine_player_update(short player_index, c_flags<long, ulong64, 64>* update_flags)
{
	if (game_is_server() && game_is_distributed() && !game_is_playback())
	{
		datum_index gamestate_index = game_engine_globals_get_player_gamestate_index(player_index);
		if (gamestate_index == NONE)
		{
			if (game_is_available())
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_player_update: failed to update player %d not attached to gamestate\n", player_index);
		}
		else
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == NONE)
				printf("MP/NET/SIMULATION,ACTION: simulation_action_game_engine_player_update: failed to update player %d gamestate 0x%8X not attached to entity\n", player_index, gamestate_index);
			else
				simulation_entity_update(entity_index, NONE, update_flags);
		}
	}
}

void simulation_action_game_engine_player_update(datum_index player_index, long update_flag)
{
	assert(update_flag >= 0 && update_flag < k_simulation_player_update_flag_count);
	assert((word)player_index >= 0 && (word)player_index <= 16);
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set(update_flag, true);
	simulation_action_game_engine_player_update((word)player_index, &update_flags);
}