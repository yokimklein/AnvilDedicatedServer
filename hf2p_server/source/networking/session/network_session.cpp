#include <iostream>
#include "network_session.h"
#include "..\..\dllmain.h"
#include "..\messages\network_message_gateway.h"
#include "..\messages\network_message_handler.h"
#include "..\network_utilities.h"
#include "..\logic\network_join.h"

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
void c_network_session::join_accept(s_network_session_join_request const* join_request, s_transport_address const* address)
{
    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] processing join request from %s\n",
        "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
        "(null)"/*transport_address_get_string(address) TODO */);

    e_network_join_refuse_reason refuse_reason = this->can_accept_join_request(join_request);
    if (refuse_reason != _network_join_refuse_reason_none)
    {
        this->acknowledge_join_request(address, refuse_reason);
        this->abort_pending_join(join_request, join_request->join_nonce);
    }
}

// TODO
e_network_join_refuse_reason c_network_session::can_accept_join_request(s_network_session_join_request const* join_request)
{
    return _network_join_refuse_reason_none;
}

// TODO
void c_network_session::abort_pending_join(s_network_session_join_request const* join_request, uint64_t join_nonce)
{

}