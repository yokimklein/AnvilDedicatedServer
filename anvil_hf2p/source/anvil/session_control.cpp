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
#include "networking\network_utilities.h"
#include "cseries\cseries_events.h"

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
    static bool logged_connection_info = true;

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

    c_network_session_parameters* parameters = session->get_session_parameters();

    // update voting once session is setup
    if (session->established() && parameters->m_parameters.dedicated_server_session_state.get_allowed()) // skip if data is not available
    {
        anvil_session_update_voting(session);
    }
}

bool anvil_session_set_map(e_map_id map_id)
{
    // create variant on the heap to avoid a large stack frame
    c_map_variant* map_variant = new c_map_variant();
    map_variant->create_default(map_id);
    bool success = user_interface_squad_set_multiplayer_map(map_variant);
    if (!success)
    {
        event(_event_warning, "networking:anvil:session: failed to set map variant!");
    }
    delete map_variant;
    return success;
}

bool anvil_session_set_gamemode(e_game_engine_type engine_index, long variant_index, ulong time_limit)
{
    if (!VALID_INDEX(engine_index, k_game_engine_type_count))
    {
        event(_event_warning, "networking:anvil:session: cannot set invalid engine index [%d]!", engine_index);
        return false;
    }

    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session)
    {
        event(_event_warning, "networking:anvil:session: cannot set gamemode with invalid session!");
        return false;
    }

    if (!game_is_authoritative())
    {
        event(_event_warning, "networking:anvil:session: not authority - cannot set gamemode!");
        return false;
    }

    c_game_variant game_variant = c_game_variant();
    if (!game_engine_tag_defined_variant_get_built_in_variant(engine_index, variant_index, &game_variant))
    {
        event(_event_warning, "networking:anvil:session: failed to get game variant!");
        return false;
    }

    game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_round_time_limit_minutes(time_limit);

    if (!session->get_session_parameters()->m_parameters.ui_game_mode.request_change(_gui_game_mode_multiplayer))
    {
        event(_event_warning, "networking:anvil:session: failed to set ui game mode!");
        return false;
    }
    if (!user_interface_squad_set_game_variant(&game_variant))
    {
        event(_event_warning, "networking:anvil:session: failed to set game variant!");
        return false;
    }
    return true;
}

bool anvil_session_start_countdown()
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session)
    {
        event(_event_warning, "networking:anvil:session: cannot start countdown with invalid session!");
        return false;
    }

    if (!game_is_authoritative())
    {
        event(_event_warning, "networking:anvil:session: not authority - cannot start countdown!");
        return false;
    }

    event(_event_status, "networking:anvil:session: starting session countdown...");

    c_network_session_parameters* parameters = session->get_session_parameters();
    if (!parameters->m_parameters.countdown_timer.set(_network_game_countdown_delayed_reason_none, 5))
    {
        return false;
    }

    e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
    if (!parameters->m_parameters.dedicated_server_session_state.set(&session_state))
    {
        return false;
    }

    return true;
}