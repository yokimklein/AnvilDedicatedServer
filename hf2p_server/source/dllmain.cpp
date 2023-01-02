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

// set to false to run the game as a client with host features
bool game_is_dedicated_server()
{
    // currently only prevents the game from creating a player instance for the host in a session
    // i'll eventually also make this run the game headerless & use it to stop client specific code from running
    return false;
}

long main_thread()
{
    FILE* f;
    AllocConsole();
    setlocale(LC_ALL, "");
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("Anvil Station Dedicated Server\n");
    printf(anvil_dedi_build_version());
    printf("\nBase address: %p\n\n", (void*)module_base);

    enable_memory_write(module_base);
    anvil_dedi_apply_patches();
    anvil_dedi_apply_hooks();

    // SESSION MAP & VARIANT
    auto map_id = _s3d_edge;
    auto engine_variant = _engine_variant_ctf;

    // g_is_loading is false when the game first launches, we need to wait for it else we'll immediately think the game has finished loading
    while (!main_game_change_in_progress()) Sleep(k_game_tick_rate);
    // we also need to wait for the network to avoid accessing uninitialised memory
    while (!network_initialized()) Sleep(k_game_tick_rate);

    c_network_session* network_session = network_get_session_manager()->session[0];

    // thread loop
    while (true)
    {
        // wait to finish loading
        while (main_game_change_in_progress()) Sleep(k_game_tick_rate);

        // create the session
        anvil_create_session();

        // wait for the managed session to create & for the session to establish
        auto managed_session = &online_session_manager_globals->managed_sessions[network_session->managed_session_index()];
        while (!managed_session->flags.test(_online_managed_session_created_bit) || !network_session->established()) Sleep(k_game_tick_rate);

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
        auto start_status_parameter = &network_session->get_session_parameters()->game_start_status;
        e_session_game_start_status* start_status = &start_status_parameter->m_data.game_start_status;
        e_session_game_start_status start_status_previous = (e_session_game_start_status)-1;
        e_session_game_start_error* start_error = &start_status_parameter->m_data.game_start_error;
        e_session_game_start_error start_error_previous = (e_session_game_start_error)-1;

        while (network_session->established())
        {
            if (start_status_parameter->get_allowed())
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
                if (*start_error == _start_error_no_map_selected)
                {
                    anvil_session_set_gamemode(network_session, engine_variant);
                    anvil_session_set_map(map_id);
                    // wait for the error to update so we don't spam the set map call
                    while (*start_error == _start_error_no_map_selected) Sleep(k_game_tick_rate);
                }
                // enable the launch key when the session is ready
                else if (*start_status == _start_status_ready && anvil_key_pressed(VK_HOME, &key_held_home))
                {
                    printf("Launching session...\n");
                    network_session->m_session_parameters.session_mode.set(_network_session_mode_setup);
                }
                // enable the end game key once a game is started
                else if (*start_status == _start_status_none && anvil_key_pressed(VK_END, &key_held_end))
                {
                    printf("Ending game...\n");
                    network_session->m_session_parameters.session_mode.set(_network_session_mode_end_game);
                }
            }
            // always available in established session
            if (anvil_key_pressed(VK_INSERT, &key_held_insert))
            {
                printf("Setting test player data...\n");
                anvil_session_set_test_player_data(network_session->get_session_membership());
            }
            if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
            {
                printf("Setting simulation protocol to synchronous...\n");
                network_session->get_session_parameters()->game_simulation_protocol.m_data = _network_game_simulation_synchronous;
                network_session->get_session_parameters()->game_simulation_protocol.notify_set_update_required();
            }
            if (anvil_key_pressed(VK_NEXT, &key_held_pgdown))
            {
                printf("Setting simulation protocol to distributed...\n");
                network_session->get_session_parameters()->game_simulation_protocol.m_data = _network_game_simulation_distributed;
                network_session->get_session_parameters()->game_simulation_protocol.notify_set_update_required();
            }
            Sleep(k_game_tick_rate);
        }
        printf("Session disconnected!\n");
    }

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