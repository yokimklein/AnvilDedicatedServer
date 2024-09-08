#include "hooks_ds.h"
#include <anvil\hooks\hooks.h>
#include <Patch.hpp>
#include <anvil\server_tools.h>
#include <networking\session\network_session_parameters_game.h>
#include <networking\transport\transport_security.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_session_interface.h>
#include <game\game.h>
#include <networking\network_globals.h>

bool const k_dedicated_has_player = false;

// runtime checks need to be disabled for these, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)
__declspec(safebuffers) void __cdecl anvil_session_update_hook()
{
    anvil_session_update();
}

__declspec(safebuffers) void __fastcall c_life_cycle_state_handler_pre_game__squad_game_start_status_update_hook()
{
    c_network_session_parameter_game_start_status* parameter;
    s_network_session_parameter_game_start_status* start_status;
    DEFINE_ORIGINAL_EBP_ESP(0x39C, sizeof(parameter) + sizeof(start_status));
    
    __asm mov parameter, ecx;
    __asm mov eax, original_esp;
    __asm lea eax, [eax + 0x398 - 0x388];
    __asm mov start_status, eax;
    
    parameter->set(start_status);
}

__declspec(safebuffers) void __cdecl c_life_cycle_state_handler_in_game__enter_hook()
{
    c_network_session* session = nullptr;
    if (network_session_interface_get_squad_session(&session) && game_is_dedicated_server())
    {
        e_dedicated_server_session_state dedi_state = _dedicated_server_session_state_in_game;
        session->get_session_parameters()->dedicated_server_session_state.set(&dedi_state);
    }
}

__declspec(safebuffers) void __cdecl c_life_cycle_state_handler_in_game__exit_hook()
{
    c_network_session* session = nullptr;
    if (network_session_interface_get_squad_session(&session) && game_is_dedicated_server())
    {
        e_dedicated_server_session_state dedi_state = _dedicated_server_session_state_waiting_for_players;
        session->get_session_parameters()->dedicated_server_session_state.set(&dedi_state);
    }
}
#pragma runtime_checks("", restore)

bool __fastcall c_network_session_parameter_game_start_status__set_hook(c_network_session_parameter_game_start_status* thisptr, void* unused, s_network_session_parameter_game_start_status* start_status)
{
    return thisptr->set(start_status);
}

// hooks to prevent the game from adding a player to the host if we're running a dedicated server
void __fastcall peer_request_player_add_hook(c_network_session* session, void* unused, const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings)
{
    if (game_is_dedicated_server() && !k_dedicated_has_player)
        return;
    session->peer_request_player_add(player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}
bool __fastcall network_session_interface_get_local_user_identifier_hook(s_player_identifier* player_identifier)
{
    return (!game_is_dedicated_server() || k_dedicated_has_player) && network_session_interface_get_local_user_identifier(player_identifier);
}

void anvil_hooks_ds_apply()
{
    // add anvil_session_update to network_update after network_session_interface_update
    insert_hook(0x24601, 0x24606, anvil_session_update_hook, _hook_execute_replaced_first);
    
    // hook c_network_session_parameter_game_start_status::set calls to log when the session start status & start error are updated
    Hook(0x3BA00, c_network_session_parameter_game_start_status__set_hook).Apply();
    insert_hook(0x4DF34, 0x4DF99, c_life_cycle_state_handler_pre_game__squad_game_start_status_update_hook, _hook_replace);

    // hook XNetCreateKey() to use a lobby/party ID from the API when running as a dedicated server
    Hook(0x3BC0, transport_secure_key_create).Apply();

    // hook transport_secure_address_resolve to get secure address from API when running as a dedicated server
    Hook(0x3C50, transport_secure_address_resolve).Apply();

    // prevent the game from adding a player to the dedicated host
    Hook(0x2F5AC, peer_request_player_add_hook, HookFlags::IsCall).Apply();
    Hook(0x212CC, network_session_interface_get_local_user_identifier_hook, HookFlags::IsCall).Apply();

    // set dedicated server session state to in game when c_life_cycle_state_handler_in_game::enter is called
    insert_hook(0x4EAE9, 0x4EAF3, c_life_cycle_state_handler_in_game__enter_hook, _hook_execute_replaced_first);
    // set dedicated server session state back to waiting for players when c_life_cycle_state_handler_in_game::exit is called
    insert_hook(0x4EB7B, 0x4EB82, c_life_cycle_state_handler_in_game__exit_hook, _hook_execute_replaced_first);

    // TODO: hook hf2p_tick and disable everything but the heartbeat service, and reimplement whatever ms23 was doing, do the same for game_startup_internal & game_shutdown_internal
    // TODO: hook network_session_interface_get_local_user_identifier in c_network_session::create_host_session to add back !game_is_dedicated_server() check
    // TODO: set wp event xp rewards at runtime when retrieving title instances from the API - right now we're just doing this in tags
    // TODO: hook main_loading_initialize & main_game_load_map to disable load progress when running as a dedicated server (check if this is the same progress used for voting)
    // TODO: disable sound & rendering system when running as a dedicated server - optionally allow playing as host & spectate fly cam
}