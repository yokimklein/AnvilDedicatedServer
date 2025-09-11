#include "hooks_miscellaneous.h"
#include <anvil\hooks\hooks.h>
#include <cseries\cseries.h>
#include <cseries\cseries_windows_debug_pc.h>
#include <config\version.h>
#include <networking\logic\network_life_cycle.h>
#include <networking\messages\network_message_gateway.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\session\network_session.h>
#include <hf2p\podium.h>
#include <game\game.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <hf2p\loadouts.h>
#include <tag_files\string_ids.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winnt.h>
#include <anvil\backend\uri_map.h>
#include <game\game_engine_display.h>
#include <cseries\cseries_events.h>

void __cdecl hf2p_podium_tick_hook(s_hook_registers registers)
{
    long player_index = (long)registers.esi;

    hf2p_trigger_player_podium_taunt(player_index);
}

void __cdecl c_simulation_player_taunt_request_event_definition__apply_game_event_hook(s_hook_registers registers)
{
    long player_index = (long)registers.esi;

    if (game_is_authoritative())
    {
        simulation_action_player_taunt_request((word)player_index);
    }
}

long __cdecl exceptions_update_hook()
{
    PEXCEPTION_POINTERS g_exception_param_exception_pointers = *base_address<PEXCEPTION_POINTERS*>(0x106DEC8);
    if (g_exception_param_exception_pointers && g_exception_param_exception_pointers->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
    {
        return 0;
    }

    return exceptions_update();
}

c_static_string<64>* __cdecl c_static_string_64_print_hook(c_static_string<64>* static_string, char const* format, ...)
{
    static_string->print("Halo Online " VERSION_WINDOW_NAME " %s", version_get_window_version());
    return static_string;
}

// print to console whenever a message packet is sent - this pointer is unused
// c_network_message_type_collection::encode_message_header
void __fastcall encode_message_header_hook(c_network_message_type_collection* _this, void*, c_bitstream* stream, e_network_message_type message_type, long message_storage_size)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();

    // const cast is kind of gross but I'll justify it here as we're currently within a method of c_network_message_type_collection and we need a replacement thisptr
    c_network_message_type_collection* message_type_collection = const_cast<c_network_message_type_collection*>(session->message_gateway()->message_types());

    event(_event_verbose, "networking:messages:send: %s (%d bytes)", message_type_collection->get_message_type_name(message_type), message_storage_size);
    DECLFUNC(0x387A0, void, __thiscall, c_network_message_type_collection*, c_bitstream*, e_network_message_type, long)(message_type_collection, stream, message_type, message_storage_size);
}

// disable contrails to prevent gpu freezing - TODO: fix this properly
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

//long __cdecl ui_get_player_model_id_evaluate_hook(long a1, long a2)
//{
//    FUNCTION_DEF(0x1210F0, long, __fastcall, hs_return, long a1, long a2);
//    return hs_return(a2, 1);
//}

int __cdecl vsnprintf_s_net_debug_hook(char* DstBuf, size_t SizeInBytes, size_t MaxCount, const char* Format, va_list ArgList)
{
    // original function call
    int result = vsnprintf_s(DstBuf, SizeInBytes, MaxCount, Format, ArgList);

    // deobfuscate URIs in request and response prints
    c_static_string<0x100> resource_uri;
    if (strcmp(Format, "Request %s") == 0)
    {
        resource_uri.set(&DstBuf[8]);
        backend_deobfuscate_uri(resource_uri.get_buffer(), SizeInBytes);
        event(_event_verbose, "backend:saber request %s", resource_uri.get_buffer());
    }
    else if (strcmp(Format, "Response %s [%d|%d]") == 0)
    {
        resource_uri.set(&DstBuf[9]);

        long end_index = resource_uri.index_of("[");
        resource_uri.get_buffer()[end_index - 1] = 0;

        backend_deobfuscate_uri(resource_uri.get_buffer(), SizeInBytes);
        event(_event_verbose, "backend:saber response %s %s", resource_uri.get_buffer(), &DstBuf[end_index + 9]);
    }
    // check if we're building a URI - we don't want to print these
    else if (strcmp(Format, "/%s.svc/%s") != 0)
    {
        event(_event_verbose, "backend:saber  %s", DstBuf);
    }

    return result;
}

