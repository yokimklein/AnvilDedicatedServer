#include "server_tools.h"
#include "cseries\cseries.h"
#include "interface\user_interface_session.h"
#include "networking\session\network_session.h"
#include "anvil\backend\user.h"
#include "anvil\backend\cache.h"
#include "hf2p\hq.h"
#include "anvil\hooks\hooks.h"
#include "networking\logic\network_life_cycle.h"
#include "game\game_engine_util.h"
#include "tag_files\string_ids.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <cseries\cseries_events.h>
#include "session_control.h"
#include <game\game.h>

bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration)
{
    ASSERT(configuration != nullptr);
    ASSERT(session != nullptr);
    bool player_data_updated = false;
    
    c_network_session_membership* membership = session->get_session_membership_for_update();
    const s_network_session_player* player = membership->get_player(player_index);
    const s_network_session_peer* peer = membership->get_peer(player->peer_index);

    // test dedi player loadout
    if (player->peer_index == membership->host_peer_index())
    {
        configuration->player_name.set(L"        ");
        configuration->user_xuid = USER_SYSTEM;
        configuration->player_appearance.service_tag.set(L"1225");
        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_scout;
        configuration->s3d_player_customization.colors[_armor_color_primary] = 0x00000000;
        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x00000000;
        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFFFFFFFF;
        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFFFFFFFF;
        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFFFFFFFF;
        configuration->s3d_player_container.loadouts[0].consumables[0] = _powerdrain;
        configuration->s3d_player_container.loadouts[0].consumables[1] = _invisibility;
        configuration->s3d_player_container.loadouts[0].consumables[2] = _deployable_cover;
        configuration->s3d_player_container.loadouts[0].consumables[3] = _hologram;
        configuration->s3d_player_container.loadouts[0].secondary_weapon = _energy_sword;
        configuration->s3d_player_container.modifiers[0].modifier_values[_revenge_shield_boost] = 2.0f;
        player_data_updated = true;
    }
    else if (!configuration->s3d_player_customization.override_api_data && configuration->user_xuid != USER_SYSTEM && configuration->user_xuid > USER_INVALID)
    {
        // assign player name based on peer name - $TODO: THIS IS TEMPORARY, RETRIEVE THIS FROM API W/ USER ID INSTEAD
        if (!configuration->player_name.is_equal(peer->properties.peer_name.get_string()))
        {
            configuration->player_name.set(peer->properties.peer_name.get_string());
            player_data_updated = true;
        }

        // $TODO: track public data by version number, increment when received from API
        // update when changed, would be much better than constantly writing and comparing configs
        s_cached_public_data* public_data = g_backend_data_cache.public_data_get(configuration->user_xuid);
        if (public_data)
        {
            s_s3d_player_container player_container;
            csmemset(&player_container, 0, sizeof(player_container));

            s_s3d_player_customization player_customisation;
            csmemset(&player_customisation, 0, sizeof(player_customisation));

            s_player_appearance player_appearance;
            csmemset(&player_appearance, 0, sizeof(player_appearance));

            public_data->write_configuration(&player_container, &player_customisation, &player_appearance);

            if (configuration->s3d_player_container != player_container)
            {
                configuration->s3d_player_container = player_container;
                player_data_updated = true;
            }
            if (configuration->s3d_player_customization != player_customisation)
            {
                configuration->s3d_player_customization = player_customisation;
                player_data_updated = true;
            }
            if (configuration->player_appearance != player_appearance)
            {
                configuration->player_appearance = player_appearance;
                player_data_updated = true;
            }
        }
    }

    if (player_data_updated)
    {
        membership->increment_update();
    }

    return player_data_updated;
}

