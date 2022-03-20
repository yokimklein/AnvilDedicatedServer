#include <iostream>
#include "network_session.h"
#include "..\..\dllmain.h"
#include "..\messages\network_message_gateway.h"
#include "..\messages\network_message_handler.h"
#include "..\network_utilities.h"
#include "..\network_globals.h"

bool c_network_session::acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason)
{
    typedef char(__fastcall* managed_session_get_id_ptr)(int session_index, s_transport_secure_identifier* secure_identifier);
    auto managed_session_get_id = reinterpret_cast<managed_session_get_id_ptr>(module_base + 0x28AE0);

    const char* transport_address_string = "(null)"; // (const char*)transport_address_get_string();
    const char* join_refuse_reason_string = network_message_join_refuse_get_reason_string(reason);
    const char* managed_session_index_string = "(null)"; // managed_session_get_id_string(session[618236]);
    printf("MP/NET/SESSION,CTRL: c_network_session::acknowledge_join_request: [%s] join failed, sending refusal (%s - possibly inaccurate) to '%s'\n", managed_session_index_string, join_refuse_reason_string, transport_address_string);
    s_network_message_join_refuse message;
    managed_session_get_id(this->m_managed_session_index, &message.session_id); // returning null? client doesn't seem to listen to the response - this is likely the culprit - TODO
    message.reason = reason;
    return this->m_message_gateway->send_message_directed(address, _network_message_type_join_refuse, 20, &message);

    // TEMP HACK - TODO REMOVE
    //s_network_message_connect_request message;
    //message.identifier = 1;
    //message.flags = 0;
    //return this->m_message_gateway->send_message_directed(address, _network_message_type_connect_request, 8, &message);
}

bool c_network_session::handle_join_request(s_transport_address const* address, s_network_message_join_request const* message)
{
    int executable_type;
    int executable_version;
    int compatible_version;
    network_get_build_identifiers(&executable_type, &executable_version, &compatible_version);

    if (message->compatible_version >= compatible_version)
    {
        if (executable_version >= message->executable_version)
        {
            if (executable_type == message->executable_type)
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] received for %d peers, adding to the join queue\n",
                    "(null)",/*managed_session_get_id_string(this->m_managed_session_index)*/ // TODO
                    "(null)",/*transport_secure_nonce_get_string(message->secure_nonce)*/ // TODO
                    message->data.joining_peer_count);
                network_join_add_join_to_queue(this, address, &message->data);
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] executable type mismatch, peer: %d, host: %d\n",
                    "(null)",/*managed_session_get_id_string(this->m_managed_session_index)*/ // TODO
                    "(null)",/*transport_secure_nonce_get_string(message->secure_nonce)*/ // TODO
                    message->executable_type, executable_type);
                this->acknowledge_join_request(address, _network_join_refuse_reason_executable_type_mismatch);
            }
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
                "(null)",/*managed_session_get_id_string(this->m_managed_session_index)*/ // TODO
                "(null)",/*transport_secure_nonce_get_string(message->secure_nonce)*/ // TODO
                message->executable_version, message->compatible_version, compatible_version, executable_version);
            this->acknowledge_join_request(address, _network_join_refuse_reason_host_version_too_low);
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
            "(null)",/*managed_session_get_id_string(this->m_managed_session_index)*/ // TODO
            "(null)",/*transport_secure_nonce_get_string(message->secure_nonce)*/ // TODO
            message->executable_version, message->compatible_version, compatible_version, executable_version);
        this->acknowledge_join_request(address, _network_join_refuse_reason_peer_version_too_low);
    }

    return true;
}

bool c_network_session::handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message)
{
    typedef bool(__fastcall* handle_peer_connect_ptr)(c_network_session* session, s_transport_address const* outgoing_address, s_network_message_peer_connect const* message);
    auto handle_peer_connect = reinterpret_cast<handle_peer_connect_ptr>(module_base + 0x4B2E0);
    return handle_peer_connect(this, outgoing_address, message);
}

bool c_network_session::handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message)
{
    typedef bool(__fastcall* handle_session_disband_ptr)(c_network_session* session, s_transport_address const* outgoing_address, s_network_message_session_disband const* message);
    auto handle_session_disband = reinterpret_cast<handle_session_disband_ptr>(module_base + 0x4B4D0);
    return handle_session_disband(this, outgoing_address, message);
}

