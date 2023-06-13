#include "simulation_game_events.h"
#include <game\players.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_action.h>

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
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set_raw_bits(0x8000i64);
	simulation_action_game_engine_player_update(player_absolute_index, &update_flags);
	return true;
}