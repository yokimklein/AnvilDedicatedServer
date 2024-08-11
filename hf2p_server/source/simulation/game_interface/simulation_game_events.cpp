#include "simulation_game_events.h"
#include "assert.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_engine_player.h>

bool __stdcall c_simulation_player_respawn_request_event_definition__apply_game_event(long reference_gamestate_count, const long* gamestate_indicies, long payload_size, const long* payload)
{
	short player_absolute_index = *payload;
	short spectating_player_absolute_index = *(payload + 1);

	if (player_absolute_index < 0 || player_absolute_index >= 16)
		return false;
	if (spectating_player_absolute_index != 65535 && spectating_player_absolute_index >= 16)
		return false;
	if (spectating_player_absolute_index == player_absolute_index)
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(players);
	s_player_datum* player_data = (s_player_datum*)datum_try_and_get_absolute(*players, player_absolute_index);
	if (!player_data || player_data->unit_index != NONE)
		return false;

	player_data->spectating_player_index = player_index_from_absolute_player_index(spectating_player_absolute_index);
	simulation_action_game_engine_player_update(player_absolute_index, _simulation_player_update_spectating_player);
	return true;
}

void simulation_event_generate_for_remote_peers(e_simulation_event_type event_type, long object_references_count, datum_index* object_references, long ignore_player_index, long payload_size, void const* payload_data)
{
	static void* simulation_event_generate_for_remote_peers_call = (void*)BASE_ADDRESS(0x7E490);
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

void simulation_event_generate_for_clients(e_simulation_event_type event_type, long object_references_count, datum_index* object_references, long ignore_player_index, long payload_size, void const* payload_data)
{
	assert(game_is_authoritative());
	simulation_event_generate_for_remote_peers(event_type, object_references_count, object_references, ignore_player_index, payload_size, payload_data);
}