bool c_network_session::handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message)
{
    typedef bool(__fastcall* handle_session_boot_ptr)(c_network_session* session, s_transport_address const* outgoing_address, s_network_message_session_boot const* message);
    auto handle_session_boot = reinterpret_cast<handle_session_boot_ptr>(module_base + 0x4B560);
    return handle_session_boot(this, outgoing_address, message);
}

bool c_network_session::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
    typedef bool(__fastcall* handle_host_decline_ptr)(c_network_session* session, c_network_channel* channel, s_network_message_host_decline const* message);
    auto handle_host_decline = reinterpret_cast<handle_host_decline_ptr>(module_base + 0x4B5F0);
    return handle_host_decline(this, channel, message);
}

bool c_network_session::handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message)
{
    typedef bool(__fastcall* handle_time_synchronize_ptr)(c_network_session* session, s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message);
    auto handle_time_synchronize = reinterpret_cast<handle_time_synchronize_ptr>(module_base + 0x4B430);
    return handle_time_synchronize(this, outgoing_address, message);
}

bool c_network_session::channel_is_authoritative(c_network_channel* channel) // untested
{
    typedef bool(__fastcall* channel_is_authoritative_ptr)(c_network_session* session, c_network_channel* channel);
    auto channel_is_authoritative = reinterpret_cast<channel_is_authoritative_ptr>(module_base + 0x227A0);
    return channel_is_authoritative(this, channel);
}

bool c_network_session::handle_membership_update(s_network_message_membership_update const* message)
{
    typedef bool(__fastcall* handle_membership_update_ptr)(c_network_session* session, s_network_message_membership_update const* message);
    auto handle_membership_update = reinterpret_cast<handle_membership_update_ptr>(module_base + 0x31DD0);
    return handle_membership_update(this, message);
}

bool c_network_session::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
    typedef bool(__fastcall* handle_player_refuse_ptr)(c_network_session* session, c_network_channel* channel, s_network_message_player_refuse const* message);
    auto handle_player_refuse = reinterpret_cast<handle_player_refuse_ptr>(module_base + 0x4B7C0);
    return handle_player_refuse(this, channel, message);
}

bool c_network_session::handle_parameters_update(s_network_message_parameters_update const* message)
{
    typedef bool(__fastcall* handle_parameters_update_ptr)(c_network_session* session, s_network_message_parameters_update const* message);
    auto handle_parameters_update = reinterpret_cast<handle_parameters_update_ptr>(module_base + 0x4B3D0);
    return handle_parameters_update(this, message);
}

e_network_join_refuse_reason c_network_session::get_closure_reason()
{
    typedef e_network_join_refuse_reason(__fastcall* get_closure_reason_ptr)(c_network_session* session);
    auto get_closure_reason = reinterpret_cast<get_closure_reason_ptr>(module_base + 0x22ED0);
    return get_closure_reason(this);
}

// WIP
void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request)
{
    s_networking_join_data* data = g_network_join_data;
    e_network_join_mode join_status = data->join_queue_mode;
    e_network_join_refuse_reason closure_reason = session->get_closure_reason();

    if (closure_reason != _network_join_refuse_reason_none)
    {
        session->acknowledge_join_request(address, closure_reason);
        return;
    }
    switch (join_status)
    {
    case _network_join_closed_to_all_joins:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: attempted to join while in _network_join_closed_to_all_joins\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_not_joinable);
        return;
    case _network_join_open_to_join_squad:
        if (session->m_session_type != _network_session_type_group)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a group session while joining is in _network_join_open_to_join_squad\n");
            session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
            return;
        }
        break;
    case _network_join_queue_joins_to_group:
    case _network_join_process_queued_group_joins:
        if (session->m_session_type != _network_session_type_squad)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a squad session while in matchmaking\n");
            session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
            return;
        }
        break;
    case _network_join_queue_closed_while_in_match:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join while in matchmaking\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
        return;
    }

    int peer_index = session->m_session_membership.get_first_peer();
    while (peer_index != -1)
    {
        if (join_request->join_nonce == session->m_session_membership.m_peers[peer_index].join_nonce)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it already exists in the join queue\n",
                "(null)"/* TODO: transport_address_get_string(address) */);
            return;
        }
        peer_index = session->m_session_membership.get_next_peer(peer_index);
    }

    // TODO
}

bool network_join_process_joins_from_queue()
{
    return true;
}