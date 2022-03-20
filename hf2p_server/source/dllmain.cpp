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

int MainThread()
{
    AllocConsole();
    UnprotectMemory(module_base);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("Anvil Station Dedicated Server\n");
    printf("Build date: " __DATE__ " @ " __TIME__ "\n");
    printf("Base address: %p\n", (void*)module_base);

    // INIT
    typedef int(__cdecl* _network_life_cycle_end_ptr)();
    auto network_life_cycle_end = reinterpret_cast<_network_life_cycle_end_ptr>(module_base + 0x311720);

    typedef char(__thiscall* network_life_cycle_create_local_squad_ptr)(e_network_session_class session_class); // 0 = offline, 1 = online, 2 = xbl
    auto network_life_cycle_create_local_squad = reinterpret_cast<network_life_cycle_create_local_squad_ptr>(module_base + 0x2AD00);

    typedef char(__thiscall* user_interface_squad_set_ui_game_mode_ptr)(int* this_ptr, e_gui_game_mode start_mode); // 0 = campaign, 1 = matchmaking, 2 = customs, 3 = forge, 4 = theater, 5 = firefight
    auto user_interface_squad_set_ui_game_mode = reinterpret_cast<user_interface_squad_set_ui_game_mode_ptr>(module_base + 0xC00 + 0x3A8D0);

    typedef char(__thiscall* user_interface_set_desired_multiplayer_mode_ptr)(e_desired_multiplayer_mode desired_multiplayer_mode);
    auto user_interface_set_desired_multiplayer_mode = reinterpret_cast<user_interface_set_desired_multiplayer_mode_ptr>(module_base + 0xC00 + 0x3A9BD0);

    typedef char(__cdecl* c_game_variant__get_ptr)(c_game_variant* output); // ms23 version took in e_engine_variant & engine_subtype_index
    // TODO: c_game_variant::get(void *this, int output_variant) - actual args? where this is e_engine_variant - alternatively rewrite function from scratch
    auto c_game_variant__get = reinterpret_cast<c_game_variant__get_ptr>(module_base + 0xC00 + 0xE9210); // function doesn't exist in the h3 debug build so this name is made up

    typedef bool(__thiscall* user_interface_squad_set_game_variant_ptr)(c_game_variant* game_variant);
    auto user_interface_squad_set_game_variant = reinterpret_cast<user_interface_squad_set_game_variant_ptr>(module_base + 0x3ABEC0);

    typedef bool(__thiscall* network_squad_session_set_game_variant_ptr)(c_game_variant* game_variant);
    auto network_squad_session_set_game_variant = reinterpret_cast<network_squad_session_set_game_variant_ptr>(module_base + 0xC00 + 0x2D510);

    typedef void*(__thiscall* c_map_variant__c_map_variant_ptr)(c_map_variant* map_variant);
    auto c_map_variant__c_map_variant = reinterpret_cast<c_map_variant__c_map_variant_ptr>(module_base + 0xC00 + 0xAA6F0);

    typedef char(__thiscall* c_map_variant__create_default_ptr)(c_map_variant* map_variant, int map_id);
    auto c_map_variant__create_default = reinterpret_cast<c_map_variant__create_default_ptr>(module_base + 0xC00 + 0xAA780); // c_map_variant_initialize

    typedef char(__thiscall* user_interface_squad_set_multiplayer_map_ptr)(c_map_variant* map_variant);
    auto user_interface_squad_set_multiplayer_map = reinterpret_cast<user_interface_squad_set_multiplayer_map_ptr>(module_base + 0xC00 + 0x3AB060);

    //typedef char(__cdecl* levels_get_path_ptr)(int campaign_id, int map_id, char* output, rsize_t size_in_bytes);
    //auto levels_get_path = reinterpret_cast<levels_get_path_ptr>(0x68D7C0);
    //
    //typedef bool(__cdecl* network_squad_session_set_map_ptr)(int campaign_id, int map_id, char* map_path);
    //auto network_squad_session_set_map = reinterpret_cast<network_squad_session_set_map_ptr>(0x5DDEC0);
    //
    //typedef char(__cdecl* network_squad_session_set_map_variant_ptr)(void* map_variant);
    //auto network_squad_session_set_map_variant = reinterpret_cast<network_squad_session_set_map_variant_ptr>(0x95BC60);

    typedef char(__thiscall* c_network_session_parameter_session_mode__set_ptr)(c_network_session_parameters* session_parameters, e_network_session_mode session_mode);
    auto c_network_session_parameter_session_mode__set = reinterpret_cast<c_network_session_parameter_session_mode__set_ptr>(module_base + 0xC00 + 0x2CC20);

    typedef char(__fastcall* managed_session_get_id_ptr)(int index, GUID* transport_secure_identifier);
    auto managed_session_get_id = reinterpret_cast<managed_session_get_id_ptr>(module_base + 0x28AE0);

    c_network_session* network_session = (c_network_session*)(module_base + 0xC00 + 0x396F568);
    c_network_session_manager* session_manager = network_session->m_session_manager;

    int* life_cycle_state_unknown = (int*)((int)network_session + 0xE1D68);
    int c_life_cycle_state_handler = (module_base + 0x3EADFAC); // 0x3EAE010[2]
    short* g_game_port = (short*)(module_base + 0xE9B7A0);
    c_network_session_parameters* network_session_parameters = &network_session->m_session_parameters; //  (c_network_session_parameters*)((int)network_session + 0xE1C90); // c_life_cycle_state + 0xE1C90

    // HOOKS
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    printf("Hooks applied\n");
    // PATCHES
    //const char* title_str = "Anvil Dedicated Server %s";
    //Pointer::Base(0xD699D8).Write(title_str);
    printf("Patches applied\n");

    printf("Game port: %hi\n", *g_game_port);
    printf("c_network_session address: %p\n\n", network_session);

    while (true)
    {
        if (GetKeyState(VK_PRIOR) & 0x8000) // PAGE UP
        {
            int map_id = 31; // s3d_turf map id

            network_life_cycle_end();
            std::cout << "Destroying current session...\n";
            network_life_cycle_create_local_squad(_network_session_class_online); // calling this should set m_local_state to 6 - why doesn't it?
            std::cout << "Creating an online syslink session\n";
            
            user_interface_squad_set_ui_game_mode(life_cycle_state_unknown, _gui_game_mode_multiplayer);
            std::cout << "UI gamemode set to customs\n";

            user_interface_set_desired_multiplayer_mode(_desired_multiplayer_mode_custom_games);
            std::cout << "Multiplayer mode set to custom-games\n";

            // TODO: fix map & game variant code
            c_game_variant* game_variant = new c_game_variant;
            c_game_variant__get(game_variant); // subtype index is specified by the wezr tag? function currently broken
            std::cout << "Getting game variant\n";
            if (!user_interface_squad_set_game_variant(game_variant))
                std::cout << "Failed to set game variant!\n";
            else
                std::cout << "Game variant set\n";

            // TODO: map variant isn't loading correctly
            char* map_path = new char[MAX_PATH];
            c_map_variant* map_variant = new c_map_variant;
            c_map_variant__c_map_variant(map_variant); // initialize map variant
            std::cout << "Initializing map variant\n";
            c_map_variant__create_default(map_variant, map_id); // create default s3d_turf map variant
            std::cout << "Creating default map variant\n";
            if (!user_interface_squad_set_multiplayer_map(map_variant))
                std::cout << "Failed to set map variant!\n";
            else
                std::cout << "Map variant set\n";
            //levels_get_path(-1, map_id, map_path, MAX_PATH); // get path to map on disk, eg maps\\s3d_turf
            //network_squad_session_set_map(-1, map_id, map_path); // set the map to use for the session
            //network_squad_session_set_map_variant(&map_variant[0]); // set the map variant to use for the session

            GUID server_secure_identifier;
            LPOLESTR secure_identifier_str;
            if (managed_session_get_id(network_session->m_session_index, &server_secure_identifier) && (StringFromCLSID(server_secure_identifier, &secure_identifier_str) == S_OK)) // *(((int*)c_life_cycle_state_handler + 36) + 0x1AC050)
                printf("\nSecure id: %ls\n", secure_identifier_str);
            else
                printf("Failed to get secure identifier\n");

            GUID server_secure_address;
            LPOLESTR secure_address_str;
            if (transport_secure_address_get(&server_secure_address) && (StringFromCLSID(server_secure_address, &secure_address_str) == S_OK))
                printf("Secure address: %ls\n\n", secure_address_str);
            else
                printf("Failed to get secure address\n\n");
        }
        else if (GetKeyState(VK_NEXT) & 0x8000) // PAGE DOWN
        {
            printf("Launching session...\n");
            c_network_session_parameter_session_mode__set(network_session_parameters, _network_session_mode_setup);
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