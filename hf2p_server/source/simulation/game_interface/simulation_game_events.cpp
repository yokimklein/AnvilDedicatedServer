#include "simulation_game_events.h"
#include <game\players.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_action.h>
#include "assert.h"
#include <game\game.h>
#include <iostream>
#include <simulation\game_interface\simulation_game_objects.h>

bool __stdcall c_simulation_player_respawn_request_event_definition__apply_game_event(long reference_gamestate_count, const long* gamestate_indicies, long payload_size, const long* payload)
{
	short player_absolute_index = *payload;
	short spectating_player_absolute_index = *(payload + 1);

	if (player_absolute_index < 0 || player_absolute_index >= 16)
		return false;
	if (spectating_player_absolute_index != 65535 && spectating_player_absolute_index >= 16u)
		return false;
	if (spectating_player_absolute_index == player_absolute_index)
		return false;

	s_player_datum* player_data = (s_player_datum*)datum_try_and_get_absolute(get_tls()->players, player_absolute_index);
	if (!player_data || player_data->unit_index != -1)
		return false;

	player_data->spectating_player_index = player_index_from_absolute_player_index(spectating_player_absolute_index);
	simulation_action_game_engine_player_update(player_absolute_index, _simulation_player_update_spectating_player);
	return true;
}

void simulation_event_generate_for_remote_peers(e_simulation_event_type event_type, long object_references_count, long* object_references, long ignore_player_index, long payload_size, void const* payload_data)
{
	const auto simulation_event_generate_for_remote_peers_call = (void (*)())base_address(0x7E490);
	__asm
	{
		push payload_data
		push payload_size
		push ignore_player_index
		push object_references
		mov edx, object_references_count
		mov ecx, event_type
		call simulation_event_generate_for_remote_peers_call
		add esp, 16
	}
}

void simulation_event_generate_for_clients(e_simulation_event_type event_type, long object_references_count, long* object_references, long ignore_player_index, long payload_size, void const* payload_data)
{
	assert(game_is_authoritative());
	simulation_event_generate_for_remote_peers(event_type, object_references_count, object_references, ignore_player_index, payload_size, payload_data);
}

void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type)
{
	if (game_is_distributed() && game_is_server() && !game_is_playback())
	{
		if (simulation_object_get_authoritative_entity(object_index) == -1)
		{
			const char* object_description;
			if (object_index == -1)
				object_description = "NONE";
			else
				object_description = object_describe(object_index);
			printf("MP/NET/SIMULATION,EVENT: simulation_action_damage_section_response: damage section on [%s] fired without an entity index\n", object_description);
		}
		else
		{
			s_simulation_damage_section_response_data response_data = {};
			response_data.damage_section_index = damage_section_index;
			response_data.response_index = response_index;
			response_data.damage_section_response_type = damage_section_response_type;
			long object_references = object_index;
			simulation_event_generate_for_clients(_simulation_event_type_damage_section_response, 1, &object_references, -1, sizeof(s_simulation_damage_section_response_data), &response_data);
		}
	}
}