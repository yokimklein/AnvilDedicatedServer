#include "hooks.h"
#include "patch\Patch.hpp"

#include "..\networking\session\network_session.h"
#include "..\networking\messages\network_message_handler.h"
#include "..\simulation\simulation_world.h"
#include "..\networking\messages\network_message_gateway.h"
#include "..\networking\logic\network_session_interface.h"
#include "..\networking\network_globals.h"
#include "backend.h"
#include "..\networking\logic\network_life_cycle.h"
#include "..\networking\transport\transport_shim.h"
#include "..\networking\session\network_managed_session.h"
#include "server_tools.h"
#include "..\simulation\simulation_debug_globals.h"
#include "..\simulation\game_interface\simulation_game_action.h"
#include "..\memory\tls.h"
#include "..\simulation\game_interface\simulation_game_objects.h"
#include "build_version.h"
#include "..\game\game_results.h"

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* stub_message)
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
    FUNCTION_DEF(0x387A0, void, __stdcall, send_message, void* stream, e_network_message_type message_type, long message_storage_size);
    c_network_message_gateway* message_gateway = nullptr;
    c_network_session* session = g_network_session_manager->session[0];
    if (session != nullptr)
        message_gateway = session->m_message_gateway;

    if (message_gateway != nullptr)
        printf("SEND: %s\n", message_gateway->m_message_type_collection->get_message_type_name(message_type));
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
void __fastcall update_establishing_view_hook(c_simulation_world* simulation_world, void* unused, c_simulation_view* simulation_view)
{
    simulation_world->update_establishing_view(simulation_view);
}

