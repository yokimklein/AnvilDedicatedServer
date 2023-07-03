// TODO: This file needs some serious cleanup, I'm sorry to anyone who has to sift through all of this
// I will eventually make it look better - Yokim

#include "hooks.h"
#include <Patch.hpp> // TODO: replace & remove ED's patch system
#include <cseries\cseries.h>
#include <networking\session\network_session.h>
#include <networking\messages\network_message_handler.h>
#include <simulation\simulation_world.h>
#include <networking\messages\network_message_gateway.h>
#include <networking\logic\network_session_interface.h>
#include <networking\network_globals.h>
#include <anvil\backend.h>
#include <networking\logic\network_life_cycle.h>
#include <networking\transport\transport_shim.h>
#include <networking\session\network_managed_session.h>
#include <anvil\server_tools.h>
#include <simulation\simulation_debug_globals.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <anvil\build_version.h>
#include <game\game_results.h>
#include <hf2p\hf2p.h>
#include <objects\objects.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <game\game.h>
#include <units\units.h>
#include <units\bipeds.h>
#include <game\game_engine_spawning.h>
#include <game\game_engine.h>
#include <items\weapons.h>
#define NMD_ASSEMBLY_IMPLEMENTATION
#include <nmd_assembly.h>

enum e_hook_type
{
    _hook_replace,
    _hook_execute_replaced_first,
    _hook_execute_replaced_last,
    _hook_replace_no_nop
};

// helper function for insert_hook, this updates call & jump offsets for the new code destination & verifies short jumps land within the shellcode buffer
void insert_hook_copy_instructions(void* destination, void* source, size_t length, bool redirect_oob_jumps)
{
    byte* code_buffer = new byte[length];
    // copy the instructions we're replacing into a buffer
    memcpy(code_buffer, source, length);

    size_t i = 0;
    do
    {
        size_t instruction_length = nmd_x86_ldisasm((void*)((size_t)source + i), length, NMD_X86_MODE_32);
        // ensure instruction length fits within the length of the buffer provided
        assert(i + instruction_length <= length);
        // assert unimplemented jump instructions - TODO: implement the rest of these
        assert(!(code_buffer[i] >= 0x70 && code_buffer[i] <= 0x73 || code_buffer[i] >= 0x77 && code_buffer[i] <= 0x7B || code_buffer[i] == 0x7D || code_buffer[i] == 0x7F || code_buffer[i] >= 0xE0 && code_buffer[i] <= 0xE3 || code_buffer[i] == 0xEA));
        
        // if jump or call
        if (code_buffer[i] == 0xE8 || code_buffer[i] == 0xE9)
        {
            // ensure instruction length is valid
            assert(instruction_length == 5);
            long offset = *(long*)&code_buffer[i + 1];
            void* destination_address = (void*)(((size_t)source + i) + offset + 5);
            long new_offset = (size_t)destination_address - ((size_t)destination + i) - 5;
            memcpy(&code_buffer[i + 1], &new_offset, sizeof(new_offset));
        }
        // if short jump - jz, jnz, jbe, jl, jle, jmp short
        else if (code_buffer[i] == 0x74 || code_buffer[i] == 0x75 || code_buffer[i] == 0x76 || code_buffer[i] == 0x7C || code_buffer[i] == 0x7E || code_buffer[i] == 0xEB)
        {
            // ensure instruction length is valid
            assert(instruction_length == 2);
            // verify offset fits in buffer
            byte offset = code_buffer[i + 1];
            bool near_jump_is_in_bounds = offset + 2 + i < length; // if this assert fails you've likely ended your replaced instructions with a short jump
            if (!near_jump_is_in_bounds && redirect_oob_jumps)
            {
                offset = (length - (i + 2)); // redirect near jump to NOP at the end of the buffer
                code_buffer[i + 1] = offset;
            }
            else
            {
                assert(near_jump_is_in_bounds);
            }
        }

        i += instruction_length;
    }
    while (i < length);

    memcpy(destination, code_buffer, length);
    delete[] code_buffer;
}

// This only works with parameterless functions - start_address is a baseless offset
// Inserted function must have runtime checks, safebuffers & JustMyCode disabled
// Make sure the destination of any short jumps included in your replaced instructions is also included
// NOTE: if you use the disassembly debug view whilst this is writing, the view will NOT update to the new instructions & bytes - this can make it look like there's a bug when there isn't
// NOTE: set redirect_oob_jumps to true to redirect out of bounds near jumps to the end of the replaced instructions
// NOTE: this does not preserve xmm registers - ensure that required xmm registers after your call aren't overwritten
void insert_hook(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type = _hook_execute_replaced_first, bool redirect_oob_jumps = false)
{
    long code_offset = 0;
    byte preserve_registers[3] = { 0x50, 0x51, 0x52 }; // push eax, push ecx, push edx
    byte restore_registers[3] = { 0x5A, 0x59, 0x58 }; // pop edx, pop ecx, pop eax
    byte call_code[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // call w/ 4x placeholder nops where the function address will go
    byte return_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder nops
    byte jump_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder nops

    long length = return_address - start_address;
    if (length < sizeof(jump_code))
    {
        printf("The hook requires at least %d bytes available to overwrite!\n", sizeof(jump_code));
        return;
    }

    // initialise our new code block
    long inserted_code_size = + sizeof(preserve_registers) + sizeof(call_code) + sizeof(restore_registers) + sizeof(return_code);
    if (hook_type != _hook_replace && hook_type != _hook_replace_no_nop)
        inserted_code_size += length;
    byte* inserted_code = (byte*)VirtualAlloc(NULL, inserted_code_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
    if (inserted_code == NULL)
    {
        printf("Failed to allocate memory for hook!\n");
        return;
    }

    if (hook_type == _hook_execute_replaced_first)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, (void*)base_address(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    // preserve registers across call
    memcpy(inserted_code + code_offset, preserve_registers, sizeof(preserve_registers));
    code_offset += sizeof(preserve_registers);

    // call inserted function
    size_t call_offset = ((size_t)inserted_function - (size_t)(inserted_code + code_offset) - sizeof(call_code));
    memcpy(&call_code[1], &call_offset, sizeof(call_code));
    memcpy(inserted_code + code_offset, call_code, sizeof(call_code));
    code_offset += sizeof(call_code);

    // restore registers
    memcpy(inserted_code + code_offset, restore_registers, sizeof(restore_registers));
    code_offset += sizeof(restore_registers);

    if (hook_type == _hook_execute_replaced_last)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, (void*)base_address(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    // return
    size_t return_offset = (base_address(return_address) - (size_t)(inserted_code + code_offset) - sizeof(return_code));
    memcpy(&return_code[1], &return_offset, sizeof(return_code));
    memcpy(inserted_code + code_offset, return_code, sizeof(return_code));
    code_offset += sizeof(return_code);

    // write jump to inserted function
    size_t jump_offset = ((size_t)(inserted_code) - base_address(start_address) - sizeof(jump_code));
    memcpy(&jump_code[1], &jump_offset, sizeof(jump_code));
    memcpy((void*)base_address(start_address), jump_code, sizeof(jump_code));

    if (hook_type != _hook_replace_no_nop)
    {
        // nop the bytes leftover between the original overwritten instructions and the return point
        // this isn't really necessary but it makes looking at the disassembly less cluttered
        memset((void*)(base_address(start_address) + sizeof(jump_code)), 0x90, length - sizeof(jump_code));
    }
}

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* stub_message)
{
    s_network_message const* message = (s_network_message const*)base_address(0x4FFB090);
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

// add debug print back to before life cycle end is called in c_gui_location_manager::update
void __fastcall network_life_cycle_end_hook()
{
    printf("Resetting network location.  If you got here and didn't just issue a console command, this is a bug.\n");
    network_life_cycle_end();
}

// disable contrails to prevent gpu freezing
__declspec(naked) void contrail_fix_hook()
{
    __asm
    {
        add edx, [0x0068A38A]
        cmp edx, -1
        jg render
        push 0x68A3E3
        retn
        render:
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
    bool(__thiscall* network_session_interface_get_local_user_identifier)(s_player_identifier* player_identifier) = reinterpret_cast<decltype(network_session_interface_get_local_user_identifier)>(base_address(0x3D50));
    return !game_is_dedicated_server() && network_session_interface_get_local_user_identifier(player_identifier);
}

// reimplement network_session_check_properties by calling it at the end of network_session_interface_update_session
void __fastcall network_session_interface_update_session_hook(c_network_session* session)
{
    void(__thiscall* network_session_interface_update_session)(c_network_session* session) = reinterpret_cast<decltype(network_session_interface_update_session)>(base_address(0x2F410));
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
    return transport_secure_key_create(session_description);
}

void __cdecl transport_secure_address_resolve_hook()
{
    memset(&transport_security_globals->secure_address, 0, sizeof(s_transport_secure_address));
    transport_secure_address_generate(&transport_security_globals->secure_address);
    transport_security_globals->address_resolved = true;
    memcpy(&transport_security_globals->local_unique_identifier, &transport_security_globals->secure_address, sizeof(s_transport_unique_identifier));
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

void __fastcall player_set_facing_player_spawn_hook(datum_index player_index, real_vector3d* forward)
{
    s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
    simulation_action_object_create(player_data->unit_index);
    simulation_action_object_update(player_data->unit_index, _simulation_unit_update_control);

    // this part was removed from HO
    //if ( game_is_multiplayer() )
    //{
    //  game_engine_add_starting_equipment(player_object_index);
    //  game_engine_spawn_influencer_record_player_spawn(player_index3);
    //}

    player_set_facing(player_index, forward);
}

void __fastcall player_set_facing_hook(datum_index player_index, real_vector3d* forward)
{
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

// TODO: fix crashing
void __fastcall hf2p_loadout_update_active_character_hook(long player_index, s_player_datum* player_data)
{
    FUNCTION_DEF(0xE05E0, void, __cdecl, sub_E05E0, s_player_identifier player_xuid);

    byte active_armor_loadout = player_data->configuration.client.active_armor_loadout;
    byte character_active_index = player_data->configuration.host.s3d_player_customization.character_active_index;
    if (active_armor_loadout <= 2u)
        character_active_index = active_armor_loadout;
    if (character_active_index != player_data->character_type_index)
    {
        s_player_identifier player_xuid = player_data->configuration.host.player_xuid;
        player_data->revenge_shield_boost_multiplier = 0;
        player_data->character_type_index = character_active_index;
        sub_E05E0(player_xuid);
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_character_type);
    }
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

void __cdecl simulation_action_game_engine_player_update_with_bitmask(datum_index player_index, ulong64 raw_bits)
{
    c_flags<long, ulong64, 64> update_flags = {};
    update_flags.set_raw_bits(raw_bits);
    simulation_action_game_engine_player_update((word)player_index, &update_flags);
}

// TODO: replace this with new update call
void __stdcall simulation_action_object_update_with_bitmask(datum_index object_index, ulong64 raw_bits)
{
    c_simulation_object_update_flags update_flags = c_simulation_object_update_flags();
    update_flags.set_raw(raw_bits);
    simulation_action_object_update_internal(object_index, update_flags);
}

__declspec(naked) void game_engine_update_time_hook()
{
    __asm
    {
        // preserve register across call
        push ecx

        // call our inserted function
        push 0
        push 32 // flag 5 (1 << 5)
        call simulation_action_game_engine_globals_update_with_bitmask
        add esp, 8

        // restore register
        pop ecx

        // execute the original instruction(s) we replaced
        cmp ecx, 0x708

        // return back to the original code
        mov eax, module_base
        add eax, 0xC98D1
        jmp eax
    }
}

__declspec(naked) void player_spawn_hook()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        call hf2p_set_user_loadout

        // save registers across the function call
        //push eax

        // call our inserted function
        test byte ptr[ebx + 4], 8 // if ((player_data->flags & 8) != 0)
        jz end_label
        push 0
        push 262144 // flag _simulation_player_update_equipment_charges (1 << 18)
        push esi // player_index
        call simulation_action_game_engine_player_update_with_bitmask
        add esp, 12

        // restore registers - TODO ARE THESE NEEDED? CHECK!!
        //pop eax

        // return back to the original code
        end_label:
        mov ecx, module_base
        add ecx, 0xBB098
        jmp ecx
    }
}

__declspec(naked) void object_update_part()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov ecx, edi
        call attachments_update

        // return back to the original code
        mov edx, module_base
        add edx, 0x40490E
        jmp edx
    }
}

// TODO: why isn't the object_needs_rigid_body_update check ever succeeding? is the hook broken or do no rigid bodies exist? the only one I know of is the flood tentacle on cold storage
__declspec(naked) void object_update_hook()
{
    __asm
    {
        // call our inserted function
        mov ecx, edi
        call object_needs_rigid_body_update
        movzx eax, al
        test eax, eax
        je end_label
        push 0
        push 16384 // _simulation_generic_update_rigid_body (1 << 14)
        push edi // object_index
        call simulation_action_object_update_with_bitmask

        end_label:
        jmp object_update_part
    }
}

__declspec(naked) void player_spawn_hook2()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        and ecx, 0x0FFFFFFF7
        mov [ebx + 4], ecx

        // call our inserted function
        push 0
        push 1 // flag _simulation_player_update_spawn_timer (1 << 0)
        push[ebp - 24] // player_index3
        call simulation_action_game_engine_player_update_with_bitmask
        add esp, 12

        // return back to the original code
        mov ecx, module_base
        add ecx, 0xBB43B
        jmp ecx
    }
}

__declspec(naked) void player_spawn_hook3()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov byte ptr [ebx + 0x1754], 0

        // call our inserted function
        push 0
        push 2 // flag _simulation_player_update_early_respawn (1 << 1)
        push [ebp - 24] // player_index3
        call simulation_action_game_engine_player_update_with_bitmask
        add esp, 12

        // return back to the original code
        mov ecx, module_base
        add ecx, 0xBB460
        jmp ecx
    }
}

