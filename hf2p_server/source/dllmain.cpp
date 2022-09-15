#include "dllmain.h"
#include <iostream>
#include "patch\Patch.hpp"
#include "networking\session\network_session.h"
#include "game\game_engine.h"
#include "networking\messages\network_message_gateway.h"
#include "networking\logic\network_session_interface.h"
#include "simulation\simulation_world.h"
#include "networking\session\network_managed_session.h"
#include "networking\transport\transport_shim.h"
#include "networking\session\network_session_manager.h"
#include "networking\messages\network_message_handler.h"

void UnprotectMemory(dword base)
{
    // Enable write to all executable memory
    size_t Offset, Total;
    Offset = Total = 0;
    MEMORY_BASIC_INFORMATION MemInfo;

    printf("Unprotecting memory...\n");
    while (VirtualQuery((byte*)base + Offset, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
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
// these aren't original names
static bool* g_netdebug_enabled = (bool*)(module_base + 0x103E768); // enables netdebug scoreboard widget
static bool* g_netdebug_local = (bool*)(module_base + 0x1038283); // disables debug statistics when true (and sets local text widget to display in green)

// FUNCTIONS - TODO: sort these into their proper file locations
char(__thiscall* network_life_cycle_create_local_squad)(e_network_session_class session_class) = reinterpret_cast<decltype(network_life_cycle_create_local_squad)>(module_base + 0x2AD00);
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

bool game_is_dedicated_server()
{
    return false;
}

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

    if (network_session->m_message_gateway != nullptr)
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

// allow view establishment to progress past the connection phase
void __fastcall update_establishing_view_hook(c_simulation_world* simulation_world, void* unknown, c_simulation_view* simulation_view)
{
    simulation_world->update_establishing_view(simulation_view);
}

// hook to prevent the game from adding a player to the host if we're running a dedicated server
void __fastcall peer_request_player_add_hook(c_network_session* session, void* unknown, const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings)
{
    if (!game_is_dedicated_server())
        session->peer_request_player_add(player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}
// ditto above
bool __fastcall network_session_interface_get_local_user_identifier_hook(s_player_identifier* player_identifier)
{
    bool(__thiscall * network_session_interface_get_local_user_identifier)(s_player_identifier * player_identifier) = reinterpret_cast<decltype(network_session_interface_get_local_user_identifier)>(module_base + 0x3D50);
    return !game_is_dedicated_server() && network_session_interface_get_local_user_identifier(player_identifier);
}

// reimplement network_session_check_properties by calling it at the end of network_session_interface_update_session
void __fastcall network_session_interface_update_session_hook(c_network_session* session)
{
    void(__thiscall * network_session_interface_update_session)(c_network_session* session) = reinterpret_cast<decltype(network_session_interface_update_session)>(module_base + 0x2F410);
    network_session_interface_update_session(session);

    if (session->established() && !session->leaving_session())
    {
        if (session->is_host())
        {
            network_session_check_properties(session);
        }
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
    Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    // contrail gpu freeze fix - twister
    Hook(0x28A38A, contrail_fix_hook).Apply();
    // allow view_establishment to progress past connection phase to established in update_establishing_view again
    Hook(0x370E0, update_establishing_view_hook).Apply();
    // prevent the game from adding a player to the dedicated host
    Hook(0x2F5AC, peer_request_player_add_hook, HookFlags::IsCall).Apply();
    Hook(0x212CC, network_session_interface_get_local_user_identifier_hook, HookFlags::IsCall).Apply();
    // add back network_session_check_properties
    //Hook(0x2AD9E, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    //Hook(0x2DC71, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    
    // TODO: hook hf2p_tick and disable everything but the heartbeat service, and reimplement whatever ms23 was doing

    printf("Hooks applied\n");
    //=== ===== ===//

    //== PATCHES ==//
    // enable tag edits
    Patch(0x082DB4, { 0xEB }).Apply();
    Patch::NopFill(Pointer::Base(0x083120), 2);
    Patch::NopFill(Pointer::Base(0x083AFC), 2);
    // contrail gpu freeze fix - twister
    Patch(0x1D6B70, { 0xC3 }).Apply();
    // enable netdebug
    *g_netdebug_enabled = true;
    *g_netdebug_local = false;
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
            auto map_id = _zanzibar;
            auto engine_variant = _engine_variant_slayer;
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
            c_game_variant* game_variant = new c_game_variant();
            build_default_game_variant(game_variant, engine_variant);
            game_variant->m_storage.m_base_variant.m_miscellaneous_options.m_number_of_rounds = 1;
            //game_variant->m_storage.m_slayer_variant.m_score_to_win = 1;
            if (!user_interface_squad_set_game_variant(game_variant))
                std::cout << "Failed to set game variant!\n";
            else
                std::cout << "Game variant set\n";

            // SET MAP VARIANT
            c_map_variant* map_variant = new c_map_variant();
            c_map_variant_ctor(map_variant); // initialize map variant
            c_map_variant__create_default(map_variant, map_id); // create default s3d_turf map variant
            if (!user_interface_squad_set_multiplayer_map(map_variant))
                std::cout << "Failed to set map variant!\n";
            else
                std::cout << "Map variant set\n";
            delete map_variant;
            delete game_variant;
            
            //char* map_path = new char[MAX_PATH];
            //levels_get_path(-1, map_id, map_path, MAX_PATH); // get path to map on disk, eg maps\\s3d_turf
            //network_squad_session_set_map(-1, map_id, map_path); // set the map to use for the session
            //network_squad_session_set_map_variant(&map_variant[0]); // set the map variant to use for the session
        }
        else if (GetKeyState(VK_NEXT) & 0x8000) // PAGE DOWN - get session info
        {
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
        else if (GetKeyState(VK_INSERT) & 0x8000) // INSERT - set player data
        {
            printf("Setting test player data...\n");
            c_network_session* sessions = *network_session->m_session_manager->session;
            s_network_session_shared_membership* membership = sessions[0].get_session_membership()->get_current_membership();

            // host player
            wchar_t host_name[16] = L"host";
            memcpy(membership->players[0].configuration.host.player_name, host_name, 32);
            wchar_t service_tag1[5] = L"TEST";
            memcpy(membership->players[0].configuration.host.player_appearance.service_tag, service_tag1, 10);
            membership->players[0].configuration.host.player_assigned_team = 0; // red team
            membership->players[0].configuration.host.player_team = 0; // red team
            membership->players[0].configuration.host.s3d_player_customization.colors[_primary] = 0x0F0F0F;
            membership->players[0].configuration.host.s3d_player_customization.colors[_secondary] = 0x003750;
            membership->players[0].configuration.host.s3d_player_customization.colors[_visor] = 0xFF640A;
            membership->players[0].configuration.host.s3d_player_customization.colors[_lights] = 0xFF640A;
            membership->players[0].configuration.host.s3d_player_customization.colors[_holo] = 0xFF640A;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].armor = _armor_pilot;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].primary_weapon = _smg_v5;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].secondary_weapon = _magnum_v1;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[0] = _adrenaline;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[1] = _bomb_run;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[2] = _concussive_blast;
            membership->players[0].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[3] = _hologram;
            membership->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_plant_plasma_on_death] = 1;
            membership->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_safety_booster] = 1;
            membership->players[0].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_grenade_warning] = 1;
            membership->players[0].controller_index = 0;

            // client player
            memcpy(membership->players[1].configuration.host.player_appearance.service_tag, service_tag1, 10);
            membership->players[1].configuration.host.player_assigned_team = 1; // blue team
            membership->players[1].configuration.host.player_team = 1; // blue team
            membership->players[1].configuration.host.s3d_player_customization.colors[_primary] = 0x0F0F0F;
            membership->players[1].configuration.host.s3d_player_customization.colors[_secondary] = 0x003750;
            membership->players[1].configuration.host.s3d_player_customization.colors[_visor] = 0xFF640A;
            membership->players[1].configuration.host.s3d_player_customization.colors[_lights] = 0xFF640A;
            membership->players[1].configuration.host.s3d_player_customization.colors[_holo] = 0xFF640A;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].armor = _armor_pilot;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].primary_weapon = _smg_v5;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].secondary_weapon = _magnum_v1;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[0] = _adrenaline;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[1] = _bomb_run;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[2] = _concussive_blast;
            membership->players[1].configuration.host.s3d_player_appearance.loadouts[0].tactical_packs[3] = _hologram;
            membership->players[1].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_plant_plasma_on_death] = 1;
            membership->players[1].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_safety_booster] = 1;
            membership->players[1].configuration.host.s3d_player_appearance.modifiers[0].modifier_values[_grenade_warning] = 1;

            // push update
            sessions[0].get_session_membership()->increment_update();
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