// hook to prevent the game from adding a player to the host if we're running a dedicated server
void __fastcall peer_request_player_add_hook(c_network_session* session, void* unused, const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings)
{
    if (!game_is_dedicated_server())
        session->peer_request_player_add(player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}
// ditto above
bool __fastcall network_session_interface_get_local_user_identifier_hook(s_player_identifier* player_identifier)
{
    bool(__thiscall* network_session_interface_get_local_user_identifier)(s_player_identifier* player_identifier) = reinterpret_cast<decltype(network_session_interface_get_local_user_identifier)>(module_base + 0x3D50);
    return !game_is_dedicated_server() && network_session_interface_get_local_user_identifier(player_identifier);
}

// reimplement network_session_check_properties by calling it at the end of network_session_interface_update_session
void __fastcall network_session_interface_update_session_hook(c_network_session* session)
{
    void(__thiscall* network_session_interface_update_session)(c_network_session* session) = reinterpret_cast<decltype(network_session_interface_update_session)>(module_base + 0x2F410);
    network_session_interface_update_session(session);

    if (session->established() && !session->leaving_session())
    {
        if (session->is_host())
        {
            network_session_check_properties(session);
        }
    }
}

int __cdecl vsnprintf_s_net_debug_hook(char* DstBuf, size_t SizeInBytes, size_t MaxCount, const char* Format, va_list ArgList)
{
    // original function call
    int result = vsnprintf_s(DstBuf, SizeInBytes, MaxCount, Format, ArgList);

    // check if we're building a URI - we don't want to print these
    if (strcmp(Format, "/%s.svc/%s") != 0)
    {
        printf("[+] %s \n", DstBuf);
    }
    // parse obfuscated URIs - temporarily disabled
    //else if (strcmp(Format, "Request %s") == 0 || strcmp(Format, "Response %s [%d|%d]") == 0)
    //{
    //    char resource_uri[0x100];
    //    memcpy(resource_uri, va_arg(ArgList, char*), 0x100); // occasional access violations here - TODO FIX
    //    backend_deobfuscate_uri(resource_uri, 0x100);
    //
    //    // potentially dangerous, but acceptable given the context?
    //    // these will return random data from memory for request logs
    //    // but they are ignored by the request format in snprintf
    //    long request_status = va_arg(ArgList, long); // more likely to be error code - response code
    //    long response_status = va_arg(ArgList, long);
    //
    //    char deobfuscated_log[0x100];
    //    memcpy(deobfuscated_log, DstBuf, 0x100);
    //    snprintf(deobfuscated_log, 0x100, Format, resource_uri, request_status, response_status);
    //
    //    printf("[+] %s \n", deobfuscated_log);
    //}

    return result;
}

bool __fastcall transport_secure_key_create_hook(s_transport_session_description* session_description)
{
    FUNCTION_DEF(0x3BC0, bool, __fastcall, transport_secure_key_create, s_transport_session_description* session_description);
    bool result = transport_secure_key_create(session_description);
    
    xnet_shim_register(&transport_security_globals->address, &session_description->host_address, &session_description->session_id);
    return result;
}

void __fastcall managed_session_delete_session_internal_hook(long managed_session_index, c_managed_session* managed_session)
{
    FUNCTION_DEF(0x28C30, void, __fastcall, managed_session_delete_session_internal, long managed_session_index, c_managed_session* managed_session);

    if (managed_session->flags.test(_online_managed_session_created_bit) && managed_session->session_class == _network_session_class_online)
        xnet_shim_unregister(&transport_security_globals->address);

    managed_session_delete_session_internal(managed_session_index, managed_session);
}

void __fastcall network_session_interface_set_local_name_hook(wchar_t const* machine_name, wchar_t const* session_name)
{
    FUNCTION_DEF(0x2E680, void, __fastcall, network_session_interface_set_local_name, wchar_t const* machine_name, wchar_t const* session_name);
    network_session_interface_set_local_name(k_anvil_machine_name, k_anvil_session_name);
}

long __cdecl internal_halt_render_thread_and_lock_resources_hook(const char* file_name, long line_number)
{
    FUNCTION_DEF(0x94CB0, long, __cdecl, internal_halt_render_thread_and_lock_resources, const char* file_name, long line_number);
    long result = internal_halt_render_thread_and_lock_resources(file_name, line_number);

    game_engine_attach_to_simulation();
    return result;
}

void __fastcall game_engine_player_added_hook(datum_index absolute_player_index)
{
    game_engine_player_added(absolute_player_index);
}

void __fastcall player_set_facing_hook(datum_index player_index, real_vector3d* forward)
{
    s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set(14, true);
    simulation_action_object_create(player_data->unit_index);
    simulation_action_object_update(player_data->unit_index, &update_flags);

    // this part was removed from HO
    //if ( game_is_multiplayer() )
    //{
    //  game_engine_add_starting_equipment(player_object_index);
    //  game_engine_spawn_influencer_record_player_spawn(player_index3);
    //}

    FUNCTION_DEF(0xB6300, void, __fastcall, player_set_facing, datum_index player_index, real_vector3d* forward);
    player_set_facing(player_index, forward);
}

void __fastcall object_scripting_clear_all_function_variables_hook(datum_index object_index)
{
    simulation_action_object_delete(object_index);

    FUNCTION_DEF(0x475CF0, void, __fastcall, object_scripting_clear_all_function_variables, datum_index object_index);
    object_scripting_clear_all_function_variables(object_index);
}

// TODO: this doesn't work outside of proxydll atm, our launcher is injecting too late
c_static_string<64>* __cdecl c_static_string_64_print_hook(c_static_string<64>* static_string, char const* format, ...)
{
    static_string->print("Halo Online %s %s", anvil_get_config_string(), "Live_release_11.1.604673");
    return static_string;
}

void __cdecl game_engine_update_round_conditions_hook()
{
    game_engine_update_round_conditions();
}

//long __cdecl ui_get_player_model_id_evaluate_hook(long a1, long a2)
//{
//    FUNCTION_DEF(0x1210F0, long, __fastcall, hs_return, long a1, long a2);
//    return hs_return(a2, 1);
//}

void __cdecl simulation_action_game_engine_globals_update_with_bitmask(ulong64 raw_bits)
{
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set_raw_bits(raw_bits);
    simulation_action_game_engine_globals_update(&update_flags);
}

void __cdecl simulation_action_game_statborg_update_with_bitmask(ulong64 raw_bits)
{
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set_raw_bits(raw_bits);
    simulation_action_game_statborg_update(&update_flags);
}

void __cdecl simulation_action_game_statborg_update_with_flag(long flag)
{
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set(flag, true);
    simulation_action_game_statborg_update(&update_flags);
}

// doing this from now on, we have so many simulation update calls to add back that rewriting each
// function that calls them would be untenable. this allows us to insert them into the existing code
__declspec(naked) void game_engine_update_time_hook()
{
    __asm
    {
        // save ecx register across function call (time remaining events won't trigger otherwise)
        push ecx

        // call our inserted function
        push 0
        push 32 // set flag 5
        call simulation_action_game_engine_globals_update_with_bitmask
        add esp, 8

        // restore ecx register
        pop ecx

        // execute the original instruction(s) we replaced
        cmp ecx, 708h

        // return back to the original code
        mov eax, module_base
        add eax, 0xC98D1
        jmp eax
    }
}

// TODO: figure out what this actually does
// c_game_statborg::stats_finalize_for_game_end
__declspec(naked) void stats_finalize_for_game_end_hook()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov byte ptr[eax + 1056], 1

        // call our inserted function
        push 0
        push 16777216 // set flag 24
        call simulation_action_game_statborg_update_with_bitmask
        add esp, 8

        // return back to the original code
        mov eax, module_base
        add eax, 0xCA2F7
        jmp eax
    }
}

