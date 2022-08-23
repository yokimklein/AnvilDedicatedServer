#include <iostream>
#include <cstdint>
#include <windows.h>
#include <inttypes.h>
#include "dllmain.h"
#include "patch\Patch.hpp"
#include "networking\session\network_session.h"
#include "networking\messages\network_message_handler.h"
#include "interface\user_interface_networking.h"
#include "networking\network_globals.h"
#include "networking\logic\network_join.h"
#include "networking\session\network_managed_session.h"
#include "networking\transport\transport_shim.h"
#include "networking\messages\network_message_gateway.h"
#include "networking\session\network_session_manager.h"
#include "networking\logic\network_session_interface.h"
#include "memory\tls.h"

void UnprotectMemory(uintptr_t base)
{
    // Enable write to all executable memory
    size_t Offset, Total;
    Offset = Total = 0;
    MEMORY_BASIC_INFORMATION MemInfo;

    printf("Unprotecting memory...\n");
    while (VirtualQuery((uint8_t*)base + Offset, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
    {
        Offset += MemInfo.RegionSize;
        if (MemInfo.Protect == PAGE_EXECUTE_READ)
        {
            //printf("%0X\n", (size_t)((uint8_t*)GetModuleBase() + Offset));
            Total += MemInfo.RegionSize;
            VirtualProtect(MemInfo.BaseAddress, MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &MemInfo.Protect);
        }
    }
    printf("Done! Unprotected %u bytes of memory\n", Total);
}

// GLOBALS
static c_network_session* network_session = (c_network_session*)(module_base + 0x3970168); // one of many sessions
static short* g_game_port = (short*)(module_base + 0xE9B7A0);
// FUNCTIONS - TODO: sort these into their proper file locations
char(__thiscall* network_life_cycle_create_local_squad)(e_network_session_class session_class) = reinterpret_cast<decltype(network_life_cycle_create_local_squad)>(module_base + 0x2AD00);
void(__thiscall* game_variant_ctor)(c_game_variant* variant) = reinterpret_cast<decltype(game_variant_ctor)>(module_base + 0xC3460);
void(__fastcall* build_default_game_variant)(c_game_variant* out_variant, long game_engine_type) = reinterpret_cast<decltype(build_default_game_variant)>(module_base + 0xE9BE0);
bool(__thiscall* user_interface_squad_set_game_variant)(c_game_variant* game_variant) = reinterpret_cast<decltype(user_interface_squad_set_game_variant)>(module_base + 0x3ABEC0);
void(__thiscall* c_map_variant_ctor)(c_map_variant* map_variant) = reinterpret_cast<decltype(c_map_variant_ctor)>(module_base + 0xAB2F0);
char(__thiscall* c_map_variant__create_default)(c_map_variant* map_variant, e_map_id map_id) = reinterpret_cast<decltype(c_map_variant__create_default)>(module_base + 0xAB380);
char(__thiscall* user_interface_squad_set_multiplayer_map)(c_map_variant* map_variant) = reinterpret_cast<decltype(user_interface_squad_set_multiplayer_map)>(module_base + 0x3ABC60);
//char(__cdecl* levels_get_path)(long campaign_id, long map_id, char* output, rsize_t size_in_bytes) = reinterpret_cast<decltype(levels_get_path)>(module_base + 0x68D7C0);
//bool(__cdecl* network_squad_session_set_map)(long campaign_id, long map_id, char* map_path) = reinterpret_cast<decltype(network_squad_session_set_map)>(module_base + 0x5DDEC0);
//char(__cdecl* network_squad_session_set_map_variant)(c_map_variant* map_variant) = reinterpret_cast<decltype(network_squad_session_set_map_variant)>(module_base + 0x95BC60);
char(__thiscall* c_network_session_parameter_session_mode__set)(c_network_session_parameters* session_parameters, e_network_session_mode session_mode) = reinterpret_cast<decltype(c_network_session_parameter_session_mode__set)>(module_base + 0x2D820);
bool(__fastcall* hf2p_setup_session)() = reinterpret_cast<decltype(hf2p_setup_session)>(module_base + 0x3AAF10);
void(__fastcall* user_interface_set_desired_multiplayer_mode)(e_desired_multiplayer_mode multiplayer_mode) = reinterpret_cast<decltype(user_interface_set_desired_multiplayer_mode)>(module_base + 0x3AA7D0);
bool(__thiscall* c_network_session_parameter_ui_game_mode__request_change)(c_network_session_parameter_ui_game_mode* parameter, e_gui_game_mode gui_gamemode) = reinterpret_cast<decltype(c_network_session_parameter_ui_game_mode__request_change)>(module_base + 0x3B4D0);
void(__fastcall* network_life_cycle_end)() = reinterpret_cast<decltype(network_life_cycle_end)>(module_base + 0x2AC20);

// HOOK FUNCTIONS
// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unknown, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}

// add back missing message handlers
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, bool unknown, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* stub_message)
{
    s_network_message const* message = (s_network_message const*)(module_base + 0x4FFB090);
    message_handler->handle_channel_message(channel, message_type, message_storage_size, message);
}

// create online squad when calling hf2p_setup_session
bool __fastcall create_local_online_squad(e_network_session_class ignore) // may have thiscall for life cycle
{
    return network_life_cycle_create_local_squad(_network_session_class_online);
}

// print to console whenever a message packet is sent
void __stdcall send_message_hook(void* stream, e_network_message_type message_type, long message_storage_size)
{
    void(__stdcall * send_message)(void* stream, e_network_message_type message_type, long message_storage_size) = reinterpret_cast<decltype(send_message)>(module_base + 0x387A0);

    printf("SEND: %s\n", network_session->m_message_gateway->m_message_type_collection->get_message_type_name(message_type));
    return send_message(stream, message_type, message_storage_size);
}

// add back missing host code, process_pending_joins & check_to_send_membership_update
void __fastcall session_idle_hook(c_network_session* session)
{
    session->idle();
}

// I couldn't directly hook peer_request_properties_update without experiencing access violations, so this will do
// add back set_peer_address & set_peer_properties to peer_request_properties_update
void __fastcall network_session_update_peer_properties_hook(c_network_session* session, s_network_session_peer* peer)
{
    network_session_update_peer_properties(session, peer);
}

// add debug print back to before life cycle end is called in c_gui_location_manager::update
void __fastcall network_life_cycle_end_hook()
{
    printf("Resetting network location.  If you got here and didn't just issue a console command, this is a bug.\n");
    network_life_cycle_end();
}

// contrail gpu freeze fix
__declspec(naked) void contrail_fix_hook()
{
    __asm
    {
        add edx, [0x0068A38A]
        cmp edx, -1
        jg render
        push 0x68A3E3
        retn
        render :
        push 0x68A390
            retn
    }
}

long MainThread()
{
    AllocConsole();
    UnprotectMemory(module_base);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("Anvil Station Dedicated Server\n");
    printf("Build date: " __DATE__ " @ " __TIME__ "\n"); // TODO - list debug/release info w/ branch here
    printf("\n");

    //=== HOOKS ===//
    // add back missing host code by replacing existing stripped down functions
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    Hook(0x21AB0, session_idle_hook).Apply();
    Hook(0x2F48E, network_session_update_peer_properties_hook, HookFlags::IsCall).Apply();
    Hook(0x3EEE1F, network_life_cycle_end_hook, HookFlags::IsCall).Apply();
    // set network_life_cycle_create_local_squad call in hf2p_setup_session to create an online session
    Hook(0x3AAF68, create_local_online_squad, HookFlags::IsCall).Apply();
    // output the message type for debugging
    //Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    // contrail gpu freeze fix - twister
    Hook(0x28A38A, contrail_fix_hook).Apply();
    printf("Hooks applied\n");
    //=== ===== ===//

    //== PATCHES ==//
    // enable tag edits
    Patch(0x082DB4, { 0xEB }).Apply();
    Patch::NopFill(Pointer::Base(0x083120), 2);
    Patch::NopFill(Pointer::Base(0x083AFC), 2);
    // contrail gpu freeze fix - twister
    Patch(0x1D6B70, { 0xC3 }).Apply();
    printf("Patches applied\n");
    //== ======= ==//
    
    printf("\n");
    printf("Base address: %p\n", (void*)module_base);
    printf("Game port: %hi\n", *g_game_port);
    printf("\n");

    e_session_game_start_status previous_start_status = _start_status_none;
    e_session_game_start_error previous_start_error = _start_error_none;

    while (true)
    {
        // IF the life cycle session exists, print the it's status and error whenever they update
        if (network_session->current_local_state() != _network_session_state_none)
        {
            e_session_game_start_status start_status = network_session->get_session_parameters()->game_start_status.m_data.game_start_status;
            e_session_game_start_error start_error = network_session->get_session_parameters()->game_start_status.m_data.game_start_error;

            if (start_status != previous_start_status || start_error != previous_start_error)
                printf("start status updated: %s (error: %s)\n", multiplayer_game_start_status_to_string(start_status), multiplayer_game_start_error_to_string(start_error));

            previous_start_status = start_status;
            previous_start_error = start_error;
        }

        if (GetKeyState(VK_PRIOR) & 0x8000) // PAGE UP - create session
        {
            // SESSION DETAILS
            auto map_id = _riverworld;
            auto engine_variant = _engine_variant_ctf;
            auto multiplayer_mode = _desired_multiplayer_mode_custom_games; // I'm not sure if this actually matters anymore
            auto gui_gamemode = _gui_game_mode_multiplayer; // or this

            // CREATE SESSION
            std::cout << "Setting up session...\n";
            if (hf2p_setup_session())
                std::cout << "Session created!\n";
            else
                std::cout << "Session creation failed!\n";

            // SET START MODE
            user_interface_set_desired_multiplayer_mode(multiplayer_mode);
            std::cout << "Multiplayer mode set\n";

            // SET UI MODE
            if (c_network_session_parameter_ui_game_mode__request_change(&network_session->get_session_parameters()->ui_game_mode, gui_gamemode))
                std::cout << "UI game mode set\n";
            else
                std::cout << "Failed to set UI game mode!\n";

            // SET GAME VARIANT
            c_game_variant game_variant;
            game_variant_ctor(&game_variant);
            build_default_game_variant(&game_variant, engine_variant);
            if (!user_interface_squad_set_game_variant(&game_variant))
                std::cout << "Failed to set game variant!\n";
            else
                std::cout << "Game variant set\n";

            // SET MAP VARIANT
            c_map_variant* map_variant = new c_map_variant;
            c_map_variant_ctor(map_variant); // initialize map variant
            c_map_variant__create_default(map_variant, map_id); // create default s3d_turf map variant
            if (!user_interface_squad_set_multiplayer_map(map_variant))
                std::cout << "Failed to set map variant!\n";
            else
                std::cout << "Map variant set\n";

            //char* map_path = new char[MAX_PATH];
            //levels_get_path(-1, map_id, map_path, MAX_PATH); // get path to map on disk, eg maps\\s3d_turf
            //network_squad_session_set_map(-1, map_id, map_path); // set the map to use for the session
            //network_squad_session_set_map_variant(&map_variant[0]); // set the map variant to use for the session
        }
        else if (GetKeyState(VK_NEXT) & 0x8000) // PAGE DOWN - get session info
        {
            auto test = online_session_manager_globals;
            auto test2 = g_xnet_shim_table;

            // GET SECURE ID
            GUID server_secure_identifier;
            LPOLESTR secure_identifier_str;
            if (network_session->get_session_id((s_transport_secure_identifier*)&server_secure_identifier) && (StringFromCLSID(server_secure_identifier, &secure_identifier_str) == S_OK))
                printf("\nSecure id: %ls\n", secure_identifier_str);
            else
                printf("Failed to get secure identifier\n");
            // GET SECURE ADDRESS
            LPOLESTR secure_address_str;
            s_transport_secure_address* server_secure_address = (s_transport_secure_address*)(module_base + 0x4EBE9D2);
            if (/*transport_secure_address_get(server_secure_address) && */(StringFromCLSID(*(GUID*)server_secure_address, &secure_address_str) == S_OK))
                printf("Secure address: %ls\n\n", secure_address_str);
            else
                printf("Failed to get secure address\n\n");
        }
        else if (GetKeyState(VK_HOME) & 0x8000) // HOME - launch session
        {
            printf("Launching session...\n");
            c_network_session_parameter_session_mode__set(&network_session->m_session_parameters, _network_session_mode_setup);
        }
        else if (GetKeyState(VK_END) & 0x8000) // END - end game
        {
            printf("Ending game...\n");
            c_network_session_parameter_session_mode__set(&network_session->m_session_parameters, _network_session_mode_end_game);
        }
        else if (GetKeyState(VK_INSERT) & 0x8000) // INSERT - set host player data
        {
            printf("Setting host player data...\n");
            c_network_session* sessions = *network_session->m_session_manager->session;
            sessions[0].get_session_membership()->get_current_membership()->players[0].controller_index = 0; // set to -1 by default, which prevents the player from spawning
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.player_assigned_team = 1; // blue team
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.player_team = 1; // blue team
            wchar_t player_name[16] = L"player";
            memcpy(sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.player_name, player_name, 32);
            wchar_t service_tag[5] = L"TEST";
            memcpy(sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.player_appearance.service_tag, service_tag, 10);
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].armor = _armor_scanner;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].primary_weapon = _smg_v5;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].secondary_weapon = _magnum_v1;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[0] = _adrenaline;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[1] = _bomb_run;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[2] = _concussive_blast;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[3] = _hologram;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_plant_plasma_on_death] = 1;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_safety_booster] = 1;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_grenade_warning] = 1;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_customization.colours[0] = 1184274;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_customization.colours[1] = 1184274;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_customization.colours[2] = 1184274;
            sessions[0].get_session_membership()->get_current_membership()->players[0].configuration.host.s3d_player_customization.colours[3] = 1184274;
        }
        else if (GetKeyState(VK_DELETE) & 0x8000) // DEL - test add player (buggy)
        {
            printf("Adding a local test player...\n");
            c_network_session* sessions = *network_session->m_session_manager->session;
            sessions[0].get_session_membership()->get_current_membership()->player_count = 2;
            sessions[0].get_session_membership()->get_current_membership()->valid_player_mask = 3;
            sessions[0].get_session_membership()->get_current_membership()->peers[0].player_mask = 3;

            sessions[0].get_session_membership()->get_current_membership()->players[1].player_identifier.data = 84207682768246;
            sessions[0].get_session_membership()->get_current_membership()->players[1].peer_index = 0;
            sessions[0].get_session_membership()->get_current_membership()->players[1].controller_index = 0;
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.client.multiplayer_team = -1;
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.client.active_armor_loadout = -1;
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.player_xuid.data = 84207682768246;

            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.player_assigned_team = 0; // red team
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.player_team = 0; // red team
            wchar_t player2_name[16] = L"empty";
            memcpy(sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.player_name, player2_name, 32);
            wchar_t service_tag2[5] = L"TST2";
            memcpy(sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.player_appearance.service_tag, service_tag2, 10);
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.s3d_player_appearance.loadouts[0].armor = _armor_air_assault;
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.s3d_player_appearance.loadouts[0].primary_weapon = _assault_rifle;
            sessions[0].get_session_membership()->get_current_membership()->players[1].configuration.host.s3d_player_appearance.loadouts[0].secondary_weapon = _magnum;
        }
        Sleep(100);
    }

    // cleanup & eject
    fclose(f);
    FreeConsole();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH: // LoadLibrary
        SetProcessDPIAware(); // disable dpi scaling
        DisableThreadLibraryCalls(hModule); //Disables the DLL_THREAD_ATTACH and DLL_THREAD_DETACH (reduces the working set size)
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MainThread, NULL, 0, NULL);
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