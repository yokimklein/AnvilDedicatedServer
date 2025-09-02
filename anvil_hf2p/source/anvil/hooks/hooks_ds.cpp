#include "hooks_ds.h"
#include <anvil\hooks\hooks.h>
#include <anvil\backend\services\user_storage_service.h>
#include <anvil\session_control.h>
#include <networking\session\network_session_parameters_game.h>
#include <networking\transport\transport_security.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_session_interface.h>
#include <game\game.h>
#include <networking\network_globals.h>
#include <scenario\scenario.h>
#include <anvil\backend\cache.h>
#include <networking\session\network_managed_session.h>
#include <networking\logic\life_cycle\life_cycle_handler_end_game_write_stats.h>

bool const k_add_local_player_in_dedicated_server_mode = false;

// runtime checks need to be disabled for these, make sure to write them within the pragmas
// also don't use the standard library in inserted hooks, it can mess up the function prologue and break variable access
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
        session->get_session_parameters()->m_parameters.dedicated_server_session_state.set(&dedi_state);
    }
}

__declspec(safebuffers) void __cdecl c_life_cycle_state_handler_in_game__exit_hook()
{
    c_network_session* session = nullptr;
    if (network_session_interface_get_squad_session(&session) && game_is_dedicated_server())
    {
        e_dedicated_server_session_state dedi_state = _dedicated_server_session_state_waiting_for_players;
        session->get_session_parameters()->m_parameters.dedicated_server_session_state.set(&dedi_state);
    }
}

