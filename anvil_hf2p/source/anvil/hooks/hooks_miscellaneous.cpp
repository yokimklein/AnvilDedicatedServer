#include "hooks_miscellaneous.h"
#include <anvil\hooks\hooks.h>
#include <cseries\cseries.h>
#include <cseries\cseries_windows_debug_pc.h>
#include <cseries\version.h>
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
#include <interface\user_interface_session.h>

#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
void __cdecl hf2p_podium_tick_hook(s_hook_registers& registers)
{
    long player_index = (long)registers.esi;

    hf2p_trigger_player_podium_taunt(player_index);
}
#endif

void __cdecl c_simulation_player_taunt_request_event_definition__apply_game_event_hook(s_hook_registers& registers)
{
    long player_index = (long)registers.esi;

    if (game_is_authoritative())
    {
        simulation_action_player_taunt_request((word)player_index);
    }
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
    DECLFUNC(ADDRESS_ENCODE_MESSAGE_HEADER_HOOK, void, __thiscall, c_network_message_type_collection*, c_bitstream*, e_network_message_type, long)(message_type_collection, stream, message_type, message_storage_size);
}

// disable contrails to prevent gpu freezing - $TODO: fix this properly
// $NOTE: This doesn't account for ASLR so surely this breaks??
__declspec(naked) void contrail_fix_hook()
{
    __asm
    {
        add edx, [ADDRESS_CONTRAIL_FIX_OPERAND_VA]
        cmp edx, -1
        jg render
        push ADDRESS_CONTRAIL_FIX_SKIP_VA
        retn
        render:
        push ADDRESS_CONTRAIL_FIX_RENDER_VA
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

#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
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

    INVOKE(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA, sub_718BF0_hook, texture_render_index, loadout, user_customisation);
    __asm add esp, 4; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
// ms30 reworked hf2p_set_biped_texture_render_data into a usercall taking the texture name in ecx, a string in edx & the loadout and customisation on the stack, which the caller cleans up
// it still dereferences the loadout & customisation without checking them, so skip the call when either is null
static size_t const k_hf2p_set_biped_texture_render_data_call_addresses[]
{
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_1,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_2,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_3,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_4,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_5,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_6,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_7,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_8,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_9,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_10,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_11,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_12,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_13,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_14,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_15,
    ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_16,
};
static size_t hf2p_set_biped_texture_render_data_address = 0;
__declspec(naked) void hf2p_set_biped_texture_render_data_hook()
{
    __asm
    {
        cmp dword ptr [esp + 4], 0 // loadout
        jz skip
        cmp dword ptr [esp + 8], 0 // customisation
        jz skip
        jmp dword ptr [hf2p_set_biped_texture_render_data_address] // registers & stack are untouched, the original function returns to our caller
    skip:
        retn
    }
}
#endif

void __cdecl sub_319CE0_hook(s_hook_registers& registers)
{
    user_interface_join_squad_abort();
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (session && !session->disconnected())
    {
        session->leave_session_and_disconnect();
    }
}

void anvil_hooks_miscellaneous_apply()
{
    // hook game window text to display "Dedicated Server" / "Game Server" instead of "Game Client"
    hook::call(ADDRESS_C_STATIC_STRING_64_PRINT_CALL, c_static_string_64_print_hook);

    // output the message type for debugging
    hook::call(ADDRESS_ENCODE_MESSAGE_HEADER_CALL, encode_message_header_hook);
    hook::call(ADDRESS_ENCODE_MESSAGE_HEADER_CALL_2, encode_message_header_hook);
    hook::call(ADDRESS_ENCODE_MESSAGE_HEADER_CALL_3, encode_message_header_hook);

    // contrail gpu freeze 'fix' - twister
    //hook::function(0x28A38A, 5, contrail_fix_hook);

    // temporary test to force elite ui model on mainmenu
    //hook::function(0x2059B0, 0x24, ui_get_player_model_id_evaluate_hook);
    
    // podium animation testing
    hook::function(ADDRESS_HF2P_PLAYER_PODIUM_INITIALIZE, 0xB2, hf2p_player_podium_initialize);

    // podium taunt triggering & syncing
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    // ms30 implements podium taunts itself (triggered with the space bar)
    hook::insert(ADDRESS_HF2P_PODIUM_TICK_HOOK, ADDRESS_HF2P_PODIUM_TICK_HOOK_RETURN, hf2p_podium_tick_hook, _hook_execute_replaced_first);
#endif
    hook::insert(ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK, ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK_RETURN, c_simulation_player_taunt_request_event_definition__apply_game_event_hook, _hook_execute_replaced_first, 0, true);

    // hook watermark
    hook::function(ADDRESS_GAME_ENGINE_RENDER_WATERMARKS, 0x5CF, game_engine_render_watermarks);

    // hook net_debug_print's vsnprintf_s call to print API logs to the console
    hook::call(ADDRESS_VSNPRINTF_S_NET_DEBUG_CALL, vsnprintf_s_net_debug_hook);
    
    // $TODO: may not be required with backend being disabled?
    // $TODO: why is this happening? Are we missing data which the clients need?
    // Fix host crashing when killed by a player when not connected to the API
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_1, sub_718BF0_hook);
    patch::nop_region(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_1, 3);
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_2, sub_718BF0_hook);
    patch::nop_region(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_2, 3);
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_3, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_3, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_4, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_4, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_5, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_5, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_6, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_6, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_7, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_7, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_8, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_8, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_9, sub_718BF0_hook);
    patch::bytes(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_9, { 0x08 });
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_10, sub_718BF0_hook);
    patch::nop_region(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_10, 3);
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_11, sub_718BF0_hook);
    patch::nop_region(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_11, 3);
    hook::call(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_12, sub_718BF0_hook);
    patch::nop_region(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_12, 3);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    hf2p_set_biped_texture_render_data_address = base_address(ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA);
    for (size_t call_address : k_hf2p_set_biped_texture_render_data_call_addresses)
    {
        hook::call(call_address, hf2p_set_biped_texture_render_data_hook);
    }
#endif

    // load string ids
    hook::insert(ADDRESS_STRING_ID_INITIALIZE, ADDRESS_STRING_ID_INITIALIZE_RETURN, string_id_initialize, _hook_execute_replaced_first);

    // leave sessions gracefully instead of force disconnecting
    hook::insert(ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK, ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK_RETURN, sub_319CE0_hook, _hook_replace);
}