// TODO: fix the crashes here
_declspec(naked) void hf2p_loadout_update_active_character_call_hook()
{
    __asm
    {
        mov eax, eax
        mov ebx, ebx
        mov ecx, ecx
        mov edx, edx
        // execute the original instruction(s) we replaced
        mov ecx, esi // player_index
        call hf2p_loadout_update_active_character_hook

        // return back to the original code
        mov eax, module_base
        add eax, 0xBAF29
        jmp eax
    }
}

// NEW HOOKS START HERE
// runtime checks need to be disabled for these, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall c_life_cycle_state_handler_pre_game__squad_game_start_status_update_hook()
{
    c_network_session_parameter_game_start_status* parameter;
    s_network_session_parameter_game_start_status* start_status;
    __asm mov parameter, ecx;
    __asm lea eax, [esp + 0x78]; // [esp+0x398-0x388] sp is 0x39C
    __asm mov start_status, eax;
    parameter->set(start_status);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_hook()
{
    simulation_action_game_statborg_update(_simulation_statborg_update_finalize_for_game_end);
}

__declspec(safebuffers) void __fastcall c_game_statborg__adjust_player_stat_hook()
{
    long absolute_player_index; __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook1()
{
    // I have to move this into eax first otherwise it'll refuse to compile for some reason
    long absolute_player_index; __asm mov eax, [esp + 0x90] __asm mov absolute_player_index, eax; // originally dword ptr [esp + 0x50 - 0x24], sp was 0x50
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook2()
{
    long absolute_player_index; __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_earn_wp_event_hook()
{
    long absolute_player_index;
    __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook3()
{
    long team_index; __asm mov team_index, ebx;
    simulation_action_game_statborg_update(_simulation_statborg_update_team0 + team_index);
}

__declspec(safebuffers) void __fastcall c_game_engine__recompute_team_score_hook()
{
    long team_index; __asm mov team_index, edi;
    simulation_action_game_statborg_update(_simulation_statborg_update_team0 + team_index);
}

__declspec(safebuffers) void __fastcall player_changed_teams_hook()
{
    short player_index; __asm mov player_index, bx;
    simulation_action_game_statborg_update(_simulation_statborg_update_player0 + player_index);
}

__declspec(safebuffers) void __fastcall player_indices_swapped_hook()
{
    long absolute_index_a; __asm mov absolute_index_a, edi;
    long absolute_index_b; __asm mov absolute_index_b, esi;
    simulation_action_game_statborg_update(absolute_index_a);
    simulation_action_game_statborg_update(absolute_index_b);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_hook2()
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_game_finished);
}

__declspec(safebuffers) void __fastcall object_set_position_internal_hook1()
{
    datum_index object_index;
    __asm mov object_index, edi;
    simulation_action_object_update(object_index, _simulation_object_update_position);
}

__declspec(safebuffers) void __fastcall object_damage_update_hook1()
{
    datum_index object_index;
    __asm mov eax, [ebp + 0xF4] __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_update_hook2()
{
    datum_index object_index;
    short body_stun_ticks;
    s_unit_data* unit;
    bool unknown_bool;
    float unknown_ticks;
    float new_body_vitality;
    float unknown_vitality;
    __asm
    {
        mov eax, [ebp + 0xF4]
        mov object_index, eax
        mov body_stun_ticks, cx
        mov unit, edi
        mov al, [ebp + 0x113] // original was [ebp - 0x01]
        mov unknown_bool, al
        movss unknown_ticks, xmm0
        movss new_body_vitality, xmm3
        movss xmm1, [ebp + 0x108] // original was [ebp - 0x0C]
        movss unknown_vitality, xmm1
    }

    if (body_stun_ticks)
    {
        if (!game_is_predicted())
        {
            unit->body_stun_ticks = body_stun_ticks - 1;
        }
        else
        {
            unknown_bool = true;
            simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
        }
    }
    else
    {
        long object_type = 1 << unit->object_identifier.type.get();
        float vitality_delta = get_tls()->game_time_globals->seconds_per_tick * unknown_ticks;
        if (TEST_BIT(_object_mask_unit, object_type))
        {
            float vitality_multiplier = game_difficulty_get_team_value(3, unit->team);
            new_body_vitality = unknown_vitality;
            vitality_delta = vitality_delta * vitality_multiplier;
        }
        unit->damage_flags |= 0x200; // enable _model_shield_depletion_is_permanent_bit?
        float vitality_result = unit->body_vitality + vitality_delta;
        unit->body_vitality = vitality_result;
        if (vitality_result > new_body_vitality)
        {
            unit->body_vitality = new_body_vitality;
            unit->damage_flags &= ~0x200; // disable _model_shield_depletion_is_permanent_bit?
            simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
        }
        else
        {
            unknown_bool = true;
            simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
        }
    }

    // set unknown_bool value back to its original address
    __asm mov al, unknown_bool;
    __asm mov [ebp + 0x113], al;
}

__declspec(safebuffers) void __fastcall object_damage_update_hook3()
{
    datum_index object_index;
    s_unit_data* unit;
    bool unknown_bool;
    short shield_stun_ticks;
    __asm
    {
        mov shield_stun_ticks, ax
        mov eax, [ebp + 0xF4]
        mov object_index, eax
        mov unit, edi
        mov al, [ebp + 0x113] // original was [ebp - 0x01]
        mov unknown_bool, al
    }

    if (!game_is_predicted() && shield_stun_ticks != 0x7FFF)
    {
        shield_stun_ticks--;
        unit->shield_stun_ticks = shield_stun_ticks;
        unknown_bool = true;
    }
    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);

    // set unknown_bool value back to its original address
    __asm mov al, unknown_bool;
    __asm mov[ebp + 0x113], al;
}

__declspec(safebuffers) void __fastcall object_damage_shield_hook1()
{
    s_player_datum* player_data;
    __asm mov player_data, esi;
    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_shield_hook2()
{
    datum_index object_index;
    __asm mov object_index, esi;
    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_body_hook1()
{
    datum_index object_index;
    __asm mov object_index, edx;
    simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
}

__declspec(safebuffers) void __fastcall object_deplete_body_internal_hook1()
{
    datum_index object_index;
    __asm mov object_index, edi;
    simulation_action_object_update(object_index, _simulation_object_update_dead);
}

__declspec(safebuffers) void __fastcall damage_response_fire_hook()
{
    s_object_data* object;
    datum_index object_index;
    long damage_section_index;
    long response_index;
    __asm
    {
        mov eax, [esp + 0x7C + 0x20] // sp 0x98, [esp+0x94-0x74]
        mov object, eax

        mov eax, [esp + 0x7C + 0x10] // sp 0x98, [esp+0x94-0x84]
        mov object_index, eax

        mov eax, [ebp + 0xB0 + 0x10]
        mov damage_section_index, eax

        mov eax, [ebp + 0xB0 + 0x14]
        mov response_index, eax
    }
    if (!game_is_predicted() && object->gamestate_index != -1)
    {
        simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_all_damage);
        c_simulation_object_update_flags update_flags = c_simulation_object_update_flags();
        update_flags.set_flag(object_index, _simulation_object_update_region_state);
        update_flags.set_flag(object_index, _simulation_object_update_constraint_state);
        simulation_action_object_update_internal(object_index, update_flags);
    }
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook()
{
    datum_index object_index;
    s_damage_owner* damage_owner;
    bool skip_update;
    __asm
    {
        mov object_index, ecx
        mov damage_owner, edx
        mov al, byte ptr [ebp + 0x1C] // [ebp+0x08] - sp was 0x04
        mov skip_update, al
    }
    object_set_damage_owner(object_index, damage_owner, skip_update);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook2()
{
    datum_index object_index;
    __asm mov eax, dword ptr [edi + 0x30]
    __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook3()
{
    datum_index object_index;
    __asm mov eax, [ebp + 0xE64] // [ebp-0x18] - sp was 0xE64
    __asm mov object_index, eax
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook4()
{
    datum_index object_index;
    __asm mov eax, [ebp + 0x17C] // [ebp-0x10] - sp was 0x17C
    __asm mov object_index, eax
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook5()
{
    datum_index object_index;
    __asm mov object_index, ebx
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook6()
{
    datum_index object_index;
    __asm mov eax, [ebp + 0xA8] // [ebp+0x08] - sp was 0x90
    __asm mov object_index, eax
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall weapon_age_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, edi;
    weapon_delay_predicted_state(weapon_index);
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_barrel_fire_hook()
{
    datum_index weapon_index;
    __asm mov eax, [esp + 0xB8] __asm mov weapon_index, eax;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_magazine_execute_reload_hook()
{
    datum_index weapon_index;
    __asm mov eax, [ebp + 0x20] __asm mov weapon_index, eax;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_magazine_update_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, ebx;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_report_kill_hook()
{
    datum_index weapon_index;
    __asm mov eax, [ebp + 0x24] __asm mov weapon_index, eax;
    simulation_action_weapon_state_update(weapon_index);
}

// TODO: untested, ensure ebp offset is correct
__declspec(safebuffers) void __fastcall weapon_set_current_amount_hook()
{
    datum_index weapon_index;
    __asm mov eax, [ebp + 0x1C] __asm mov weapon_index, eax;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_set_total_rounds_hook()
{
    datum_index weapon_index;
    __asm mov eax, [ebp + 0x28] __asm mov weapon_index, eax;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_take_inventory_rounds_hook1()
{
    datum_index weapon_index;
    __asm mov weapon_index, edi;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_take_inventory_rounds_hook2()
{
    datum_index unit_weapon_object_index;
    __asm mov unit_weapon_object_index, esi;
    simulation_action_weapon_state_update(unit_weapon_object_index);
}

__declspec(safebuffers) void __fastcall weapon_trigger_update_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, ebx
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall hf2p_podium_tick_hook()
{
    long player_index;
    __asm mov player_index, esi
    hf2p_trigger_player_podium_taunt(player_index);
}
#pragma runtime_checks("", restore)

__declspec(naked) void object_set_position_internal_hook2()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov eax, [eax + 8]
        mov [ecx + 0x74], eax

        // eax, ecx & edx are caller-saved across a cdecl function call - save their values
        push eax
        push ecx

        // call our inserted function
        push 0
        push 4 // _simulation_object_update_forward_and_up (1 << 2)
        push edi // object_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop ecx
        pop eax

        // return back to the original code
        mov eax, module_base
        add eax, 0x3FC066
        jmp eax
    }
}

__declspec(naked) void object_set_position_internal_hook3()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov eax, [esp + 0x70 - 4]
        mov [ecx + 0x5C], eax

        // eax, ecx & edx are caller-saved across a cdecl function call - save their values
        push ecx
        push edx

        // call our inserted function
        push 0
        push 2 // _simulation_object_update_position (1 << 1)
        push ebx // object_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop edx
        pop ecx

        // return back to the original code
        mov eax, module_base
        add eax, 0x404ABB
        jmp eax
    }
}

__declspec(naked) void object_set_position_internal_hook4()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        mov eax, [esi + 8]
        mov [ecx + 0x74], eax

        // eax, ecx & edx are caller-saved across a cdecl function call - save their values
        push eax

        // call our inserted function
        push 0
        push 4 // _simulation_object_update_forward_and_up (1 << 2)
        push ebx // object_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop eax

        // return back to the original code
        mov edx, module_base
        add edx, 0x404ADD
        jmp edx
    }
}

__declspec(naked) void object_apply_acceleration_hook()
{
    __asm
    {
        // save object_index value
        push ecx

        // execute the original instruction(s) we replaced
        mov ecx, [eax + ebx * 8 + 0x0C]
        movq qword ptr [ecx + 0x78], xmm0
        mov eax, [esp + 0x30 + -0xC + 0x8]
        mov [ecx + 0x80], eax
        movq xmm0, qword ptr [esp + 0x30 + -0x18]
        movq qword ptr [ecx + 0x84], xmm0
        mov eax, [esp + 0x30 + -0x18 + 0x8]
        mov [ecx + 0x8C], eax

        // retrieve object_index value
        pop ecx
        // call our inserted function
        push 0
        push 48 // _simulation_object_update_translational_velocity & _simulation_object_update_angular_velocity ((1 << 4) | (1 << 5))
        push ecx // object_index
        call simulation_action_object_update_with_bitmask

        // return back to the original code
        mov ecx, module_base
        add ecx, 0x3FC833
        jmp ecx
    }
}

__declspec(naked) void object_set_velocities_internal_hook()
{
    __asm
    {
        // wrap usercall function to rewritten function
        push ebp
        mov ebp, esp
        and esp, 0x0FFFFFFF8
        
        push 0 // skip_update = false
        push [ebp + 0x8] // angular_velocity
        push edx // transitional_velocity
        push ecx // object_index
        call object_set_velocities_internal
        // match behaviour of the original optimised calling convention
        // return velocity values
        pop ecx
        pop edx
        add esp, 8

        // cleanup
        mov esp, ebp
        pop ebp
        retn
    }
}

void __cdecl object_set_at_rest_simulation_update(datum_index object_index)
{
    if (TEST_BIT(_object_mask_item, object_get_type(object_index)))
        simulation_action_object_update(object_index, _simulation_item_update_set_at_rest);
    else if (TEST_BIT(_object_mask_projectile, object_get_type(object_index)))
        simulation_action_object_update(object_index, _simulation_projectile_update_set_at_rest);
}

// c_simulation_generic_entity_definition::object_required_to_join_game - UNTESTED!! TODO: figure out how to call this!
__declspec(naked) void object_set_at_rest_hook2()
{
    __asm
    {
        // call our inserted function
        push edi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        mov eax, edi
        pop edi
        pop esi
        mov esp, ebp
        pop ebp
        retn 0x14
    }
}

// c_simulation_vehicle_entity_definition::create_object - UNTESTED!!TODO: figure out how to call this!
__declspec(naked) void object_set_at_rest_hook3()
{
    __asm
    {
        // call our inserted function
        push edi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        movzx ecx, word ptr [ebx + 8]
        mov eax, 0x3EDDCDC
        add eax, module_base
        mov eax, ds:[eax]

        // return back to the original code
        mov edx, module_base
        add edx, 0x725B1
        jmp edx
    }
}

// object_attach_to_node_immediate
__declspec(naked) void object_set_at_rest_hook4()
{
    __asm
    {
        // call our inserted function
        push edi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        mov esi, [esp + 0x88 - 0x74]
        mov ecx, esi

        // return back to the original code
        mov eax, module_base
        add eax, 0x400AD7
        jmp eax
    }
}

// projectile_attach
__declspec(naked) void object_set_at_rest_hook5()
{
    __asm
    {
        // preserve register
        push edx

        // call our inserted function
        push [esp + 0x5C - 0x48]  // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // restore register
        pop edx

        // execute the original instruction(s) we replaced
        cmp ebx, 0xFFFFFFFF
        jz if_label

        // return back to the original code
        mov eax, module_base
        add eax, 0x467DF0
        jmp eax

        if_label:
        mov eax, module_base
        add eax, 0x467F3C
        jmp eax
    }
}

// projectile_collision
__declspec(naked) void object_set_at_rest_hook6()
{
    __asm
    {
        // execute the original instruction(s) we replaced
        test eax, 0x7377
        jz if_label1
        mov eax, [edx + 0xAC]
        shr eax, 7
        test al, 1
        jz if_label1
        cmp dword ptr[edx + 0xA0], 0xFFFFFFFF
        jnz if_label2

        if_label1:
        or dword ptr[edx + 0xAC], 0x200
        
        if_label2:
        // preserve register
        push eax

        // call our inserted function
        push [esp + 0x25C - 0x210]  // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // restore register
        pop eax

        // execute the original instruction(s) we replaced
        mov ecx, [esp + 0x258 - 0x248]

        // return back to the original code
        mov edx, module_base
        add edx, 0x464ECB
        jmp edx
    }
}

// projectile_initial_update - called for conically fired projectiles, ie shotguns
__declspec(naked) void object_set_at_rest_hook7()
{
    __asm
    {
        // call our inserted function
        push [esp + 0x298 - 0x28C]  // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        movss xmm0, dword ptr [edi + 0x60]

        // return back to the original code
        mov eax, module_base
        add eax, 0x462436
        jmp eax
    }
}

// object_early_mover_delete - UNTESTED!! TODO: check once we have an early mover to test with! create one with an existing vehicle?
__declspec(naked) void object_set_at_rest_hook8()
{
    __asm
    {
        // call our inserted function
        push esi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        mov eax, [edi + 0x0C]
        mov ecx, [esp + 0x28 - 0x18]

        // return back to the original code
        mov eax, module_base
        add eax, 0x46D288
        jmp eax
    }
}

// swarm_accelerate > creature_accelerate inlined > object_set_at_rest - UNTESTED!! TODO: I'm pretty sure swarms are only used for the flood, so it's probably fine to ignore this
__declspec(naked) void object_set_at_rest_hook9()
{
    __asm
    {
        // call our inserted function
        push esi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        push 0
        lea edx, [esp + 0x3C - 0xC]
        mov ecx, esi

        // return back to the original code
        mov eax, module_base
        add eax, 0x6D50F7
        jmp eax
    }
}

// scenery_new
void __fastcall object_set_at_rest_hook10(datum_index object_index)
{
    FUNCTION_DEF(0x490200, void, __fastcall, scenery_animation_idle, datum_index object_index);

    object_set_at_rest_simulation_update(object_index);
    scenery_animation_idle(object_index);
}

// vehicle_program_activate
__declspec(naked) void object_set_at_rest_hook11()
{
    __asm
    {
        // call our inserted function
        push esi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // execute the original instruction(s) we replaced
        or eax, 0xFFFFFFFF
        mov [esp + 0x80 - 0x34], ax

        // return back to the original code
        mov ecx, module_base
        add ecx, 0x4BEBBF
        jmp ecx
    }
}

// vehicle_program_update - UNTESTED!! TODO: figure out how to call this!
__declspec(naked) void object_set_at_rest_hook12()
{
    __asm
    {
        // call our inserted function
        push edi // object_index
        call object_set_at_rest_simulation_update
        add esp, 4

        // return back to the original code
        mov eax, module_base
        add eax, 0x4C055D
        jmp eax
    }
}

// unit_custom_animation_play_animation_submit
void __fastcall object_set_at_rest_hook13(datum_index object_index)
{
    FUNCTION_DEF(0x4056A0, void, __fastcall, object_compute_node_matrices, datum_index object_index);

    object_set_at_rest_simulation_update(object_index);
    object_compute_node_matrices(object_index);
}

void __fastcall object_set_at_rest_hook(datum_index object_index)
{
    object_wake(object_index);
    object_set_at_rest_simulation_update(object_index);
}

__declspec(naked) void event_generate_part_hook()
{
    __asm
    {
        // call our inserted function
        push esi // object_index
        call simulation_action_object_create
        add esp, 4

        // return back to the original code
        mov eax, module_base
        add eax, 0x114BF5
        jmp eax
    }
}

datum_index __fastcall weapon_barrel_create_projectiles_hook1(void* thisptr)
{
    FUNCTION_DEF(0x3FCEE0, datum_index, __thiscall, object_new, void* thisptr);

    datum_index object_index = object_new(thisptr);
    if (object_index != -1)
        simulation_action_object_create(object_index);
    return object_index;
}

__declspec(naked) void weapon_barrel_create_projectiles_hook2()
{
    __asm
    {
        // call our inserted function
        // if (v220) 
        cmp [esp + 0x2250 - 0x2244], 0 // v220
        jz if_failed

        push [esp + 0x2250 - 0x2230] // object_index
        call simulation_action_object_create
        add esp, 4

        if_failed:
        // execute the instructions we replaced
        mov eax, [esp + 0x2250 - 0x21F8]
        xor dl, dl

        // return back to the original code
        mov ecx, module_base
        add ecx, 0x4394A8
        jmp ecx
    }
}

__declspec(naked) void throw_release_hook1()
{
    __asm
    {
        cmp [ebp + 0x20], 0
        jnz sim_update
        mov eax, module_base
        add eax, 0x47CFC7
        jmp eax

        sim_update:
        push esi // object_index
        call simulation_action_object_create
        add esp, 4

        mov eax, module_base
        add eax, 0x47D185
        jmp eax
    }
}

__declspec(naked) void throw_release_hook2()
{
    __asm
    {
        add esp, 4
        test al, al
        jnz sim_update
        mov eax, module_base
        add eax, 0x47D17B
        jmp eax

        sim_update:
        push esi // object_index
        call simulation_action_object_create
        add esp, 4
        
        mov eax, module_base
        add eax, 0x47D185
        jmp eax
    }
}

__declspec(naked) void equipment_activate_hook()
{
    __asm
    {
        // preserve register
        push eax

        push eax // object_index
        call simulation_action_object_create
        add esp, 4

        // restore register
        pop eax

        // original replaced instructions
        mov ecx, [esp + 0x358 - 0x348]
        mov [ecx + 0x1A0], eax

        // return
        mov eax, module_base
        add eax, 0x451498
        jmp eax
    }
}

__declspec(naked) void item_in_unit_inventory_hook1()
{
    __asm
    {
        push edi // object_index
        call simulation_action_object_create
        add esp, 4

        // original replaced instructions
        mov eax, [ebx + 0x0C]
        mov ecx, [ebp - 0x08]

        // return
        mov edx, module_base
        add edx, 0x48433D
        jmp edx
    }
}

__declspec(naked) void item_in_unit_inventory_hook2()
{
    __asm
    {
        // original replaced instructions
        or eax, 0xFFFFFFFF
        mov [esi + 0x1E], ax

        // preserve register
        push ecx

        push edi // object_index
        call simulation_action_object_delete
        add esp, 4

        // restore register
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x48418D
        jmp eax
    }
}

__declspec(naked) void c_map_variant__remove_object_hook()
{
    __asm
    {
        // original replaced if statement
        cmp ecx, 0xFFFFFFFF
        jz sim_update
        mov eax, [esi]
        movzx ecx, cx
        mov eax, [eax + 0x0C]
        add ecx, ecx
        mov eax, [eax + 0x44]
        mov eax, [eax + ecx * 8 + 0x0C]
        mov [eax + 0x1C], dx
        jmp sim_update

        sim_update:
        // if (game_is_authoritative())
        call game_is_authoritative
        test al, al
        jz return_label
        push 0
        push 8192 // _simulation_object_update_map_variant_index (1 << 13)
        push [ebp + 0xC] // object_index
        call simulation_action_object_update_with_bitmask

        // return
        return_label:
        mov eax, module_base
        add eax, 0xADB52
        jmp eax
    }
}

__declspec(naked) void c_map_variant__unknown4_hook1()
{
    __asm
    {
        push 0
        push 6 // _simulation_object_update_position and _simulation_object_update_forward_and_up ((1 << 1) | (1 << 2))
        push [ebx + esi + 0x134] // object_index
        call simulation_action_object_update_with_bitmask

        // execute instructions we replaced
        mov ax, [ebx + esi + 0x130]
        shr ax, 9

        // return
        mov ecx, module_base
        add ecx, 0xABA8A
        jmp ecx
    }
}

__declspec(naked) void c_map_variant__unknown4_hook2()
{
    __asm
    {
        // execute instructions we replaced
        mov eax, [esp + 0x84 - 0x70]
        add esp, 4

        // preserve register
        push eax

        push 0
        push 2048 // _simulation_object_update_parent_state (1 << 11)
        push[ebx + esi + 0x134] // object_index
        call simulation_action_object_update_with_bitmask

        // restore register
        pop eax

        // return
        mov ecx, module_base
        add ecx, 0xABAAA
        jmp ecx
    }
}

void __fastcall player_set_unit_index_hook1(datum_index unit_index, bool unknown)
{
    simulation_action_object_update(unit_index, _simulation_unit_update_control);
    unit_set_actively_controlled(unit_index, unknown);
}

__declspec(naked) void player_set_unit_index_hook2()
{
    __asm
    {
        // original replaced instructions
        call player_clear_assassination_state

        push 1 // _simulation_unit_update_assassination_data (1 << 32)
        push 0
        push [edi + 0x30] // unit_index
        call simulation_action_object_update_with_bitmask

        // return
        mov ecx, module_base
        add ecx, 0xB628E
        jmp ecx
    }
}

__declspec(naked) void unit_died_hook()
{
    __asm
    {
        // original replaced instructions
        movss dword ptr [eax + 0x404], xmm0

        // preserve register
        push edx

        // biped/vehicle update
        cmp byte ptr[eax + 0x9A], 1
        jnz biped_update

        // vehicle update
        push 0
        push 131072 // _simulation_vehicle_update_active_camo (1 << 17)
        push esi // unit_index
        call simulation_action_object_update_with_bitmask
        jmp return_label

        // biped update
        biped_update:
        push 0
        push 134217728 // _simulation_unit_update_active_camo (1 << 27)
        push esi // unit_index
        call simulation_action_object_update_with_bitmask

        // restore register
        return_label:
        pop edx

        // return
        mov eax, module_base
        add eax, 0x421471
        jmp eax
    }
}

__declspec(naked) void grenade_throw_move_to_hand_hook()
{
    __asm
    {
        // original replaced instructions
        mov [ecx + 0x324], al

        // preserve registers across call
        push ecx
        push edx

        push 0
        push 67108864 // _simulation_unit_update_grenade_counts (1 << 26)
        push [ebp - 0x08] // unit_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop edx
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x47D435
        jmp eax
    }
}

__declspec(naked) void unit_add_grenade_to_inventory_hook()
{
    __asm
    {
        // original replaced instructions
        inc byte ptr[esi + ebx + 0x324]

        // preserve registers across call
        push edx

        push 0
        push 67108864 // _simulation_unit_update_grenade_counts (1 << 26)
        push [ebp - 0x08] // unit_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop edx

        // return
        mov ecx, module_base
        add ecx, 0x4243DF
        jmp ecx
    }
}

__declspec(naked) void unit_add_equipment_to_inventory_hook()
{
    __asm
    {
        // original replaced instructions
        mov [ecx + 0x314], eax

        // preserve registers across call
        push ecx

        push 0
        push 805306368 // _simulation_unit_update_equipment & _simulation_unit_update_equipment_charges ((1 << 28) | (1 << 29))
        push [esp + 0x2C - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x42458C
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook1()
{
    __asm
    {
        // original replaced instructions
        mov [edi + 0x1E4], eax

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask

        // return
        mov eax, module_base
        add eax, 0x418550
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook2()
{
    __asm
    {
        // original replaced instructions
        mov [edi + 0x1B4], eax

        // preserve register across call
        push ecx

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask

        // restore register
        pop ecx
        
        // return
        mov eax, module_base
        add eax, 0x418693
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook3()
{
    __asm
    {
        // original replaced instructions
        mov eax, [edx + 0x1A4]

        // preserve register across call
        push edx

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask

        // restore register
        pop edx

        // return
        mov eax, module_base
        add eax, 0x418A79
        jmp eax
    }
}

__declspec(naked) void unit_add_initial_loadout_hook0()
{
    __asm
    {
        // create a new variable to preserve player_object_index in
        sub esp, 0x1C0 // originally sub esp, 0x1BC
        mov [ebp - 0x1BC], ecx // ecx is player_object_index

        // return
        mov eax, module_base
        add eax, 0xFB6E9
        jmp eax
    }
}

__declspec(naked) void unit_add_initial_loadout_hook1()
{
    __asm
    {
        push 0
        push 67108864 // _simulation_unit_update_grenade_counts (1 << 26)
        push [ebp - 0x1BC] // player object index
        call simulation_action_object_update_with_bitmask

        // original replaced instructions
        mov ecx, [ebx + 0x4C]
        cmp ecx, 0xFFFFFFFF

        // return
        mov eax, module_base
        add eax, 0xFBA3A
        jmp eax
    }
}

__declspec(naked) void unit_add_initial_loadout_hook2()
{
    __asm
    {
        push 0
        push 128 // _simulation_object_update_shield_vitality (1 << 7)
        push[ebp - 0x1BC] // player object index
        call simulation_action_object_update_with_bitmask

        // original replaced instructions
        mov byte ptr [ebx + 0x18B4], 0

        // return
        mov eax, module_base
        add eax, 0xFBAE0
        jmp eax
    }
}

__declspec(naked) void projectile_attach_hook()
{
    __asm
    {
        // original replaced instructions
        movss dword ptr [esi + 0x18C], xmm1

        // preserve registers across call
        push eax
        push ecx
        push edx

        push 0
        push 2048 // _simulation_object_update_parent_state (1 << 11)
        push [esp + 0x6C - 0x48] // projectile_index
        call simulation_action_object_update_with_bitmask

        // restore registers
        pop edx
        pop ecx
        pop eax

        // replaced instructions
        mov eax, [eax + 0x0C]
        mov eax, [eax + 0x44]
        mov cl, [eax + ecx * 8 + 3]
        // return
        mov eax, module_base
        add eax, 0x467F23
        jmp eax
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook1() // c_simulation_unit_entity_definition::apply_object_update
{
    __asm
    {
        push [edi + 0x0B4] // looking_vector
        mov edx, [edi + 0xB4] // aiming_vector
        mov ecx, ebx // unit_index
        call unit_set_aiming_vectors

        // return
        mov eax, module_base
        add eax, 0x59F48
        jmp eax
    }
}

__declspec(naked) void c_simulation_weapon_fire_event_definition__apply_object_update_hook1()
{
    __asm
    {
        // original replaced instruction
        mov dword ptr [esp + 0xE0 - 0xC0], 0xFFFFFFFF

        // set new variable
        mov dword ptr [esp + 0xE0 - 0xD4], 0xFFFFFFFF

        // return
        mov edx, module_base
        add edx, 0x60BE5
        jmp edx
    }
}

__declspec(naked) void c_simulation_weapon_fire_event_definition__apply_object_update_hook2()
{
    __asm
    {
        // original replaced instruction
        mov eax, [esp + 0xE0 - 0xC4]
        mov [esp + 0xE0 - 0xC0], edi

        // set new variable
        mov [esp + 0xE0 - 0xD4], edi

        // return
        mov ecx, module_base
        add ecx, 0x60C74
        jmp ecx
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook2() // c_simulation_weapon_fire_event_definition::apply_object_update
{
    __asm
    {
        // replaced instructions
        mov [ecx + 0x1E4], eax

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push [esp + 0xE8 - 0xD4] // unit_index
        call simulation_action_object_update_with_bitmask

        // return
        mov ecx, module_base
        add ecx, 0x610CF
        jmp ecx
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook3() // c_vehicle_auto_turret::control
{
    __asm
    {
        lea eax, [esi + 0x8BC]
        push eax // looking_vector
        mov edx, eax // aiming_vector
        mov ecx, ebx // unit_index
        call unit_set_aiming_vectors

        // return
        mov eax, module_base
        add eax, 0x4A1010
        jmp eax
    }
}

__declspec(naked) void equipment_activate_hook2()
{
    __asm
    {
        // replaced instructions
        mov [edi + 0x198], eax

        push 0
        push 65536 // _simulation_item_update_unknown16 (1 << 16)
        push [esp + 0x360 - 0x328] // equipment_index
        call simulation_action_object_update_with_bitmask

        // return
        mov eax, module_base
        add eax, 0x4514E2
        jmp eax
    }
}

__declspec(naked) void unit_update_energy_hook()
{
    __asm
    {
        // replaced instructions
        mov [esi + 0x31C], eax

        push 0
        push 1073741824 // _simulation_unit_update_consumable_energy (1 << 30)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask

        // return
        mov eax, module_base
        add eax, 0x41B606
        jmp eax
    }
}

__declspec(naked) void equipment_handle_energy_cost_hook0()
{
    __asm
    {
        // add new variable to preserve unit_index in
        sub esp, 0x10 // increased by 4 bytes from 0x0C
        push ebx
        push esi
        push edi
        mov edi, [eax]
        mov esi, ecx

        // preserve unit_index in new variable
        mov dword ptr [esp + 0x18 - 0x0C], esi

        // return
        mov eax, module_base
        add eax, 0x42D2A6
        jmp eax
    }
}

__declspec(naked) void equipment_handle_energy_cost_hook1()
{
    __asm
    {
        // replaced instructions
        call sub_2E7BE0

        push 0
        push 1073741824 // _simulation_unit_update_consumable_energy (1 << 30)
        push [esp + 0x20 - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask

        // return
        mov eax, module_base
        add eax, 0x42D39D
        jmp eax
    }
}

__declspec(naked) void equipment_handle_energy_cost_hook2()
{
    __asm
    {
        // replaced instructions
        call hf2p_set_player_cooldown

        push 0
        push 536870912 // _simulation_unit_update_equipment_charges (1 << 29)
        push [esp + 0x20 - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask

        push 0
        push 262144 // flag _simulation_player_update_equipment_charges (1 << 18)
        push [ebx + 0x198] // unit->player_index
        call simulation_action_game_engine_player_update_with_bitmask
        add esp, 12

        // return
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        retn
    }
}

__declspec(naked) void unit_set_hologram_hook()
{
    __asm
    {
        // replaced instructions
        mov dword ptr[eax + 0x400], 0x3F800000

        // save register across call
        push ecx
        push edx

        // biped/vehicle update
        cmp byte ptr[esi + 0x9A], 1
        jnz biped_update

        // vehicle update
        push 0
        push 131072 // _simulation_vehicle_update_active_camo (1 << 17)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask
        jmp return_label

        // biped update
        biped_update:
        push 0
        push 134217728 // _simulation_unit_update_active_camo (1 << 27)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask

        // restore register
        return_label:
        pop edx
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x42C578
        jmp eax
    }
}

__declspec(naked) void weapon_handle_potential_inventory_item_hook()
{
    __asm
    {
        // original replaced instructions
        mov al, [ebp - 0x01]
        add esp, 8

        push [ebp - 0x30] // weapon index
        call simulation_action_weapon_state_update
        add esp, 4

        // if ( ((1 << *item_tag) & 4) != 0 ) // if statement passes for weapons, but not ammo packs?
        mov ecx, [ebp - 0x28] // item tag
        mov eax, 1
        mov cl, [ecx]
        shl eax, cl
        test al, 4
        je return_label

        push 0
        push 1048576 // _simulation_weapon_update_ammo (1 << 20)
        push [ebp - 0x24] // item index
        call simulation_action_object_update_with_bitmask

        // return
        return_label:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        retn
    }
}

__declspec(naked) void unit_inventory_set_weapon_index_hook0()
{
    __asm
    {
        // create a new variable to preserve unit_index in
        push ebp
        mov ebp, esp
        sub esp, 0x10 // originally sub esp, 0x0C

        mov [ebp - 0x0C], ecx // ecx is unit_index

        // return
        mov eax, module_base
        add eax, 0x426D16
        jmp eax
    }
}

__declspec(naked) void unit_inventory_set_weapon_index_hook1()
{
    __asm
    {
        //c_flags<long, ulong64, 64> update_flags = {};
        //update_flags.set(inventory_index + 18, true);
        //update_flags.set(inventory_index + 22, true);
        //simulation_action_object_update(unit_index, &update_flags);

        // flags1 = (1 << (inventory_index + _simulation_unit_update_weapon1_type))
        mov ecx, [ebp - 0x02] // inventory_index - TODO: is this a datum_index in the original code? make sure those top 16 bits won't interfere with this
        add ecx, 18 // inventory_index + _simulation_unit_update_weapon1_type
        mov edx, 1
        shl edx, cl
        mov eax, edx

        // flags2 = (1 << (inventory_index + _simulation_unit_update_weapon1_state))
        mov ecx, [ebp - 0x02] // inventory_index
        add ecx, 22 // inventory_index + _simulation_unit_update_weapon1_state
        mov edx, 1
        shl edx, cl

        or edx, eax // update_flags = flags1 | flags2;

        push 0
        push edx // update_flags
        push [ebp - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask

        mov ecx, [ebp - 0x0C] // unit_index
        call unit_inventory_cycle_weapon_set_identifier // fastcall

        // return
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        retn
    }
}

__declspec(naked) void unit_handle_deleted_object_hook()
{
    __asm
    {
        // replace inlined unit_inventory_set_weapon_index call with our rewritten one
        push 4 // drop_type
        push -1 // item_index
        mov edx, ebx // inventory_index
        mov ecx, [ebp + 0x08] // unit index
        call unit_inventory_set_weapon_index

        // return
        mov eax, module_base
        add eax, 0x427178
        jmp eax
    }
}

void __fastcall adjust_team_stat_hook(c_game_statborg* thisptr, void* unused, e_game_team team_index, long statistic, short unknown, long value)
{
    thisptr->adjust_team_stat(team_index, statistic, unknown, value);
}

void __fastcall stats_reset_for_round_switch_hook(c_game_statborg* thisptr)
{
    thisptr->stats_reset_for_round_switch();
}

void __fastcall game_engine_player_set_spawn_timer_hook(long player_index, long countdown_ticks)
{
    FUNCTION_DEF(0xC7700, void, __fastcall, game_engine_player_set_spawn_timer, long player_index, long countdown_ticks);
    game_engine_player_set_spawn_timer(player_index, countdown_ticks);
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_spawn_timer);
}

void __fastcall hf2p_player_podium_initialize_hook(long podium_biped_index, long player_index)
{
    hf2p_player_podium_initialize(podium_biped_index, player_index);
}

void __cdecl draw_watermark_hook()
{
    // empty function
}

void __fastcall game_engine_register_object_hook(datum_index object_index)
{
    FUNCTION_DEF(0x172600, void, __fastcall, game_engine_register_object, datum_index object_index);
    game_engine_register_object(object_index);
    simulation_action_object_create(object_index);
}

long __cdecl exceptions_update_hook()
{
    FUNCTION_DEF(0x167CF0, long, __cdecl, exceptions_update);
    PEXCEPTION_POINTERS g_exception_param_exception_pointers = *(PEXCEPTION_POINTERS*)base_address(0x106DEC8);
    if (g_exception_param_exception_pointers && g_exception_param_exception_pointers->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
        return 0;

    return exceptions_update();
}

bool __fastcall c_network_session_parameter_game_start_status__set_hook(c_network_session_parameter_game_start_status* thisptr, void* unused, s_network_session_parameter_game_start_status* start_status)
{
    return thisptr->set(start_status);
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
    //g_network_interface_show_latency_and_framerate_metrics_on_chud = true; // set this to true to enable
    //g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;
    
    // test to replace tutorial map id with diamondback id in damage function
    //Patch(0x40F290, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
    //Patch(0x40F2E3, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
    //Patch(0x411E02, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
}

// TODO: verify all hooks are working as intended
// check for register corruption - especially where i've added new variables
// TODO: insert_hook should sanitise against register corruption - replace all old hooks with new system
void anvil_dedi_apply_hooks()
{
    // hook exceptions_update to catch esoteric crashes
    Hook(0x95C0F, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x98BCB, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x9AB6B, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x9ABAB, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x9EFE3, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x9F07C, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x9F8B1, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0xC3E8B, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0xDCCBD, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x16A63B, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x17C5FE, exceptions_update_hook, HookFlags::IsCall).Apply();
    Hook(0x3DBB5B, exceptions_update_hook, HookFlags::IsCall).Apply();

    // DEDICATED SERVER HOOKS
    // add anvil_session_update() to the end of network_update
    insert_hook(0x246C1, 0x246D4, anvil_session_update);
    // hook c_network_session_parameter_game_start_status::set calls to log when the session start status & start error are updated
    Hook(0x3BA00, c_network_session_parameter_game_start_status__set_hook).Apply();
    insert_hook(0x4DF34, 0x4DF99, c_life_cycle_state_handler_pre_game__squad_game_start_status_update_hook, _hook_replace);
    // hook xnet_shim_create_key() to use a lobby/party ID from the API when running as a dedicated server
    Hook(0x3BC0, transport_secure_key_create_hook).Apply();
    // hook transport_secure_address_resolve to get secure address from API when running as a dedicated server
    Patch::NopFill(Pointer::Base(0x3D17), 0x25);
    Hook(0x3D12, transport_secure_address_resolve_hook, HookFlags::IsCall).Apply(); // TODO: nop old secure address assignment code
    // TODO: hook hf2p_tick and disable everything but the heartbeat service, and reimplement whatever ms23 was doing, do the same for game_startup_internal & game_shutdown_internal
    // TODO: hook network_session_interface_get_local_user_identifier in c_network_session::create_host_session to add back !game_is_dedicated_server() check
    // TODO: set wp event xp rewards at runtime when retrieving title instances from the API - right now we're just doing this in tags
    // TODO: hook main_loading_initialize & main_game_load_map to disable load progress when running as a dedicated server
    // TODO: disable sound & rendering system when running as a dedicated server - optionally allow playing as host & spectate fly cam
    // prevent the game from adding a player to the dedicated host
    //Hook(0x2F5AC, peer_request_player_add_hook, HookFlags::IsCall).Apply();
    //Hook(0x212CC, network_session_interface_get_local_user_identifier_hook, HookFlags::IsCall).Apply();
    // set peer & session name
    Hook(0x3AA897, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC21A, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC243, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    // hook game window text to display "Dedicated Server" / "Game Server" instead of "Game Client"
    Hook(0x13C3, c_static_string_64_print_hook, HookFlags::IsCall).Apply();

    // SESSION HOOKS
    // add back missing host code by replacing existing stripped down functions
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    Hook(0x21AB0, session_idle_hook).Apply();
    // I couldn't directly hook peer_request_properties_update without experiencing access violations, so this will do
    // add back set_peer_address & set_peer_properties to peer_request_properties_update
    Hook(0x2F650, network_session_update_peer_properties).Apply();
    Hook(0x3EEE1F, network_life_cycle_end_hook, HookFlags::IsCall).Apply();
    // set network_life_cycle_create_local_squad call in hf2p_setup_session to create an online session
    Hook(0x3AAF68, create_local_online_squad, HookFlags::IsCall).Apply();
    // add back network_session_check_properties
    Hook(0x2AD9E, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    Hook(0x2DC71, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    // unregister the host address description to the xnet shim table on session destruction - the transport_secure_key_create hook further down handles session creation
    Hook(0x21342, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Hook(0x28051, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0x284B8).WriteJump(managed_session_delete_session_internal_hook, HookFlags::None);

    // hook game_engine_should_spawn_player so we can control the pregame spawn countdown
    Hook(0xFBBA0, game_engine_should_spawn_player).Apply();

    // SIMULATION HOOKS
    // allow view_establishment to progress past connection phase to established in update_establishing_view again
    Hook(0x370E0, update_establishing_view_hook).Apply();
    // add game_engine_attach_to_simulation back to game_engine_game_starting
    Hook(0xC703E, internal_halt_render_thread_and_lock_resources_hook, HookFlags::IsCall).Apply();
    // add game_engine_detach_from_simulation_gracefully back to game_engine_game_ending
    insert_hook(0xC7320, 0xC7353, game_engine_detach_from_simulation_gracefully, _hook_replace);

    // STATBORG UPDATES
    // add back simulation_action_game_statborg_update & simulation_action_game_engine_player_update calls
    Hook(0xFA2D0, game_engine_player_added).Apply();
    // c_game_statborg::stats_finalize_for_game_end
    insert_hook(0xCA2F0, 0xCA2F7, game_engine_update_after_game_hook);
    // c_game_statborg::adjust_player_stat
    insert_hook(0x1AF61D, 0x1AF624, c_game_statborg__adjust_player_stat_hook);
    insert_hook(0xC8ACE, 0xC8AF3, game_engine_end_round_with_winner_hook1);
    insert_hook(0xC8C18, 0xC8C3D, game_engine_end_round_with_winner_hook2);
    insert_hook(0xFAFDB, 0xFB000, game_engine_earn_wp_event_hook);
    // TODO: other inlined instances of c_game_statborg::adjust_player_stat
    //c_game_statborg::record_kill // this call from ms23 is gone entirely in ms29, not even inlined
    //game_engine_adjust_player_wp // survival only
    //metagame_earn_wp_event_maybe // survival only
    //sub_9DBC70 // infection related, c_infection_engine::update which calls this is empty in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_left
    // c_game_statborg::adjust_team_stat
    Hook(0x1AF710, adjust_team_stat_hook).Apply();
    insert_hook(0xC8A5F, 0xC8A66, game_engine_end_round_with_winner_hook3);
    insert_hook(0x1C7FC4, 0x1C7FD2, c_game_engine__recompute_team_score_hook, _hook_execute_replaced_last);
    // TODO: c_territories_engine::unknown has several c_game_statborg::adjust_team_stat calls
    // c_game_statborg::player_changed_teams
    insert_hook(0xFA956, 0xFA95F, player_changed_teams_hook);
    // game_engine_player_indices_swapped > c_game_statborg::player_indices_swapped (inlined)
    insert_hook(0xFA7B1, 0xFA7B8, player_indices_swapped_hook); // TODO: TEST - WHERE IS THIS USED?
    // c_game_statborg::stats_reset_for_round_switch
    Hook(0x1AEE00, stats_reset_for_round_switch_hook).Apply();
    // TODO: c_game_statborg::reset_player_stat for infection
    // TODO: c_game_statborg::reset_team_stat for territories

    // GLOBALS UPDATES
    // pre-game camera countdown
    Hook(0xC6C00, game_engine_update_round_conditions_hook).Apply();
    // round timer - why isn't this also causing x minutes remaining events to generate?
    Pointer::Base(0xC98CB).WriteJump(game_engine_update_time_hook, HookFlags::None);
    // sync game end & podium
    insert_hook(0xCA265, 0xCA26C, game_engine_update_after_game_hook2);

    // OBJECT CREATION
    // create & update player biped on spawn (player_spawn)
    Hook(0xBB084, player_set_facing_player_spawn_hook, HookFlags::IsCall).Apply();
    // map variant object spawning
    Hook(0xAEA03, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // c_map_variant::create_object
    Hook(0x172D86, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // c_candy_spawner:spawn_object
    Hook(0x4095BB, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // object_new_from_scenario_internal
    // effect object spawning
    Pointer::Base(0x114A58).WriteJump(event_generate_part_hook, HookFlags::None);
    Pointer::Base(0x114A6C).WriteJump(event_generate_part_hook, HookFlags::None);
    // weapon_barrel_create_projectiles
    Hook(0x4391D4, weapon_barrel_create_projectiles_hook1, HookFlags::IsCall).Apply(); // crate projectiles // hooks nearby object_new
    Pointer::Base(0x4394A2).WriteJump(weapon_barrel_create_projectiles_hook2, HookFlags::None); // standard projectiles
    // grenade & equipment throw spawning
    Pointer::Base(0x47CFBD).WriteJump(throw_release_hook1, HookFlags::None);
    Pointer::Base(0x47D174).WriteJump(throw_release_hook2, HookFlags::None);
    // hologram spawning
    Pointer::Base(0x45113A).WriteJump(equipment_activate_hook, HookFlags::None);
    // item inventory
    Pointer::Base(0x484337).WriteJump(item_in_unit_inventory_hook1, HookFlags::None);

    // OBJECT DELETION
    // add simulation_action_object_delete back to object_delete
    Hook(0x3FE1BE, object_scripting_clear_all_function_variables_hook, HookFlags::IsCall).Apply();
    // item inventory
    Pointer::Base(0x484186).WriteJump(item_in_unit_inventory_hook2, HookFlags::None);

    // OBJECT UPDATES
    // add simulation_action_object_update back to object_update
    Pointer::Base(0x404907).WriteJump(object_update_hook, HookFlags::None);
    Pointer::Base(0x4047B4).WriteJump(object_update_part, HookFlags::None);
    // add simulation_action_object_update back to player_set_facing
    Hook(0xB6300, player_set_facing_hook).Apply();
    // c_map_variant::remove_object - should fix map variant object respawn times
    Pointer::Base(0xADB2B).WriteJump(c_map_variant__remove_object_hook, HookFlags::None);
    // c_map_variant::unknown4 - called when objects spawn/respawn on sandtrap's elephants
    Pointer::Base(0xABA7E).WriteJump(c_map_variant__unknown4_hook1, HookFlags::None); // UNTESTED!!
    Pointer::Base(0xABAA3).WriteJump(c_map_variant__unknown4_hook2, HookFlags::None); // UNTESTED!!
    // player_set_unit_index
    Hook(0xB5F8E, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    Hook(0xB60E4, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    Pointer::Base(0xB6289).WriteJump(player_set_unit_index_hook2, HookFlags::None);
    // player_increment_control_context - fixes player control after respawning
    Hook(0xB9A90, player_increment_control_context).Apply();
    // unit_died - sync unit deaths
    Pointer::Base(0x421469).WriteJump(unit_died_hook, HookFlags::None);
    // sync grenade count after throw
    Pointer::Base(0x47D42F).WriteJump(grenade_throw_move_to_hand_hook, HookFlags::None);
    // sync grenade pickups
    Pointer::Base(0x4243D8).WriteJump(unit_add_grenade_to_inventory_hook, HookFlags::None);
    // sync equipment pickup
    Pointer::Base(0x424586).WriteJump(unit_add_equipment_to_inventory_hook, HookFlags::None);
    // unit_update_control
    Pointer::Base(0x41854A).WriteJump(unit_update_control_hook1, HookFlags::None); // UNTESTED!!
    Pointer::Base(0x41868D).WriteJump(unit_update_control_hook2, HookFlags::None); // called for units with flag bit 2 set
    Pointer::Base(0x418A73).WriteJump(unit_update_control_hook3, HookFlags::None); // sets aim & look vectors for controlled units - ie driving vehicles
    // unit_add_initial_loadout
    Pointer::Base(0xFB6E3).WriteJump(unit_add_initial_loadout_hook0, HookFlags::None); // create a variable to preserve player_object_index in for the 2 hooks below
    Pointer::Base(0xFBA34).WriteJump(unit_add_initial_loadout_hook1, HookFlags::None); 
    Pointer::Base(0xFBAD9).WriteJump(unit_add_initial_loadout_hook2, HookFlags::None); // used to sync the revenge_shield_boost modifier shield bonus
    // projectile_attach - prevents plasma nades from appearing like they can be picked up when stuck to a player
    Pointer::Base(0x467F11).WriteJump(projectile_attach_hook, HookFlags::None);
    // rewrite biped_update_melee_turning w/ updates
    Hook(0x440E30, biped_update_melee_turning).Apply();
    // unit_control
    Hook(0x2BB82, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD4E7, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD573, unit_control, HookFlags::IsCall).Apply();
    Hook(0x181410, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69BB96, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69DA76, unit_control, HookFlags::IsCall).Apply();
    // unit_set_aiming_vectors
    Hook(0x42A490, unit_set_aiming_vectors).Apply(); // UNTESTED!! called by c_game_engine::apply_player_update & player_teleport_on_bsp_switch
    Patch::NopFill(Pointer::Base(0x1C9AF5), 3);// remove push 4 after original call to convert usercall to fastcall
    Pointer::Base(0xB911B).Write<byte>(0x20); // 0x24 to 0x20 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    Pointer::Base(0xB911E).Write<byte>(0x10); // 0x14 to 0x10 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    Pointer::Base(0x59EFF).WriteJump(unit_set_aiming_vectors_hook1, HookFlags::None); // UNTESTED!! c_simulation_unit_entity_definition::apply_object_update
    Pointer::Base(0x610C9).WriteJump(unit_set_aiming_vectors_hook2, HookFlags::None); // handles recoil - c_simulation_weapon_fire_event_definition::apply_object_update
    Pointer::Base(0x4A0FCB).WriteJump(unit_set_aiming_vectors_hook3, HookFlags::None); // auto turret aiming direction, but not facing? - c_vehicle_auto_turret::control
    Pointer::Base(0x60B68).Write<byte>(0xD8); // expand variable space by 4 bytes to create a new variable (0xD4 to 0xD8)
    Pointer::Base(0x60BDD).WriteJump(c_simulation_weapon_fire_event_definition__apply_object_update_hook1, HookFlags::None); // set new variable to -1
    Pointer::Base(0x60C6C).WriteJump(c_simulation_weapon_fire_event_definition__apply_object_update_hook2, HookFlags::None); // preserve unit_index in our new variable for unit_set_aiming_vectors_hook2 to use
    // equipment_activate
    Pointer::Base(0x4514DC).WriteJump(equipment_activate_hook2, HookFlags::None); // TODO: this doesn't seem to work the way I expect it to - deployable covers still don't activate for clients
    // sync unit energy levels
    Pointer::Base(0x41B600).WriteJump(unit_update_energy_hook, HookFlags::None);
    // sync energy costs / energy levels decreasing when throwing an equipment
    Pointer::Base(0x42D29C).WriteJump(equipment_handle_energy_cost_hook0, HookFlags::None);
    Pointer::Base(0x42D398).WriteJump(equipment_handle_energy_cost_hook1, HookFlags::None);
    Pointer::Base(0x42D3ED).WriteJump(equipment_handle_energy_cost_hook2, HookFlags::None); // includes player update
    // unit_set_hologram
    Pointer::Base(0x42C56E).WriteJump(unit_set_hologram_hook, HookFlags::None);

    // OBJECT PHYSICS UPDATES
    // object_set_position_internal
    insert_hook(0x3FC038, 0x3FC03E, object_set_position_internal_hook1);
    Pointer::Base(0x3FC060).WriteJump(object_set_position_internal_hook2, HookFlags::None);
    Pointer::Base(0x404AB4).WriteJump(object_set_position_internal_hook3, HookFlags::None); // inlined call in object_move_respond_to_physics
    Pointer::Base(0x404AD7).WriteJump(object_set_position_internal_hook4, HookFlags::None); // inlined call in object_move_respond_to_physics
    // object_set_velocities_internal
    Pointer::Base(0x3FC808).WriteJump(object_apply_acceleration_hook, HookFlags::None); // inlined call in object_apply_acceleration
    Hook(0x3FC500, object_set_velocities_internal_hook).Apply();
    // object_set_at_rest
    Hook(0x4011F0, object_set_at_rest).Apply(); // add updates back to original call
    // hook nearby object_wake calls in inlined object_set_at_rest instances to add back sim updates
    Hook(0x6BBB8, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // c_simulation_object_entity_definition::object_apply_update
    Hook(0x773D6, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // c_simulation_generic_entity_definition::handle_delete_object
    Hook(0xC82A1, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // garbage_collect_multiplayer
    Hook(0x172CB3, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // c_candy_spawner::spawn_object
    Hook(0x1282B8, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // c_havok_component::wake_all_bodies_in_phantoms
    Hook(0x3FBF35, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // object_reset
    Hook(0x419397, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // unit_fix_position
    Hook(0x413DD3, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // damage_response_fire
    Hook(0x415DC3, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // object_damage_constraints
    Hook(0x43DAF6, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // biped_update_without_parent
    Hook(0x4632B7, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // projectile_accelerate
    Hook(0x456EB5, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // motor_animation_exit_seat_immediate_internal
    Hook(0x1EF12D, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // object_wake_physics - inlined into object_wake_physics_evaluate w/ object_wake call
    Hook(0x46D33E, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // object_early_mover_delete
    Hook(0x4848B8, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // item_multiplayer_at_rest_state_initialize 
    Hook(0x4980E3, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // biped_stun_submit
    Hook(0x4A9126, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // c_vehicle_type_mantis::update_physics
    Hook(0x4A4300, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // biped_dead_force_airborne
    Hook(0x4A1F00, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // biped_exit_relaxation
    Hook(0x4A2396, object_set_at_rest_hook, HookFlags::IsCall).Apply(); // biped_start_relaxation
    // more inlined object_set_at_rest instances
    Pointer::Base(0x77142).WriteJump(object_set_at_rest_hook2, HookFlags::None); // UNTESTED!! // c_simulation_generic_entity_definition::object_required_to_join_game
    Pointer::Base(0x725A8).WriteJump(object_set_at_rest_hook3, HookFlags::None); // UNTESTED!! // c_simulation_vehicle_entity_definition::create_object
    Pointer::Base(0x400AD1).WriteJump(object_set_at_rest_hook4, HookFlags::None); // object_attach_to_node_immediate
    Pointer::Base(0x467DE7).WriteJump(object_set_at_rest_hook5, HookFlags::None); // projectile_attach
    Pointer::Base(0x464EA0).WriteJump(object_set_at_rest_hook6, HookFlags::None); // projectile_collision
    Pointer::Base(0x462431).WriteJump(object_set_at_rest_hook7, HookFlags::None); // projectile_initial_update
    Pointer::Base(0x46D281).WriteJump(object_set_at_rest_hook8, HookFlags::None); // UNTESTED!! // object_early_mover_delete
    Pointer::Base(0x6D50EF).WriteJump(object_set_at_rest_hook9, HookFlags::None); // UNTESTED!! // swarm_accelerate > creature_accelerate inlined > object_set_at_rest inlined
    Hook(0x48FB9A, object_set_at_rest_hook10, HookFlags::IsCall).Apply(); // scenery_new, hooked scenery_animation_idle call
    Pointer::Base(0x4BEBB7).WriteJump(object_set_at_rest_hook11, HookFlags::None); // vehicle_program_activate
    Pointer::Base(0x4BFF66).WriteJump(object_set_at_rest_hook12, HookFlags::None); // UNTESTED!! // vehicle_program_update
    Pointer::Base(0x4BFF76).WriteJump(object_set_at_rest_hook12, HookFlags::None); // UNTESTED!! // vehicle_program_update
    Hook(0x4C7A66, object_set_at_rest_hook13, HookFlags::IsCall).Apply(); // unit_custom_animation_play_animation_submit // plays on podium

    // OBJECT DAMAGE UPDATES
    // object_damage_update
    insert_hook(0x40D4CE, 0x40D553, object_damage_update_hook1, _hook_replace_no_nop); // same update as below, compiled logic requires hooks in 2 separate places
    insert_hook(0x40D526, 0x40D542, object_damage_update_hook3, _hook_replace, true); // same update as above, compiled logic requires hooks in 2 separate places
    insert_hook(0x40D5CB, 0x40D660, object_damage_update_hook2, _hook_replace, true);
    // object_damage_shield
    insert_hook(0x41268C, 0x412694, object_damage_shield_hook1); // shield vamparism trait
    insert_hook(0x41287B, 0x412881, object_damage_shield_hook2, _hook_execute_replaced_first); // if this runs first, object_index should be in esi
    // object_damage_body
    insert_hook(0x411E2B, 0x411E37, object_damage_body_hook1, _hook_execute_replaced_first, true);
    // object_deplete_body_internal
    insert_hook(0x40D9D5, 0x40D9DA, object_deplete_body_internal_hook1, _hook_execute_replaced_last);
    // damage_response_fire
    insert_hook(0x413D43, 0x413D50, damage_response_fire_hook);
    // object_set_damage_owner
    insert_hook(0x404323, 0x404393, object_set_damage_owner_hook, _hook_replace);
    insert_hook(0x113B0F, 0x113B15, object_set_damage_owner_hook2); // inlined in event_generate_accelerations
    insert_hook(0x20CF07, 0x20CF0D, object_set_damage_owner_hook3); // inlined in havok_collision_damage_update
    insert_hook(0x40F00C, 0x40F012, object_set_damage_owner_hook4); // inlined in object_cause_damage
    insert_hook(0x4572A5, 0x4572AB, object_set_damage_owner_hook5); // inlined in motor_animation_exit_seat_immediate_internal
    insert_hook(0x4BEA5D, 0x4BEA63, object_set_damage_owner_hook6); // inlined in vehicle_flip_submit

    // WEAPON STATE UPDATES
    // weapon_age
    insert_hook(0x433CE6, 0x433CF0, weapon_age_hook, _hook_replace);
    // weapon_barrel_fire
    insert_hook(0x43577A, 0x43577F, weapon_barrel_fire_hook);
    // weapon_magazine_execute_reload
    insert_hook(0x434ECE, 0x434ED6, weapon_magazine_execute_reload_hook);
    // weapon_magazine_update (syncs Weapon > Magazines > RoundsRecharged)
    insert_hook(0x42DBB4, 0x42DBB9, weapon_magazine_update_hook);
    // weapon_report_kill (syncs weapon ages with each kill flag, used by the energy sword)
    insert_hook(0x4339C5, 0x4339CA, weapon_report_kill_hook);
    // weapon_set_current_amount (used by scripts & actors only, TODO: untested)
    insert_hook(0x43374B, 0x433750, weapon_set_current_amount_hook);
    // weapon_set_total_rounds
    insert_hook(0x433479, 0x43347F, weapon_set_total_rounds_hook, _hook_execute_replaced_last);
    // weapon_take_inventory_rounds 2x
    insert_hook(0x432147, 0x43214E, weapon_take_inventory_rounds_hook1);
    insert_hook(0x4321E1, 0x4321E8, weapon_take_inventory_rounds_hook2);
    // weapon_trigger_update
    insert_hook(0x42E055, 0x42E05A, weapon_trigger_update_hook);

    // ammo pickup
    Pointer::Base(0x4310CC).WriteJump(weapon_handle_potential_inventory_item_hook, HookFlags::None);
    // weapon set index updates
    Pointer::Base(0x426D10).WriteJump(unit_inventory_set_weapon_index_hook0, HookFlags::None);
    Pointer::Base(0x426D8E).WriteJump(unit_inventory_set_weapon_index_hook1, HookFlags::None);
    // weapon set identifier updates
    Hook(0x426CC0, unit_inventory_cycle_weapon_set_identifier, HookFlags::IsCall).Apply();
    // add inlined unit_inventory_set_weapon_index with sim updates back to unit_delete_all_weapons_internal
    Hook(0x424E60, unit_delete_all_weapons_internal).Apply();
    // add inlined unit_inventory_set_weapon_index with sim updates back to unit_handle_deleted_object
    Pointer::Base(0x427119).WriteJump(unit_handle_deleted_object_hook, HookFlags::None);

    // PLAYER UPDATES
    // add back simulation_action_game_engine_player_update to player_spawn
    Pointer::Base(0xBB093).WriteJump(player_spawn_hook, HookFlags::None);
    // game_engine_player_set_spawn_timer
    Hook(0xFA07F, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();
    Hook(0xFBC5B, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0xFBF43).WriteJump(game_engine_player_set_spawn_timer_hook, HookFlags::None);
    Pointer::Base(0xFBF4F).WriteJump(game_engine_player_set_spawn_timer_hook, HookFlags::None);
    Hook(0xFC01C, game_engine_player_set_spawn_timer_hook, HookFlags::IsCall).Apply();
    // add more player updates back to player_spawn - syncs player data info? TODO: investigate
    Pointer::Base(0xBB435).WriteJump(player_spawn_hook2, HookFlags::None);
    Pointer::Base(0xBB459).WriteJump(player_spawn_hook3, HookFlags::None);
    // add simulation_action_game_engine_player_update back to c_simulation_player_respawn_request_event_definition::apply_game_event
    Hook(0x68B40, c_simulation_player_respawn_request_event_definition__apply_game_event).Apply(); // I'm not sure what this request is used for, something spectator?
    // add player update back to hf2p function - TODO fix crashing
    //Patch(0xBAF22, { 0x89, 0xF1 }).Apply(); // replace player_data parameter with player_index  mov ecx,esi in call
    //Pointer::Base(0xBAF24).WriteJump(hf2p_loadout_update_active_character_call_hook, HookFlags::None);
    //Hook(0xE0660, hf2p_loadout_update_active_character_hook).Apply();
    
    // MISC HOOKS
    // output the message type for debugging
    Hook(0x16AF8, send_message_hook, HookFlags::IsCall).Apply();
    Hook(0x16C26, send_message_hook, HookFlags::IsCall).Apply();
    Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    // hook net_debug_print's vsnprintf_s call to print API logs to the console - temporarily disabled due to crashes
    //Hook(0x55D8BF, vsnprintf_s_net_debug_hook, HookFlags::IsCall).Apply();
    // contrail gpu freeze fix - twister
    Hook(0x28A38A, contrail_fix_hook).Apply();
    // temporary test to force elite ui model on mainmenu
    //Hook(0x2059B0, ui_get_player_model_id_evaluate_hook).Apply();
    // podium animation testing
    Hook(0x2E8750, hf2p_player_podium_initialize_hook).Apply();
    // podium taunt triggering & syncing
    insert_hook(0x2E9C3A, 0x2E9C3F, hf2p_podium_tick_hook);
    // disable build watermark text
    Hook(0x1B0AB0, draw_watermark_hook).Apply();
}