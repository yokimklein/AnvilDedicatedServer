#include "hooks_session.h"
#include <anvil\hooks\hooks.h>
#include <networking\messages\network_message_handler.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_session_interface.h>
#include <networking\logic\network_life_cycle.h>
#include <networking\session\network_managed_session.h>
#include <networking\transport\transport_shim.h>
#include <game\game_engine_spawning.h>
#include <networking\messages\network_message_gateway.h>
#include <cseries\cseries_events.h>

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, transport_address const* address, e_network_message_type message_type, long unused2, const void* message)
{
    // message_storage_size is unused and just set to the same value as message_handler
    // $TODO: Fix up message_storage_size arg, it exists at [ebp + 0x0C] prior to the function call
    message_handler->handle_out_of_band_message(address, message_type, 0, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, const void* stub_message)
{
    const void* message = base_address<const void*>(ADDRESS_MEMBERSHIP_UPDATE_MESSAGE);
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
    event(_event_warning, "ui:location_manager: Resetting network location.  If you got here and didn't just issue a console command, this is a bug.");
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
    {
        XNetUnregisterKey(&transport_security_globals.address);
    }
    managed_session_delete_session_internal(managed_session_index, managed_session);
}

void __fastcall can_accept_player_join_request_hook(c_network_session* thisptr, void* unused, s_player_identifier* player_identifier, s_transport_secure_address* stubbed1, long stubbed2, bool stubbed3)
{
    // add back transport secure address get which was stripped from the build
    s_transport_secure_address secure_address;
    if (transport_secure_address_get(&secure_address))
    {
        thisptr->can_accept_player_join_request(player_identifier, &secure_address, 0, false);
    }
}

void __fastcall session_disconnect_hook(c_network_session* thisptr)
{
    thisptr->disconnect();
}

void __fastcall send_all_pending_messages_hook(c_network_message_gateway* thisptr)
{
    thisptr->send_all_pending_messages();
}

void anvil_hooks_session_apply()
{
    // add back missing host code by replacing existing stripped down functions
    hook::function(ADDRESS_HANDLE_OUT_OF_BAND_MESSAGE, 0x1D4, handle_out_of_band_message_hook);
    hook::function(ADDRESS_HANDLE_CHANNEL_MESSAGE, 0x369, handle_channel_message_hook);
    hook::function(ADDRESS_NETWORK_JOIN_PROCESS_JOINS_FROM_QUEUE, 0xBC, network_join_process_joins_from_queue);
    hook::function(ADDRESS_SESSION_IDLE, 0x17C, session_idle_hook);

    // I couldn't directly hook peer_request_properties_update without experiencing access violations, so this will do
    // add back set_peer_address & set_peer_properties to peer_request_properties_update
    hook::function(ADDRESS_NETWORK_SESSION_UPDATE_PEER_PROPERTIES, 0x255, network_session_update_peer_properties);

    // add debug print back to before life cycle end is called in c_gui_location_manager::update
    hook::call(ADDRESS_NETWORK_LIFE_CYCLE_END_CALL, network_life_cycle_end_hook);

    // add back network_session_check_properties
    hook::call(ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL, network_session_interface_update_session_hook);
    hook::call(ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL_2, network_session_interface_update_session_hook);

    // unregister the host address description to the xnet shim table on session destruction - the transport_secure_key_create hook further down handles session creation
    hook::call(ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL, managed_session_delete_session_internal_hook);
    hook::call(ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL_2, managed_session_delete_session_internal_hook);
    hook::function(ADDRESS_MANAGED_SESSION_DELETE_JUMP, 5, managed_session_delete_session_internal_hook); // replace jump call

    // hook game_engine_should_spawn_player so we can control the pregame spawn countdown
    hook::function(ADDRESS_GAME_ENGINE_SHOULD_SPAWN_PLAYER, LENGTH_GAME_ENGINE_SHOULD_SPAWN_PLAYER, game_engine_should_spawn_player);

    // hook can_accept_player_join_request to reimplement dedicated server userid check
    hook::call(ADDRESS_CAN_ACCEPT_PLAYER_JOIN_REQUEST_CALL, can_accept_player_join_request_hook);

    // hook c_network_session::disconnect to add call to clear lobby info
    //hook::call(0x21B29, session_disconnect_hook); // c_network_session::idle
    //hook::call(0x21B6A, session_disconnect_hook); // c_network_session::idle
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_2, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_3, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_4, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_5, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_6, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_7, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_8, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_9, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_10, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_11, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_12, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_13, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_14, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_15, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_16, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_17, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_18, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_19, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_20, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_21, session_disconnect_hook);
    hook::call(ADDRESS_SESSION_DISCONNECT_CALL_22, session_disconnect_hook);

    // hook c_network_message_gateway::send_all_pending_messages to attempt to fix stack overflow
    hook::function(ADDRESS_SEND_ALL_PENDING_MESSAGES, 0x13F, send_all_pending_messages_hook);
}