__declspec(naked) void adjust_player_stat_hook()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov edx, [ebp + 20]
        mov[edi + 4], ax

        // save registers across the function call
        push eax
        push edx

        // call our inserted function
        push esi // push absolute_player_index
        call simulation_action_game_statborg_update_with_flag
        add esp, 4

        // restore registers
        pop edx
        pop eax

        // return back to the original code
        mov ebx, module_base
        add ebx, 0x1AF624
        jmp ebx
    }
}

__declspec(naked) void player_changed_teams_hook()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov ecx, [esi + 36]
        inc dword ptr[eax]
        cmp byte ptr[ecx + 20], 4

        // save registers across the function call
        push eax
        push edx

        // call our inserted function
        movzx eax, bx // long absolute_player_index = (word)player_index;
        push eax // push absolute_player_index
        call simulation_action_game_statborg_update_with_flag
        add esp, 4

        // restore registers
        pop edx
        pop eax

        // return back to the original code
        mov ecx, module_base
        add ecx, 0xFA95F
        jmp ecx
    }
}

// TODO: test this, also where is this used?
// breakpoint ED's and see if it ever works?
__declspec(naked) void player_indices_swapped_hook()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        lea esi, [ebp + 52]
        add edi, eax
        rep movsd

        // call our inserted function
        push edi // absolute_index_a
        call simulation_action_game_statborg_update_with_flag
        add esp, 4
        push esi // absolute_index_b
        call simulation_action_game_statborg_update_with_flag
        add esp, 4

        // return back to the original code
        mov edx, module_base
        add edx, 0xFA7B8
        jmp edx
    }
}

__declspec(naked) void stats_reset_for_round_switch_hook1()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        cmp byte ptr[ecx + 106724], 0
        jz short else_label
        mov byte ptr[eax + 18], 1
        jmp short end_label
        else_label:
        xor ecx, ecx
        mov[eax - 28], cx
        end_label:

        // save registers across the function call
        push eax
        push ecx
        push edx

        // call our inserted function
        push esi // player_absolute_index
        call simulation_action_game_statborg_update_with_flag
        add esp, 4

        // restore registers
        pop edx
        pop ecx
        pop eax

        // return back to the original code
        mov ebx, module_base
        add ebx, 0x1AEEB9
        jmp ebx
    }
}

__declspec(naked) void stats_reset_for_round_switch_hook2()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        cmp byte ptr[eax + 106724], 0
        jz short else_label
        mov byte ptr[esi], 1
        jmp short end_label
        else_label:
        xor eax, eax
        mov[esi - 24], ax
        end_label:

        // call our inserted function
        mov eax, edi // team_index + 16
        add eax, 16
        push eax
        call simulation_action_game_statborg_update_with_flag
        add esp, 4

        // restore register

        // return back to the original code
        mov eax, module_base
        add eax, 0x1AEF20
        jmp eax
    }
}

