#include "dllmain.h"
#include <iostream>
#include "anvil\dllproxy.h"
#include "anvil\build_version.h"
#include "anvil\hooks.h"
#include "anvil\server_tools.h"
#include "networking\session\network_session.h"
#include "game\game_engine.h"
#include "networking\session\network_session_manager.h"
#include "interface\user_interface_session.h"
#include "hf2p\hf2p_session.h"
#include "networking\network_utilities.h"
#include "networking\session\network_managed_session.h"
#include "main\main_game.h"
#include "hf2p\hq.h"

// TODO: enable write on rdata page to overwrite tutorial scenario path for easy scenario laucnhing
void enable_memory_write(dword base)
{
    // Enable write to all executable memory
    size_t Offset, Total;
    Offset = Total = 0;
    MEMORY_BASIC_INFORMATION MemInfo;

    while (VirtualQuery((byte*)base + Offset, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
    {
        Offset += MemInfo.RegionSize;
        if (MemInfo.Protect == PAGE_EXECUTE_READ)
        {
            Total += MemInfo.RegionSize;
            VirtualProtect(MemInfo.BaseAddress, MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &MemInfo.Protect);
        }
    }
}

bool game_is_dedicated_server()
{
    return true;
}

long main_thread()
{
    FILE* f;
    AllocConsole();
    setlocale(LC_ALL, "");
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("%s %s\n", anvil_get_project_name_string(), anvil_get_config_string());
    printf("%s\n", anvil_build_version_string());
    printf("Base address: %p\n\n", (void*)module_base);

    enable_memory_write(module_base);
    anvil_dedi_apply_patches();
    anvil_dedi_apply_hooks();

    // SESSION MAP & VARIANT
    auto map_id = _s3d_edge;
    auto engine_variant = _engine_variant_ctf;

    // g_is_loading is false when the game first launches, we need to wait for it else we'll immediately think the game has finished loading
    while (!main_game_change_in_progress()) Sleep(k_anvil_update_rate_ms);
    // we also need to wait for the network to avoid accessing uninitialised memory
    while (!network_initialized()) Sleep(k_anvil_update_rate_ms);

    c_network_session* network_session = network_get_session_manager()->session[0];

    // thread loop
    while (true)
    {
        // wait to finish loading
        while (main_game_change_in_progress()) Sleep(k_anvil_update_rate_ms);

        // create the session
        anvil_create_session();

        // wait for the managed session to create & for the session to establish
        auto managed_session = &online_session_manager_globals->managed_sessions[network_session->managed_session_index()];
        while (!managed_session->flags.test(_online_managed_session_created_bit) || !network_session->established()) Sleep(k_anvil_update_rate_ms);

        // log session connection info
        char address_str[0x100];
        s_transport_secure_identifier secure_identifier = {};
        s_transport_secure_address secure_address = {};
        if (transport_secure_identifier_retrieve(&transport_security_globals->address, _transport_platform_pc, &secure_identifier, &secure_address))
        {
            printf("\nSession ready to join!\nServer Address: %s\nSecure Address: %s\nSecure Identifier: %s\n\n",
                transport_address_to_string(&transport_security_globals->address, nullptr, address_str, 0x100, true, false),
                transport_secure_address_get_string(&secure_address),
                transport_secure_identifier_get_string(&secure_identifier));
        }
        else
        {
            printf("\nSession failed to retrieve security info.\nServer Address: %s\n\n",
                transport_address_to_string(&transport_security_globals->address, nullptr, address_str, 0x100, true, false));
        }

        bool key_held_home = false;
        bool key_held_end = false;
        bool key_held_insert = false;
        bool key_held_pgup = false;
        bool key_held_pgdown = false;
        bool key_held_delete = false;
        // wait for game_start_status to unlock
        while (!network_session->get_session_parameters()->game_start_status.get_allowed());
        auto start_status_parameter = network_session->get_session_parameters()->game_start_status.get();
        e_session_game_start_status* start_status = (e_session_game_start_status*)&start_status_parameter->game_start_status;
        e_session_game_start_status start_status_previous = _session_game_start_status_none;
        e_session_game_start_error* start_error = (e_session_game_start_error*)&start_status_parameter->game_start_error;
        e_session_game_start_error start_error_previous = _session_game_start_error_none;

        e_dedicated_server_session_state session_state = _dedicated_server_session_state_waiting_for_players;
        network_session->get_session_parameters()->dedicated_server_session_state.set(&session_state);

        while (network_session->established())
        {
            // log if the status or error codes have updated
            if (*start_status != start_status_previous || *start_error != start_error_previous)
            {
                printf("start status updated: %s (error: %s)\n",
                    multiplayer_game_start_status_to_string(*start_status),
                    multiplayer_game_start_error_to_string(*start_error));
                start_status_previous = *start_status;
                start_error_previous = *start_error;
            }

            // set the map when none is selected
            if (*start_error == _session_game_start_error_no_map_selected)
            {
                anvil_session_set_gamemode(network_session, engine_variant);
                anvil_session_set_map(map_id);
                // wait for the error to update so we don't spam the set map call
                while (*start_error == _session_game_start_error_no_map_selected) Sleep(k_anvil_update_rate_ms);
            }
            // enable the launch key when the session is ready
            else if (*start_status == _session_game_start_status_ready_leader)
            {
                if (anvil_key_pressed(VK_HOME, &key_held_home))
                {
                    printf("Launching session...\n");
                    network_session->m_session_parameters.session_mode.set(_network_session_mode_setup);
                }
                else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
                {
                    printf("Starting session countdown...\n");
                    network_session->get_session_parameters()->countdown_timer.set(_countdown_type_game_start, 5);
                    e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
                    network_session->get_session_parameters()->dedicated_server_session_state.set(&session_state);
                }
                else if (anvil_key_pressed(VK_NEXT, &key_held_pgdown))
                {
                    printf("Lobby vote set test...\n");
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[0].gamemode = 1;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[0].map = 9;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[0].number_of_votes = 0;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[1].gamemode = 0;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[1].map = 10;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.vote_options[1].number_of_votes = 0;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.__unknown6 = 0;
                    network_session->get_session_parameters()->lobby_vote_set.m_data.__unknown7 = 0;

                    network_session->get_session_parameters()->lobby_vote_set.m_state_flags |= 1;
                    memset(network_session->get_session_parameters()->lobby_vote_set.m_peers_updated, 0, k_network_maximum_machines_per_session);
                    network_session->get_session_parameters()->lobby_vote_set.notify_set_update_required();
                    e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
                    network_session->get_session_parameters()->dedicated_server_session_state.set(&session_state);
                    network_session->get_session_parameters()->countdown_timer.set(_countdown_type_voting, 10);
                }
            }
            // start countdown after voting finishes
            else if (*start_status == _session_game_start_status_voting &&
                *network_session->get_session_parameters()->dedicated_server_session_state.get() == _dedicated_server_session_state_voting &&
                network_session->get_session_parameters()->countdown_timer.get_countdown_type() == _countdown_type_voting &&
                network_session->get_session_parameters()->countdown_timer.get_countdown_timer() <= 0)
            {
                network_session->get_session_parameters()->countdown_timer.set(_countdown_type_game_start, 5);
                e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
                network_session->get_session_parameters()->dedicated_server_session_state.set(&session_state);
            }
            // enable the end game key once a game is started
            else if (*start_status == _session_game_start_status_none && anvil_key_pressed(VK_END, &key_held_end))
            {
                printf("Ending game...\n");
                network_session->m_session_parameters.session_mode.set(_network_session_mode_end_game);
            }

            // always available in established session
            if (anvil_key_pressed(VK_INSERT, &key_held_insert))
            {
                printf("Setting test player data...\n");
                anvil_session_set_test_player_data(network_session->get_session_membership());
            }
            else if (anvil_key_pressed(VK_DELETE, &key_held_delete))
            {
                printf("Setting session to synchronous...\n");
                e_network_game_simulation_protocol protocol = _network_game_simulation_synchronous;
                network_session->get_session_parameters()->game_simulation_protocol.set(&protocol);
            }
            Sleep(k_anvil_update_rate_ms);
        }
        printf("Session disconnected!\n");
    }
    
    /*
    // temp scenario launching loop
    bool key_held_home = false;
    while (true)
    {
        if (anvil_key_pressed(VK_HOME, &key_held_home))
        {
            printf("Launching scenario...\n");
            anvil_launch_scenario("levels\\multi\\guardian\\guardian");
        }
    }
    */
    // cleanup & eject
    fclose(f);
    FreeConsole();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        // disable dpi scaling
        SetProcessDPIAware();
        // disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH (reduces the working set size)
        DisableThreadLibraryCalls(hModule);

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&main_thread, NULL, 0, NULL);
        return true;
    case DLL_PROCESS_DETACH:
        return FreeLibrary(hModule);
    case DLL_THREAD_ATTACH:
        return true;
    case DLL_THREAD_DETACH:
        return true;
    }
    return TRUE;
}