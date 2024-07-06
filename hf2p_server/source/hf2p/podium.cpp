#include "podium.h"
#include <memory\tls.h>
#include <game\game.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <anvil\server_tools.h>

REFERENCE_DECLARE(0x4A2973C, long, g_player_podium_count);
REFERENCE_DECLARE_ARRAY(0x4A29740, s_player_podium, g_player_podiums, k_maximum_multiplayer_players);

void hf2p_player_podium_initialize(long podium_biped_index, long player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);

	s_player_datum* player_data = (s_player_datum*)datum_get(*players, player_index);
	s_player_podium& player_podium = g_player_podiums[g_player_podium_count++];
	player_podium.current_action = _podium_action_base;
	player_podium.ticks = game_time_globals->elapsed_ticks;
	player_podium.player_index = player_index;
	player_podium.player_model_index = player_data->configuration.host.player_appearance.player_model_choice;
	player_podium.female = TEST_BIT(player_data->configuration.host.player_appearance.flags, 0);
	player_podium.loop_count = 0;
	player_podium.player_unit_indices[0] = NONE;
	player_podium.player_unit_indices[1] = NONE;
	player_podium.player_unit_indices[2] = NONE;
	player_podium.stance_index = 0;
	player_podium.move_index = 0;
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
			s_simulation_player_taunt_request_data payload_data = {};
			payload_data.player_index = (word)player_podium->player_index;
			if (game_is_predicted())
				simulation_event_generate_for_remote_peers(_simulation_event_type_player_taunt_request, 0, nullptr, -1, sizeof(s_simulation_player_taunt_request_data), &payload_data);
			else if (game_is_server()) // TODO: i'm pretty sure this call goes in apply_event_update
				simulation_event_generate_for_clients(_simulation_event_type_player_taunt_request, 0, nullptr, -1, sizeof(s_simulation_player_taunt_request_data), &payload_data);
		}
	}
}

void __fastcall hf2p_player_podium_increment_loop_count(long player_index)
{
	INVOKE(0x2E8430, hf2p_player_podium_increment_loop_count, player_index);
}

/*
// TODO: replace login function in essential_components_initialize
// TODO: dedi check in unknown func called by user_interface_update
// TODO: replace client tick in hf2p_tick
// TODO: disable sound and rendering in game_globals_initialize

void hf2p_init_dedicated_server()
{
	// some global = 0 here
	//hf2p_init_game_statistics();
	//hf2p_init_user_data();
	//hf2p_init_clan_data(); // ClanDataStorage
	//hf2p_init_user_base_data();
	//hf2p_init_clan_service(); // ClanGetMembership
	//hf2p_init_session_control_service();
	//hf2p_init_user_base_data();
}

void hf2p_tick_dedicated_server()
{
	//hf2p_heartbeat_service_tick();
	// nullsub call here - I have no idea what this was/did
	//hf2p_session_control_service_tick();
}

void hf2p_shutdown()
{

}
*/