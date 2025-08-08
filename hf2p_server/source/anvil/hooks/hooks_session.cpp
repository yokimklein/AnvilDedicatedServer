#include "hooks_session.h"
#include <anvil\hooks\hooks.h>
#include <Patch.hpp>
#include <networking\messages\network_message_handler.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_session_interface.h>
#include <networking\logic\network_life_cycle.h>
#include <networking\session\network_managed_session.h>
#include <networking\transport\transport_shim.h>
#include <game\game_engine_spawning.h>

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, transport_address const* address, e_network_message_type message_type, long unused2, const void* message)
{
    // message_storage_size is unused and just set to the same value as message_handler
    // $TODO: Fix up message_storage_size arg, it exists at [ebp + 0x0C] prior to the function call
    message_handler->handle_out_of_band_message(address, message_type, 0, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, const void* stub_message)
{
    const void* message = base_address<const void*>(0x4FFB090);
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

void __fastcall managed_session_delete_session_internal_hook(long managed_session_index, s_online_managed_session* managed_session)
{
    if (managed_session->flags.test(_online_managed_session_created_bit) && managed_session->session_class == _network_session_class_online)
        XNetUnregisterKey(&transport_security_globals.address);

    managed_session_delete_session_internal(managed_session_index, managed_session);
}

void __fastcall can_accept_player_join_request_hook(c_network_session* thisptr, void* unused, s_player_identifier* player_identifier, s_transport_secure_address* secure_address, long peer_index, bool unknown)
{
    thisptr->can_accept_player_join_request(player_identifier, secure_address, peer_index, unknown);
}

void __fastcall session_disconnect_hook(c_network_session* thisptr)
{
    thisptr->disconnect();
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

    // add debug print back to before life cycle end is called in c_gui_location_manager::update
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

    // TODO: REWRITE CALL FOR THIS, FUNCTION IS MISSING ARGUMENTS PASSED INTO IT
    // hook can_accept_player_join_request to reimplement dedicated server userid check
    //Hook(0x22F30, can_accept_player_join_request_hook).Apply();

    // hook c_network_session::disconnect to add call to clear lobby info
    Hook(0x421B29, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x421B6A, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x421CB4, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x4225FE, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x42440E, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x4256D4, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x425723, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x42AC3C, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x42AD3D, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x43E36F, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x43E8A4, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x43EA40, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x43EAA9, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44B418, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44B522, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44B538, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44B5B2, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44B659, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44D14E, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x44F42F, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x711749, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x711767, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x7AA106, session_disconnect_hook, HookFlags::IsCall).Apply();
    Hook(0x7AAF2C, session_disconnect_hook, HookFlags::IsCall).Apply();
}