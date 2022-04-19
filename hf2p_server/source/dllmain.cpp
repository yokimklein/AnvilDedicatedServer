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

// INIT
static c_network_session* network_session = (c_network_session*)(module_base + 0x3970168);
static short* g_game_port = (short*)(module_base + 0xE9B7A0);

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

bool __fastcall create_local_online_squad(e_network_session_class ignore) // may have thiscall for life cycle
{
    return network_life_cycle_create_local_squad(_network_session_class_online);
}

void __stdcall send_message_hook(void* stream, e_network_message_type message_type, long unknown)
{
    void(__stdcall * send_message)(void* stream, e_network_message_type message_type, long unknown) = reinterpret_cast<decltype(send_message)>(module_base + 0x387A0);

    printf("SEND: %s\n", network_session->m_message_gateway->m_message_type_collection->get_message_type_name(message_type));
    return send_message(stream, message_type, unknown);
}

long MainThread()
{
    AllocConsole();
    UnprotectMemory(module_base);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("Anvil Station Dedicated Server\n");
    printf("Build date: " __DATE__ " @ " __TIME__ "\n");
    printf("\n");

    //=== HOOKS ===//
    // replace existing functions with missing code
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    // set network_life_cycle_create_local_squad call in hf2p_setup_session to create an online session
    Hook(0x3AAF68, create_local_online_squad, HookFlags::IsCall).Apply();
    // output the message type for debugging
    Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    printf("Hooks applied\n");
    //=== ===== ===//

    //== PATCHES ==//
    printf("Patches applied\n");
    //== ======= ==//
    
    printf("\n");
    printf("Base address: %p\n", (void*)module_base);
    printf("c_network_session address: %p\n", network_session);
    printf("Game port: %hi\n", *g_game_port);
    printf("\n");

    while (true)
    {
        if (GetKeyState(VK_PRIOR) & 0x8000) // PAGE UP - create session
        {
            // SESSION DETAILS
            auto map_id = _s3d_turf;
            auto engine_variant = _engine_variant_slayer;

            // CREATE SESSION
            std::cout << "Setting up session...\n";
            if (hf2p_setup_session())
                std::cout << "Session created!\n";
            else
                std::cout << "Session creation failed!\n";

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
        else if (GetKeyState(VK_END) & 0x8000) // END - get session info
        {
            auto test = online_session_manager_globals;
            auto test2 = g_xnet_shim_table;

            // GET SECURE ID
            GUID server_secure_identifier;
            LPOLESTR secure_identifier_str;
            if (managed_session_get_id(network_session->m_managed_session_index, (s_transport_secure_identifier*)&server_secure_identifier) && (StringFromCLSID(server_secure_identifier, &secure_identifier_str) == S_OK))
                printf("\nSecure id: %ls\n", secure_identifier_str);
            else
                printf("Failed to get secure identifier\n");
            // GET SECURE ADDRESS
            GUID server_secure_address;
            LPOLESTR secure_address_str;
            if (transport_secure_address_get(&server_secure_address) && (StringFromCLSID(server_secure_address, &secure_address_str) == S_OK))
                printf("Secure address: %ls\n\n", secure_address_str);
            else
                printf("Failed to get secure address\n\n");
        }
        else if (GetKeyState(VK_NEXT) & 0x8000) // PAGE DOWN - launch session
        {
            printf("Launching session...\n");
            c_network_session_parameter_session_mode__set(&network_session->m_session_parameters, _network_session_mode_setup);
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