void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status)
{
    if (start_status->game_start_status == _session_game_start_status_error)
    {
        event(_event_status, "networking:anvil:session: start status updated: error [%s] affected player mask [%08X]", multiplayer_game_start_error_to_string(start_status->game_start_error), start_status->player_error_mask);
    }
    else
    {
        event(_event_status, "networking:anvil:session: start status updated: status [%s] map load progress [%d]", multiplayer_game_start_status_to_string(start_status->game_start_status), start_status->map_load_progress);
    }
}

bool anvil_key_pressed(long vkey, bool* key_held)
{
    bool key_pressed = false;

    if ((GetKeyState(vkey) & 0x8000))
    {
        if (*key_held == false)
        {
            key_pressed = true;
        }
        else
        {
            key_pressed = false;
        }
        *key_held = true;
    }
    else
    {
        *key_held = false;
        key_pressed = false;
    }
    return key_pressed;
}

// map_name is optional to retrieve a map id from a .map
void anvil_launch_scenario(const char* scenario_path, const wchar_t* map_name)
{
    ASSERT(scenario_path);
    g_tutorial_scenario_path.set(scenario_path);
    if (map_name)
    {
        g_tutorial_map_name.set(map_name);
    }
    else
    {
        g_tutorial_map_name.set(L"no .map provided");
    }

    long scnr_path_address = (long)g_tutorial_scenario_path.get_string();
    patch::bytes(0x33AB0D, (byte*)&scnr_path_address, 4);
    patch::bytes(0x33AB58, (byte*)&scnr_path_address, 4);

    long map_name_address = (long)g_tutorial_map_name.get_string();
    patch::bytes(0xDD176, (byte*)&map_name_address, 4);

    event(_event_status, "" __FUNCTION__ ": launching scenario [%s] map [%ls]", g_tutorial_scenario_path.get_string(), g_tutorial_map_name.get_string());
    hq_start_tutorial_level();
}

void anvil_boot_peer(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
    {
        event(_event_warning, "networking:anvil:session: specified peer index [%d] was invalid", peer_index);
        return;
    }

    if (!game_is_authoritative())
    {
        event(_event_warning, "networking:anvil:session: not authority - cannot boot peer!");
        return;
    }

    // If we're in a game, display boot message for all associated players
    if (life_cycle_globals.state_manager.get_current_state() == _life_cycle_state_in_game)
    {
        const s_network_session_peer* peer = session->get_session_membership()->get_peer(peer_index);
        for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
        {
            if (!peer->player_mask.test(player_absolute_index))
            {
                continue;
            }

            datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
            if (player_index == NONE)
            {
                continue;
            }

            s_game_engine_event_data event_data;
            game_engine_initialize_event(_multiplayer_event_type_general, STRING_ID(game_engine, general_event_player_booted_player), &event_data);
            game_engine_set_event_effect_player_and_team(player_index, &event_data);
            game_engine_send_event(&event_data);
        }
    }

    session->host_boot_machine(peer_index, _network_session_boot_from_ui);
}

void anvil_power_sister(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
    {
        return;
    }

    c_network_session_membership* membership = session->get_session_membership_for_update();
    const s_network_session_peer* peer = membership->get_peer(peer_index);
    for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
    {
        if (!peer->player_mask.test(player_absolute_index))
        {
            continue;
        }
        datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
        if (player_index == NONE)
        {
            continue;
        }

        s_network_session_player* player = membership->get_player(player_absolute_index);
        player->configuration.host.player_name.set(L"tickle tipson");
        player->configuration.host.player_appearance.flags = 1;
        player->configuration.host.player_appearance.service_tag.set(L"SIS");
        player->configuration.host.s3d_player_container.override_api_data = true;
        player->configuration.host.s3d_player_container.loadouts[0].armor_suit = _armor_prophet;
        player->configuration.host.s3d_player_container.loadouts[0].gender = _gender_female;
        player->configuration.host.s3d_player_customization.active_loadout_index = 0;
        player->configuration.host.s3d_player_customization.colors[_armor_color_primary] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_secondary] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_lights] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_holo] = 0xFF69B4;
        membership->increment_update();
    }
}