#pragma runtime_checks("", off)
// fastcall which user cleans up 4 bytes
void __fastcall sub_718BF0_hook(long texture_render_index, s_backend_loadout* loadout, s_backend_customisation* user_customisation)
{
    // Check if loadout is valid before calling first
    // If a player kills the local player and no API loadout information for the killer exists, the pointer is null and can crash
    if (!loadout || !user_customisation)
    {
        return;
    }

    // texture_render_index is Bitmaps[].Index in texture_render_list tag

    INVOKE(0x318BF0, sub_718BF0_hook, texture_render_index, loadout, user_customisation);
    __asm add esp, 4; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

void anvil_hooks_miscellaneous_apply()
{
    // hook exceptions_update to catch esoteric crashes
    hook::call(0x95C0F, exceptions_update_hook);
    hook::call(0x98BCB, exceptions_update_hook);
    hook::call(0x9AB6B, exceptions_update_hook);
    hook::call(0x9ABAB, exceptions_update_hook);
    hook::call(0x9EFE3, exceptions_update_hook);
    hook::call(0x9F07C, exceptions_update_hook);
    hook::call(0x9F8B1, exceptions_update_hook);
    hook::call(0xC3E8B, exceptions_update_hook);
    hook::call(0xDCCBD, exceptions_update_hook);
    hook::call(0x16A63B, exceptions_update_hook);
    hook::call(0x17C5FE, exceptions_update_hook);
    hook::call(0x3DBB5B, exceptions_update_hook);

    // hook game window text to display "Dedicated Server" / "Game Server" instead of "Game Client"
    hook::call(0x13C3, c_static_string_64_print_hook);

    // output the message type for debugging
    hook::call(0x16AF8, encode_message_header_hook);
    hook::call(0x16C26, encode_message_header_hook);
    hook::call(0x233D4, encode_message_header_hook);

    // contrail gpu freeze 'fix' - twister
    hook::function(0x28A38A, 5, contrail_fix_hook);

    // temporary test to force elite ui model on mainmenu
    //hook::function(0x2059B0, 0x24, ui_get_player_model_id_evaluate_hook);
    
    // podium animation testing
    hook::function(0x2E8750, 0xB2, hf2p_player_podium_initialize);

    // podium taunt triggering & syncing
    hook::insert(0x2E9C3A, 0x2E9C3F, hf2p_podium_tick_hook, _hook_execute_replaced_first);
    hook::insert(0x68CDC, 0x68CEE, c_simulation_player_taunt_request_event_definition__apply_game_event_hook, _hook_execute_replaced_first, true);

    // hook watermark
    hook::function(0x1B0AB0, 0x5CF, game_engine_render_watermarks);

    // hook net_debug_print's vsnprintf_s call to print API logs to the console
    hook::call(0x55D8BF, vsnprintf_s_net_debug_hook);
    
    // $TODO: may not be required with backend being disabled?
    // $TODO: why is this happening? Are we missing data which the clients need?
    // Fix host crashing when killed by a player when not connected to the API
    hook::call(0x33B1E0, sub_718BF0_hook);
    patch::nop_region(0x33B1E5, 3);
    hook::call(0x33B2B0, sub_718BF0_hook);
    patch::nop_region(0x33B2B5, 3);
    hook::call(0x33B33A, sub_718BF0_hook);
    patch::bytes(0x33B341, { 0x08 });
    hook::call(0x33B3BA, sub_718BF0_hook);
    patch::bytes(0x33B3C1, { 0x08 });
    hook::call(0x33B43A, sub_718BF0_hook);
    patch::bytes(0x33B441, { 0x08 });
    hook::call(0x33B4B7, sub_718BF0_hook);
    patch::bytes(0x33B4BE, { 0x08 });
    hook::call(0x33B53A, sub_718BF0_hook);
    patch::bytes(0x33B541, { 0x08 });
    hook::call(0x33B5BA, sub_718BF0_hook);
    patch::bytes(0x33B5C1, { 0x08 });
    hook::call(0x33B63A, sub_718BF0_hook);
    patch::bytes(0x33B641, { 0x08 });
    hook::call(0x33B6B8, sub_718BF0_hook);
    patch::nop_region(0x33B6BD, 3);
    hook::call(0x33B6D3, sub_718BF0_hook);
    patch::nop_region(0x33B6D8, 3);
    hook::call(0x33B6EE, sub_718BF0_hook);
    patch::nop_region(0x33B6F3, 3);

    // load string ids
    hook::insert(0x110C, 0x1111, string_id_initialize, _hook_execute_replaced_first);
}