#include "session_control.h"
#include "networking\logic\network_life_cycle.h"
#include "interface\user_interface_session.h"
#include "interface\user_interface_networking.h"
#include "networking\logic\network_session_interface.h"
#include "anvil\backend\backend.h"
#include "networking\session\network_session.h"
#include "anvil\backend\cache.h"
#include "networking\session\network_managed_session.h"
#include "game\game.h"
#include "anvil\backend\services\private_service.h"
#include "anvil\config.h"
#include "anvil\server_tools.h"
#include "anvil\session_voting.h"
#include <networking\network_utilities.h>
#include <cseries\cseries_events.h>

bool anvil_session_create()
{
    event(_event_message, "networking:" __FUNCTION__ ": creating session...");
    network_life_cycle_end();
    if (!network_life_cycle_create_local_squad(_network_session_class_online))
    {
        event(_event_error, "networking:" __FUNCTION__ ": session creation failed!");
        return false;
    }
    user_interface_set_desired_multiplayer_mode(_desired_multiplayer_mode_custom_games);
    network_session_interface_set_local_name(L"ANVIL_DEDICATED", L"ANVIL_DEDICATED_SESSION");
    return true;
}

void anvil_session_update()
{
    // $TODO: move these debug keys somewhere better
    static bool logged_connection_info = true;
    static bool key_held_home = false;
    static bool key_held_end = false;
    static bool key_held_insert = false;
    static bool key_held_pgup = false;
    static bool key_held_pgdown = false;
    //static bool key_held_delete = false; // used for podium taunts

    if (!life_cycle_globals.initialized || !network_initialized())
    {
        return;
    }

    c_backend::update();

    // Wait until we're connected to the API before proceeding
    // $TODO: handle disconnects, and attempt to reconnect
    if (!c_backend::ready())
    {
        return;
    }

    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();

    // If there's no session, create one once we have a lobby from the API
    if (session->disconnected() && g_backend_data_cache.m_lobby_info.valid)
    {
        anvil_session_create();
        logged_connection_info = false;
    }

    // log session connection info
    if (!logged_connection_info)
    {
        // wait for the managed session to create & for the session to establish
        s_online_managed_session* managed_session = &online_session_manager_globals.managed_sessions[session->managed_session_index()];
        if (managed_session->flags.test(_online_managed_session_created_bit) && session->established())
        {
            logged_connection_info = true;
            char address_str[0x100];
            s_transport_secure_identifier secure_identifier = {};
            s_transport_secure_address secure_address = {};
            if (transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_windows, &secure_identifier, &secure_address))
            {
                event(_event_message, "networking:" __FUNCTION__ ": session ready! address: [%s]",
                    transport_address_to_string(&transport_security_globals.address, NULL, address_str, 0x100, true, false));
                event(_event_message, "networking:" __FUNCTION__ ": serverid: [%s] lobbyid: [%s]",
                    transport_secure_address_get_string(&secure_address),
                    transport_secure_identifier_get_string(&secure_identifier));
            }
            else
            {
                event(_event_error, "networking:" __FUNCTION__ ": session failed to retrieve security info!");
            }
            if (game_is_dedicated_server())
            {
                // update server info on API
                s_transport_secure_address server_identifier;
                anvil_get_server_identifier(&server_identifier);
                c_backend::private_service::update_game_server::request
                (
                    transport_secure_address_get_string(&server_identifier),
                    // $TODO: pull this IP from somewhere
                    g_anvil_configuration["server_address"], //transport_address_to_string(&transport_security_globals.address, NULL, address_str, 0x100, false, false);
                    transport_security_globals.address.port,
                    g_anvil_configuration["playlist_id"]
                );
                // set default dedicated server state
                e_dedicated_server_session_state session_state = _dedicated_server_session_state_matchmaking_session;
                session->get_session_parameters()->m_parameters.dedicated_server_session_state.set(&session_state);
            }
        }
    }

    // run anvil session loop once the session is setup
    if (!session->established())
    {
        return;
    }

    c_network_session_membership* membership = session->get_session_membership_for_update();
    c_network_session_parameters* parameters = session->get_session_parameters();

    // debug server controls
    if (anvil_key_pressed(VK_NEXT, &key_held_pgdown)) // begin voting
    {
        anvil_session_start_voting(session);
    }
    else if (anvil_key_pressed(VK_HOME, &key_held_home))
    {
        event(_event_message, "networking:" __FUNCTION__ ": launching session...");
        parameters->m_parameters.session_mode.set(_network_session_mode_setup);
    }
    else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
    {
        event(_event_message, "networking:" __FUNCTION__ ": disconnecting session...");
        session->disconnect();

        /*
        // load new string from text file
        std::ifstream file("map_load.txt");
        std::string scenario_path_str;
        std::string map_name_str;
        if (std::getline(file, scenario_path_str))
        {
            if (!std::getline(file, map_name_str))
            {
                printf("no .map name provided, using -1 map id\n");
                anvil_launch_scenario(scenario_path_str.c_str(), nullptr);
            }
            else
            {
                wchar_t map_name[128];
                ascii_string_to_wchar_string(map_name_str.c_str(), map_name, map_name_str.length(), nullptr);
                anvil_launch_scenario(scenario_path_str.c_str(), map_name);
            }
        }
        else
        {
            printf("map_load.txt mising scenario path line!\n");
        }
        */
        //printf("Starting session countdown...\n");
        //parameters->countdown_timer.set(_network_game_countdown_delayed_reason_none, 5);
        //e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
        //parameters->dedicated_server_session_state.set(&session_state);
    }
    else if (anvil_key_pressed(VK_END, &key_held_end))
    {
        event(_event_message, "networking:" __FUNCTION__ ": ending game...");
        parameters->m_parameters.session_mode.set(_network_session_mode_end_game);
    }
    else if (anvil_key_pressed(VK_INSERT, &key_held_insert))
    {
        event(_event_message, "networking:" __FUNCTION__ ": setting test mode...");
        anvil_session_set_gamemode(session, _game_engine_type_slayer, 0, 0);
        anvil_session_set_map(_riverworld);
    }

    // update voting
    if (parameters->m_parameters.dedicated_server_session_state.get_allowed()) // skip if data is not available
    {
        anvil_session_update_voting(session);
    }
}