#include "server_tools.h"
#include <iostream>
#include "..\hf2p\hf2p_session.h"
#include "..\interface\user_interface_session.h"
#include "..\networking\session\network_session.h"
#include "..\text\unicode.h"
#include "..\hf2p\hq.h"
#include "..\memory\tls.h"

bool anvil_create_session()
{
    printf("Creating session...\n");
    if (!hf2p_setup_session())
    {
        printf("Session creation failed!\n");
        return false;
    }
    user_interface_set_desired_multiplayer_mode(_desired_multiplayer_mode_custom_games);
    return true;
}

bool anvil_session_set_map(e_map_id map_id)
{
    c_map_variant map_variant = c_map_variant();
    map_variant.create_default(map_id);
    bool success = user_interface_squad_set_multiplayer_map(&map_variant);
    if (success == false)
        printf("Failed to set map variant!\n");
    return success;
}

bool anvil_session_set_gamemode(c_network_session* session, e_engine_variant engine_variant)
{
    c_game_variant game_variant = c_game_variant();
    build_default_game_variant(&game_variant, engine_variant);
    //game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_round_time_limit = 1;
    //game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_number_of_rounds = 3;
    //game_variant.m_storage.m_slayer_variant.m_score_to_win = 1;

    //game_variant.m_storage.m_base_variant.m_respawn_options.m_flags.set(_game_engine_respawn_options_auto_respawn_disabled, false);
    //wchar_t variant_name[16] = L"RESPAWN TEST";
    //ustrnzcpy(game_variant.m_storage.m_base_variant.m_metadata.m_name, variant_name, 16);

    if (!session->get_session_parameters()->ui_game_mode.request_change(_gui_game_mode_multiplayer))
    {
        printf("Failed to set UI game mode!\n");
        return false;
    }
    if (!user_interface_squad_set_game_variant(&game_variant))
    {
        printf("Failed to set game variant!\n");
        return false;
    }
    return true;
}

void anvil_session_set_test_player_data(c_network_session_membership* membership)
{
    wchar_t service_tag[5] = L"TEST";
    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
    {
        auto current_player = membership->get_player(i);
        auto host_configuration = &current_player->configuration.host;
        memcpy(&host_configuration->player_appearance.service_tag, service_tag, 10);
        host_configuration->user_selected_team_index = _game_team_blue;
        host_configuration->team_index = _game_team_blue;
        host_configuration->s3d_player_customization.colors[_primary] = 0x0F0F0F;
        host_configuration->s3d_player_customization.colors[_secondary] = 0x003750;
        host_configuration->s3d_player_customization.colors[_visor] = 0xFF640A;
        host_configuration->s3d_player_customization.colors[_lights] = 0xFF640A;
        host_configuration->s3d_player_customization.colors[_holo] = 0xFF640A;
        host_configuration->s3d_player_container.loadouts[0].armor_suit = _armor_scout;
        host_configuration->s3d_player_container.loadouts[0].primary_weapon = _smg_v5;
        host_configuration->s3d_player_container.loadouts[0].secondary_weapon = _magnum_v1;
        host_configuration->s3d_player_container.loadouts[0].tactical_packs[0] = _adrenaline;
        host_configuration->s3d_player_container.loadouts[0].tactical_packs[1] = _bomb_run;
        host_configuration->s3d_player_container.loadouts[0].tactical_packs[2] = _concussive_blast;
        host_configuration->s3d_player_container.loadouts[0].tactical_packs[3] = _hologram;
        //host_configuration->s3d_player_appearance.modifiers[0].modifier_values[_plant_plasma_on_death] = 1;
        //host_configuration->s3d_player_appearance.modifiers[0].modifier_values[_safety_booster] = 1;
        //host_configuration->s3d_player_appearance.modifiers[0].modifier_values[_grenade_warning] = 1;

        // display server-set loadouts in the client's UI
        host_configuration->s3d_player_customization.override_api_data = true;
        host_configuration->s3d_player_container.override_api_data = true;
        // zzVertigo user id
        host_configuration->player_xuid.data = 2;

        // host player data
        if (current_player->peer_index == membership->host_peer_index())
        {
            wchar_t host_name[16] = L"HOST PLAYER";
            host_configuration->s3d_player_container.loadouts[0].armor_suit = _armor_pilot;
            memcpy(&host_configuration->player_name, host_name, 32);
            host_configuration->user_selected_team_index = _game_team_red;
            host_configuration->team_index = _game_team_red;
            current_player->controller_index = 0;
            // JocKe user id
            host_configuration->player_xuid.data = 1;
        }
    }
    // push update
    membership->increment_update();
}

bool anvil_key_pressed(long vkey, bool* key_held)
{
    bool key_pressed = false;

    if ((GetKeyState(vkey) & 0x8000))
    {
        if (*key_held == false)
            key_pressed = true;
        else
            key_pressed = false;
        *key_held = true;
    }
    else
    {
        *key_held = false;
        key_pressed = false;
    }
    return key_pressed;
}

void anvil_launch_scenario(const char* scenario_path)
{
    //csstrnzcpy(g_tutorial_scenario_path, scenario_path, 40);
    //csmemcpy(g_tutorial_scenario_path, scenario_path, strlen_debug(scenario_path));
    hq_start_tutorial_level();
}

long anvil_get_update_rate_ms()
{
    // TODO: get main game tls and save address globally so we can access it here
    // OR add AS update function to the game's main loop
    return 0; // get_tls()->game_time_globals->seconds_per_tick * 1000
}

// TODO: use this to retrieve a secure address from the API matchmaking service
void anvil_get_dedicated_secure_address(s_transport_secure_address* secure_address)
{
    // cdbeb834-c1a4-9a4d-8dda-f540f378d644
    byte temp_address[16] = { 0x34, 0xb8, 0xbe, 0xcd, 0xa4, 0xc1, 0x4d, 0x9a, 0x8d, 0xda, 0xf5, 0x40, 0xf3, 0x78, 0xd6, 0x44 };
    memcpy(secure_address, temp_address, sizeof(s_transport_secure_address));
}

// TODO: above, but retrieve the lobby/party ID
void anvil_get_dedicated_secure_identifier(s_transport_secure_identifier* secure_identifier)
{
    // 379bd202-9787-bd4a-ad96-a89bae9c7e4a
    byte temp_identifier[16] = { 0x02, 0xd2, 0x9b, 0x37, 0x87, 0x97, 0x4a, 0xbd, 0xad, 0x96, 0xa8, 0x9b, 0xae, 0x9c, 0x7e, 0x4a };
    memcpy(secure_identifier, temp_identifier, sizeof(s_transport_secure_identifier));
}