#include "server_tools.h"
#include <iostream>
#include "..\hf2p\hf2p_session.h"
#include "..\interface\user_interface_session.h"
#include "..\networking\session\network_session.h"

bool anvil_create_session()
{
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
    // set test data for each player
    wchar_t service_tag[5] = L"TEST";
    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
    {
        auto current_player = membership->get_player(i);
        auto host_configuration = &current_player->configuration.host;
        memcpy(&host_configuration->player_appearance.service_tag, service_tag, 10);
        host_configuration->player_assigned_team = 1; // blue team
        host_configuration->player_team = 1; // blue team
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

        // host player data
        if (current_player->peer_index == membership->host_peer_index())
        {
            wchar_t host_name[16] = L"HOST PLAYER";
            host_configuration->s3d_player_container.loadouts[0].armor_suit = _armor_pilot;
            memcpy(&host_configuration->player_name, host_name, 32);
            host_configuration->player_assigned_team = 0; // red team
            host_configuration->player_team = 0; // red team
            current_player->controller_index = 0;
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