void __fastcall adjust_team_stat_hook(c_game_statborg* thisptr, void* unused, e_game_team team_index, long statistic, short unknown, long value)
{
    thisptr->adjust_team_stat(team_index, statistic, unknown, value);
}

void __fastcall game_results_statistic_set_hook(datum_index absolute_player_index, e_game_team team_index, long statistic, long value)
{
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set(team_index + 16, true);
    simulation_action_game_statborg_update(&update_flags);
    game_results_statistic_set(absolute_player_index, team_index, statistic, value);
}

void __fastcall game_engine_player_set_spawn_timer_hook(long player_index, long countdown_ticks)
{
    FUNCTION_DEF(0xC7700, void, __fastcall, game_engine_player_set_spawn_timer, long player_index, long countdown_ticks);
    game_engine_player_set_spawn_timer(player_index, countdown_ticks);
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set(1, true);
    simulation_action_game_engine_player_update((word)player_index, &update_flags);
}

void anvil_dedi_apply_patches()
{
    // enable tag edits
    Patch(0x082DB4, { 0xEB }).Apply();
    Patch::NopFill(Pointer::Base(0x083120), 2);
    Patch::NopFill(Pointer::Base(0x083AFC), 2);
    // contrail gpu freeze fix - twister
    Patch(0x1D6B70, { 0xC3 }).Apply();
    // enable netdebug
    g_network_interface_show_latency_and_framerate_metrics_on_chud = true;
    g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;
}

