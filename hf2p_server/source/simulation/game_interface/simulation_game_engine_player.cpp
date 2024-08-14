#include "simulation_game_engine_player.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_world.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <networking\replication\replication_entity_manager.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <memory\tls.h>

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
	assert((word)player_index >= 0 && (word)player_index <= k_maximum_players);
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set(update_flag, true);
	simulation_action_game_engine_player_update((word)player_index, &update_flags);
}

void simulation_action_player_taunt_request(short player_index)
{
	assert(player_index != NONE);
	if (game_is_distributed() && game_is_server() && !game_is_playback())
	{
		s_simulation_player_taunt_request_data payload_data = {};
		payload_data.player_index = player_index;
		simulation_event_generate_for_clients(_simulation_event_type_player_taunt_request, 0, 0, NONE, sizeof(payload_data), &payload_data);
	}
}

bool __stdcall c_simulation_player_respawn_request_event_definition__apply_game_event(long reference_gamestate_count, const datum_index* gamestate_indicies, long payload_size, const s_player_respawn_target_request_event_data* payload)
{
	short player_absolute_index = payload->absolute_player_index;
	short spectating_player_absolute_index = payload->player_index;

	if (!VALID_INDEX(player_absolute_index, k_maximum_players))
		return false;
	if (spectating_player_absolute_index != 65535 && spectating_player_absolute_index >= k_maximum_players)
		return false;
	if (spectating_player_absolute_index == player_absolute_index)
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player_data = (player_datum*)datum_try_and_get_absolute(*players, player_absolute_index);
	if (!player_data || player_data->unit_index != NONE)
		return false;

	player_data->spectating_player_index = player_index_from_absolute_player_index(spectating_player_absolute_index);
	simulation_action_game_engine_player_update(player_absolute_index, _simulation_player_update_spectating_player);
	return true;
}