#include "hooks_session.h"
#include <anvil\hooks\hooks.h>
#include <anvil\server_tools.h>
#include <Patch.hpp>
#include <networking\messages\network_message_handler.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_session_interface.h>
#include <networking\logic\network_life_cycle.h>
#include <networking\session\network_managed_session.h>
#include <networking\transport\transport_shim.h>
#include <game\game_engine_spawning.h>

// runtime checks need to be disabled for these, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)

#pragma runtime_checks("", restore)

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* stub_message)
{
    s_network_message const* message = (s_network_message const*)BASE_ADDRESS(0x4FFB090);
    message_handler->handle_channel_message(channel, message_type, message_storage_size, message);
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

// reimplement network_session_check_properties by calling it at the end of network_session_interface_update_session
void __fastcall network_session_interface_update_session_hook(c_network_session* session)
{
    network_session_interface_update_session(session);

    if (session->established() && !session->leaving_session())
    {
        if (session->is_host())
        {
            network_session_check_properties(session);
        }
    }
}

void __fastcall managed_session_delete_session_internal_hook(long managed_session_index, c_managed_session* managed_session)
{
    if (managed_session->flags.test(_online_managed_session_created_bit) && managed_session->session_class == _network_session_class_online)
        XNetUnregisterKey(&transport_security_globals.address);

    managed_session_delete_session_internal(managed_session_index, managed_session);
}

void anvil_hooks_session_apply()
{
    // add back missing host code by replacing existing stripped down functions
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    Hook(0x21AB0, session_idle_hook).Apply();
    // I couldn't directly hook peer_request_properties_update without experiencing access violations, so this will do
    // add back set_peer_address & set_peer_properties to peer_request_properties_update
    Hook(0x2F650, network_session_update_peer_properties).Apply();
    Hook(0x3EEE1F, network_life_cycle_end_hook, HookFlags::IsCall).Apply();
    // add back network_session_check_properties
    Hook(0x2AD9E, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    Hook(0x2DC71, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    // unregister the host address description to the xnet shim table on session destruction - the transport_secure_key_create hook further down handles session creation
    Hook(0x21342, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Hook(0x28051, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0x284B8).WriteJump(managed_session_delete_session_internal_hook, HookFlags::None);
    // hook game_engine_should_spawn_player so we can control the pregame spawn countdown
    Hook(0xFBBA0, game_engine_should_spawn_player).Apply();
}