// request all player containers for all players in session in case loadouts have been updated since joining & prior to game starting
__declspec(safebuffers) void __cdecl c_life_cycle_state_handler_start_game__enter_hook()
{
    c_network_session* session;
    __asm
    {
        mov session, eax
    }

    if (!session->is_host())
    {
        return;
    }

    const c_network_session_membership* membership = session->get_session_membership();
    qword user_ids[k_network_maximum_players_per_session];

    for (long player_index = membership->get_first_player(); player_index != NONE; player_index = membership->get_next_player(player_index))
    {
        user_ids[player_index] = membership->get_player(player_index)->configuration.host.user_xuid;
    }
    
    for (long container_index = 0; container_index < k_user_storage_container_count; container_index++)
    {
        c_backend::user_storage_service::get_public_data::request(user_ids, membership->get_player_count(), (e_user_storage_container)container_index);
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
    if (game_is_dedicated_server() && !k_add_local_player_in_dedicated_server_mode)
    {
        return;
    }
    session->peer_request_player_add(player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}
bool __fastcall network_session_interface_get_local_user_identifier_hook(s_player_identifier* player_identifier)
{
    return (!game_is_dedicated_server() || k_add_local_player_in_dedicated_server_mode) && network_session_interface_get_local_user_identifier(player_identifier);
}

// Set the xp rewards + consumable costs & reset wp event tracker on scenario load
__declspec(safebuffers) void __fastcall anvil_scenario_tags_load_title_instances()
{
    if (game_is_dedicated_server())
    {
        c_backend_data_cache::refresh_consumable_costs();
        // wp events are only used in multiplayer/survival
        // $TODO: game globals aren't initialised by this point, can't use these checks
        //if (game_is_multiplayer() || game_is_survival())
        {
            c_backend_data_cache::refresh_scoring_events();
            c_backend_data_cache::reset_earned_wp_events();
        }
    }
}

// Hook usercall
__declspec(naked) void remove_from_player_list_hook(s_online_session_player* players, long unused, qword const* xuids, long xuid_count)
{
    __asm
    {
        mov edx, k_network_maximum_players_per_session // add back missing parameter
        jmp remove_from_player_list
    }
}

// Replace get from saber's backend TI cache with a function call which gets our own
// used for updating the equipment costs on the HUD
__declspec(naked) void chud_update_user_data_hook()
{
    __asm
    {
        mov ecx, [ebp-0x0C] // player datum
        mov edx, esi // consumable_slot
        call player_get_consumable_cost
        // set return to edx where original code expects it to be
        mov edx, eax
        ret
    }
}

void __fastcall c_life_cycle_state_handler_end_game_write_stats__update_hook(c_life_cycle_state_handler_end_game_write_stats* thisptr)
{
    thisptr->update_();
}

void anvil_hooks_ds_apply()
{
    // add anvil_session_update to network_update after network_session_interface_update
    hook::insert(0x24601, 0x24606, anvil_session_update_hook, _hook_execute_replaced_first);
    
    // hook c_network_session_parameter_game_start_status::set calls to log when the session start status & start error are updated
    hook::function(0x3BA00, 0x6C, c_network_session_parameter_game_start_status__set_hook);
    hook::insert(0x4DF34, 0x4DF99, c_life_cycle_state_handler_pre_game__squad_game_start_status_update_hook, _hook_replace);

    // hook XNetCreateKey() to use a lobby/party ID from the API when running as a dedicated server
    hook::function(0x3BC0, 0x79, transport_secure_key_create);

    // hook transport_secure_address_resolve to get secure address from API when running as a dedicated server
    hook::function(0x3C50, 0xFB, transport_secure_address_resolve);

    // prevent the game from adding a player to the dedicated host
    hook::call(0x2F5AC, peer_request_player_add_hook);
    hook::call(0x212CC, network_session_interface_get_local_user_identifier_hook);

    // set dedicated server session state to in game when c_life_cycle_state_handler_in_game::enter is called
    hook::insert(0x4EAE9, 0x4EAF3, c_life_cycle_state_handler_in_game__enter_hook, _hook_execute_replaced_first);
    // set dedicated server session state back to waiting for players when c_life_cycle_state_handler_in_game::exit is called
    hook::insert(0x4EB7B, 0x4EB82, c_life_cycle_state_handler_in_game__exit_hook, _hook_execute_replaced_first);

    // TODO: hook main_loading_initialize & main_game_load_map to disable load progress when running as a dedicated server (check if this is the same progress used for voting)
    // TODO: disable sound & rendering system when running as a dedicated server - optionally allow playing as host & spectate fly cam

    // disable saber's backend, we're using our own now
    // replace inlined hf2p_scenario_tags_load_finished in scenario_load with our own function to set xp event rewards & consumable costs
    // remove call to hf2p_initialize in scenario_load
    hook::insert(0x7E978, 0x7E9AD, anvil_scenario_tags_load_title_instances, _hook_replace);

    // remove 13 hf2p service update calls in hf2p_game_update
    patch::nop_region(0x2B0C51, 65);

    // remove call to game_shield_initialize in hf2p_security_initialize - this will crash unless hf2p_initialize is disabled
    // this prevents the game from exiting when no username and signincode launch args are supplied
    patch::nop_region(0x2B0226, 5);
    // $TODO: replace above with removing hf2p_game_initialize?

    // remove call to hf2p_client_dispose in game_dispose
    patch::nop_region(0x95D9F, 5);

    // remove call to heartbeat update in main_loop_pregame
    patch::nop_region(0x96067, 5);

    // hook remove_from_player_list to remove leaving players from the public data cache
    hook::call(0x286F4, remove_from_player_list_hook);
    hook::call(0x29567, remove_from_player_list_hook);
    hook::call(0x30EE3, remove_from_player_list_hook);
    hook::call(0x31AA9, remove_from_player_list_hook);
    // fix stack for remove_from_player_list hook
    patch::nop_region(0x286F9, 3);
    patch::nop_region(0x2956C, 3);
    patch::nop_region(0x30EFB, 3);
    patch::nop_region(0x31AB7, 3);

    // Request public data on game start (hook end of c_life_cycle_state_handler_start_game::enter)
    hook::insert(0x4C40F, 0x4C415, c_life_cycle_state_handler_start_game__enter_hook);

    // Replace saber's backend for getting consumable TI data
    hook::insert(0x3AF851, 0x3AF857, chud_update_user_data_hook, _hook_replace_no_preserve);
    hook::function(0x42D290, 0x169, unit_handle_equipment_energy_cost);
    hook::function(0xBF840, 0x62, player_can_use_consumable);

    // Restore end game write stats to submit game stats to the API
    hook::function(0x4C630, 0x218, c_life_cycle_state_handler_end_game_write_stats__update_hook);
}