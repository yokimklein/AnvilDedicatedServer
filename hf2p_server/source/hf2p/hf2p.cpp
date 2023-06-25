#include "hf2p.h"
#include <memory\tls.h>
#include <game\game.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <anvil\server_tools.h>

void hf2p_player_podium_initialize(long podium_biped_index, long player_index)
{
	s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
	s_player_podium* player_podium = &g_player_podiums[*g_player_podiums_count];
	*g_player_podiums_count = *g_player_podiums_count + 1;
	player_podium->current_action = _podium_action_base;
	player_podium->ticks = get_tls()->game_time_globals->elapsed_ticks;
	player_podium->player_index = player_index;
	player_podium->player_model_index = player_data->configuration.host.player_appearance.player_model_choice;
	player_podium->female = player_data->configuration.host.player_appearance.flags & 1;
	player_podium->loop_count = 0;
	player_podium->object_index = -1;
	player_podium->object_index2 = -1;
	player_podium->object_index3 = -1;
	player_podium->stance_index = 0;
	player_podium->move_index = 0;
}

void hf2p_trigger_player_podium_taunt(long player_podium_index)
{
	s_player_podium* player_podium = &g_player_podiums[player_podium_index];
	static bool key_held_delete = false;
	if (player_is_local(player_podium->player_index) && player_podium->loop_count <= 0 && anvil_key_pressed(VK_DELETE, &key_held_delete)) // TODO: wtf are the byte checks in this statement in ms30?
	{
		hf2p_player_podium_increment_loop_count(player_podium->player_index);
		if (game_is_multiplayer() && !game_is_playback())
		{
			long payload_data = (word)player_podium->player_index;
			if (game_is_predicted())
				simulation_event_generate_for_remote_peers(_simulation_event_type_player_taunt_request, 0, nullptr, -1, 4, &payload_data);
			else if (game_is_server())
				simulation_event_generate_for_clients(_simulation_event_type_player_taunt_request, 0, nullptr, -1, 4, &payload_data);
		}
	}
}

void hf2p_player_podium_increment_loop_count(long player_index)
{
	DECLFUNC(base_address(0x2E8430), void, __thiscall, long)(player_index);
}