void anvil_dedi_apply_hooks()
{
    // SESSION HOOKS
    // add back missing host code by replacing existing stripped down functions
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    Hook(0x21AB0, session_idle_hook).Apply();
    Hook(0x2F48E, network_session_update_peer_properties_hook, HookFlags::IsCall).Apply();
    Hook(0x3EEE1F, network_life_cycle_end_hook, HookFlags::IsCall).Apply();
    // set network_life_cycle_create_local_squad call in hf2p_setup_session to create an online session
    Hook(0x3AAF68, create_local_online_squad, HookFlags::IsCall).Apply();
    // add back network_session_check_properties
    Hook(0x2AD9E, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    Hook(0x2DC71, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    // register/unregister the host address description to the xnet shim table on session creation/destruction so we can locate them with one another
    Hook(0x28E32, transport_secure_key_create_hook, HookFlags::IsCall).Apply();
    Hook(0x21342, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Hook(0x28051, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0x284B8).WriteJump(managed_session_delete_session_internal_hook, HookFlags::None);

    // SIMULATION HOOKS
    // allow view_establishment to progress past connection phase to established in update_establishing_view again
    Hook(0x370E0, update_establishing_view_hook).Apply();
    // add game_engine_attach_to_simulation back to game_engine_game_starting
    Hook(0xC703E, internal_halt_render_thread_and_lock_resources_hook, HookFlags::IsCall).Apply();
    // add back simulation_action_game_statborg_update & simulation_action_game_engine_player_update calls
    Hook(0xFA2D0, game_engine_player_added_hook).Apply();
    // add simulation updates back to player_spawn
    Hook(0xBB084, player_set_facing_hook, HookFlags::IsCall).Apply();
    // add simulation_action_object_delete back to object_delete
    Hook(0x3FE1BE, object_scripting_clear_all_function_variables_hook, HookFlags::IsCall).Apply();
    // add simulation_action_game_engine_globals_update back to game_engine_update_round_conditions
    Hook(0xC6C00, game_engine_update_round_conditions_hook).Apply();
    // TODO: nop out leftover instructions
    // add simulation_action_game_engine_globals_update back to game_engine_update_time
    Pointer::Base(0xC98CB).WriteJump(game_engine_update_time_hook, HookFlags::None);
    // add simulation_action_game_statborg_update back to c_game_statborg::stats_finalize_for_game_end
    Pointer::Base(0xCA2F0).WriteJump(stats_finalize_for_game_end_hook, HookFlags::None);
    // add simulation_action_game_statborg_update back to c_game_statborg::adjust_player_stat
    Pointer::Base(0x1AF61D).WriteJump(adjust_player_stat_hook, HookFlags::None);
    // add simulation_action_game_statborg_update back to c_game_statborg::adjust_team_stat
    Hook(0x1AF710, adjust_team_stat_hook).Apply();
    // add simulation_action_game_statborg_update back to game_engine_end_round_with_winner > c_game_statborg::adjust_team_stat
    Hook(0xC8A6E, game_results_statistic_set_hook, HookFlags::IsCall).Apply();
    Patch::NopFill(Pointer::Base(0xC8A76), 3); // nop stack callup our wrapper for game_results_statistic_set is already handling
    // add simulation_action_game_statborg_update back to c_game_engine::recompute_team_score > c_game_statborg::adjust_team_stat
    Hook(0x1C7FCD, game_results_statistic_set_hook, HookFlags::IsCall).Apply();
    Patch::NopFill(Pointer::Base(0x1C7FD8), 3); // nop stack callup our wrapper for game_results_statistic_set is already handling
    // add simulation_action_game_statborg_update back to c_game_statborg::player_changed_teams
    Pointer::Base(0xFA956).WriteJump(player_changed_teams_hook, HookFlags::None);
    // add simulation_action_game_statborg_update back to game_engine_player_indices_swapped > c_game_statborg::player_indices_swapped
    Pointer::Base(0xFA7B1).WriteJump(player_indices_swapped_hook, HookFlags::None);
    // add simulation_action_game_statborg_update back to c_game_statborg::stats_reset_for_round_switch
    Pointer::Base(0x1AEE9B).WriteJump(stats_reset_for_round_switch_hook1, HookFlags::None);
    Pointer::Base(0x1AEF0C).WriteJump(stats_reset_for_round_switch_hook2, HookFlags::None);
    // add simulation_action_game_engine_player_update back to game_engine_player_set_spawn_timer (sync spawn countdown? may be internal only)
    Hook(0xFA07F, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();
    Hook(0xFBC5B, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0xFBF43).WriteJump(game_engine_player_set_spawn_timer_hook, HookFlags::None);
    Pointer::Base(0xFBF4F).WriteJump(game_engine_player_set_spawn_timer_hook, HookFlags::None);
    Hook(0xFC01C, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();

    // DEDICATED SERVER HOOKS
    // TODO: hook hf2p_tick and disable everything but the heartbeat service, and reimplement whatever ms23 was doing, do the same for game_startup_internal & game_shutdown_internal 
    // TODO: hook xnet_shim_create_key() to use a lobby/party ID from the API when running as a dedicated server
    // TODO: hook network_session_interface_get_local_user_identifier in c_network_session::create_host_session to add back !game_is_dedicated_server() check
    // TODO: hook transport_secure_address_resolve to get secure address from API when running as a dedicated server
    // TODO: hook main_loading_initialize & main_game_load_map to disable load progress when running as a dedicated server
    // TODO: disable sound & rendering system when running as a dedicated server - optionally allow playing as host & spectate fly cam
    // prevent the game from adding a player to the dedicated host
    Hook(0x2F5AC, peer_request_player_add_hook, HookFlags::IsCall).Apply();
    Hook(0x212CC, network_session_interface_get_local_user_identifier_hook, HookFlags::IsCall).Apply();
    // set peer & session name
    Hook(0x3AA897, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC21A, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC243, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    // hook game window text to display "Dedicated Server" / "Game Server" instead of "Game Client"
    Hook(0x13C3, c_static_string_64_print_hook, HookFlags::IsCall).Apply();
    
    // MISC HOOKS
    // output the message type for debugging
    Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    // hook net_debug_print's vsnprintf_s call to print API logs to the console - temporarily disabled due to crashes
    //Hook(0x55D8BF, vsnprintf_s_net_debug_hook, HookFlags::IsCall).Apply();
    // contrail gpu freeze fix - twister
    Hook(0x28A38A, contrail_fix_hook).Apply();
    // temporary test to force elite ui model on mainmenu
    //Hook(0x2059B0, ui_get_player_model_id_evaluate_hook).Apply();
}