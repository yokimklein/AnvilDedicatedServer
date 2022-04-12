#include <iostream>
#include "network_session.h"
#include "..\..\dllmain.h"
#include "..\messages\network_message_gateway.h"
#include "..\messages\network_message_handler.h"
#include "..\network_utilities.h"
#include "..\logic\network_join.h"
#include "network_managed_session.h"
#include "../transport/transport_shim.h"

bool c_network_session::acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason)
{
    typedef char(__fastcall* managed_session_get_id_ptr)(long session_index, s_transport_secure_identifier* secure_identifier);
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
    long executable_type;
    long executable_version;
    long compatible_version;
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

// WIP - unfinished function calls, needing testing
void c_network_session::join_accept(s_network_session_join_request const* join_request, s_transport_address const* address)
{
    typedef long(__cdecl* bit_check_ptr)(long mask, long index); // test calling this
    auto bit_check = reinterpret_cast<bit_check_ptr>(module_base + 0xC39D0);

    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] processing join request from %s\n",
        "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
        "(null)"/*transport_address_get_string(address) TODO */);

    e_network_join_refuse_reason refuse_reason = this->can_accept_join_request(join_request);
    if (refuse_reason != _network_join_refuse_reason_none)
    {
        this->acknowledge_join_request(address, refuse_reason);
        this->abort_pending_join(join_request, join_request->join_nonce); // unfinished function
        return;
    }

    for (long i = 0; i < join_request->joining_peer_count; i++)
    {
        // ensure the joining peer doesn't already have membership in the session
        if (this->m_session_membership.get_peer_from_secure_address(&join_request->joining_peers[i].joining_peer_address) == -1)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] -- joining with peer %s\n",
                "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
                "(null)"/*transport_secure_address_get_string(&join_request->joining_peers[i].joining_peer_address) TODO */);
            
            long peer_index = -1;
            for (long j = 0; j < k_network_maximum_machines_per_session; j++)
            {
                // if peer is invalid
                if (!this->m_session_membership.is_peer_valid(j))
                {
                    peer_index = j;
                    break;
                }
            }
            if (peer_index == -1)
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. Failed to find available peer index -- this should always work\n");
                refuse_reason = _network_join_refuse_reason_session_full;
            }
            else
            {
                s_transport_session_description session_description;
                e_transport_platform transport_platform = (e_transport_platform)0;
                if (managed_session_get_security_information(this->m_session_membership.m_host_peer_index, false, &session_description, &transport_platform)
                    && this->m_session_membership.add_peer(peer_index, 2, &join_request->joining_peers[i].joining_network_version_number,
                        &join_request->joining_peers[i].joining_peer_address, &join_request->join_party_nonce, &join_request->join_nonce))
                {
                    xnet_shim_table_add(address, &join_request->joining_peers[i].joining_peer_address, &session_description);
                    this->m_observer.observer_channel_initiate_connection(this->m_session_index, this->m_session_membership.m_peer_channels[peer_index].channel_index);

                    // if not refused
                    if (refuse_reason == _network_join_refuse_reason_none)
                    {
                        long user_player_index = join_request->joining_peers[i].user_player_index;
                        if (user_player_index != -1)
                        {
                            long player_index = this->m_session_membership.find_or_add_player(peer_index, join_request->joining_players[user_player_index], join_request->join_from_recruiting);
                            s_player_configuration player_config = {};
                            player_config.client.multiplayer_team = -1;
                            player_config.client.unknown_team = -1;
                            s_machine_identifier host_identifier = {};
                            player_config.host.machine_identifier = host_identifier;
                            player_config.host.team = -1;
                            player_config.host.player_assigned_team = -1;
                            this->m_session_membership.update_player_data(player_index, &player_config);
                        }
                        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] '%s' session accepted a new peer %s with %d users\n",
                            "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
                            0, // c_network_session::get_type_string() TODO
                            "(null)", // c_network_session::get_peer_description TODO
                            bit_check(this->m_session_membership.m_peers[peer_index].player_mask, 16));
                    }
                }
                else
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] host failed to create observer when adding peer [%s] at peer #%d\n",
                        "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
                        "(null)",/*transport_secure_address_get_string(&join_request->joining_peers[i].joining_peer_address) TODO */
                        peer_index);
                    refuse_reason = _network_join_refuse_reason_too_many_observers;
                }
            }
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] peer tried to join a session that it's already in\n",
                "(null)"/*managed_session_get_id_string(this->m_managed_session_index) TODO */);
            refuse_reason = _network_join_refuse_reason_address_invalid;
        }

        if (refuse_reason != _network_join_refuse_reason_none)
        {
            this->acknowledge_join_request(address, refuse_reason);
            this->abort_pending_join(join_request, join_request->join_nonce); // unfinished function
            return;
        }
    }

    long peer_from_address = this->m_session_membership.get_peer_from_incoming_address(address);
    if (peer_from_address == -1)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Warning. [%s] lost connect to peer %s during join_accept\n",
            "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
            "(null)" /* c_network_session::get_peer_description TODO */);
        this->acknowledge_join_request(address, _network_join_refuse_reason_address_invalid);
        this->abort_pending_join(join_request, join_request->join_nonce); // unfinished function
        return;
    }
    long observer_channel_index = this->m_session_membership.m_peer_channels[peer_from_address].channel_index;
    if (observer_channel_index == -1)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] peer %s has invalid observer channel index\n",
            "(null)",/*managed_session_get_id_string(this->m_managed_session_index) TODO */
            "(null)" /* c_network_session::get_peer_description TODO */);
        this->acknowledge_join_request(address, _network_join_refuse_reason_too_many_observers);
        this->abort_pending_join(join_request, join_request->join_nonce); // unfinished function
        return;
    }

    if (this->m_session_membership.m_peers[peer_from_address].connection_state == (e_network_session_peer_state)2)
        this->m_session_membership.set_peer_connection_state(peer_from_address, (e_network_session_peer_state)4);
    this->m_observer.observer_channel_initiate_connection(this->m_session_index, observer_channel_index);
}

// TEST THIS
e_network_join_refuse_reason c_network_session::can_accept_join_request(s_network_session_join_request const* join_request)
{
    typedef long(__cdecl* bit_check_ptr)(long mask, long index); // test calling this
    auto bit_check = reinterpret_cast<bit_check_ptr>(module_base + 0xC39D0);

    if (!this->is_host())
        return _network_join_refuse_reason_not_joinable;
    if (!this->join_allowed_by_privacy())
        return _network_join_refuse_reason_privacy_mode;

    e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
    long joining_peer_count = join_request->joining_peer_count;
    long joining_player_count = 0;
    bool has_players = false;

    for (long i = 0; i < join_request->joining_peer_count; i++)
    {
        long peer_index = this->m_session_membership.get_peer_from_secure_address(&join_request->joining_peers[i].joining_peer_address);
        long user_player_index = join_request->joining_peers[i].user_player_index;
        if (join_request->joining_players[user_player_index].data != 0) // player_identifier_is_valid
        {
            joining_player_count++;
            has_players = true;
            refuse_reason = this->can_accept_player_join_request(&join_request->joining_players[user_player_index], join_request->joining_peers[i].joining_peer_address, peer_index, false);
        }
        else
        {
            has_players = false;
        }

        if (refuse_reason == _network_join_refuse_reason_none)
        {
            if (peer_index != -1)
            {
                joining_peer_count--;
                long peer_player_count = bit_check(this->m_session_membership.m_peers[peer_index].player_mask, 16);
                has_players = true;
                joining_player_count -= peer_player_count;
            }
            if (!has_players)
                refuse_reason = _network_join_refuse_reason_player_count_zero;
        }
        if (refuse_reason != _network_join_refuse_reason_none)
            return refuse_reason;
    }

    if (this->m_local_state <= _network_session_state_peer_join_abort) // session is_established
        return refuse_reason;

    if (this->session_is_full(joining_peer_count, joining_player_count))
        return _network_join_refuse_reason_session_full;

    return refuse_reason;
}

// TEST THIS ONCE boot_peer IS IMPLEMENTED
void c_network_session::abort_pending_join(s_network_session_join_request const* join_request, uint64_t join_nonce)
{
    if (this->m_session_membership.m_peers[this->m_session_membership.m_host_peer_index].join_nonce == join_nonce)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] the aborted join [%s] contains the host peer, disconnecting the session\n",
            "(null)" /*managed_session_get_id_string() TODO*/,
            "(null)" /*transport_secure_nonce_get_string() TODO*/);
        this->disconnect();
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] aborting join [%s]\n",
            "(null)" /*managed_session_get_id_string() TODO*/,
            "(null)" /*transport_secure_nonce_get_string() TODO*/);

        for (long i = this->m_session_membership.get_first_peer(); i != -1; i = this->m_session_membership.get_next_peer(i))
        {
            if (this->m_session_membership.m_peers[i].join_nonce == join_nonce)
                this->boot_peer(i, _network_session_boot_reason_join_aborted);
        }
    }
}

// TEST THIS
bool c_network_session::is_host()
{
    return this->m_local_state == _network_session_state_host_established || this->m_local_state == _network_session_state_host_disband;
}

// TEST THIS
bool c_network_session::join_allowed_by_privacy()
{
    if (!this->m_session_parameters.privacy_mode.get_allowed())
        return true;

    if (this->m_session_parameters.privacy_mode.get_allowed())
    {
        s_network_session_privacy_mode* privacy_parameter = &this->m_session_parameters.privacy_mode.m_data;
        return this->m_session_parameters.privacy_mode.m_data.closed_mode != _network_session_closed_none
            && this->m_session_parameters.privacy_mode.m_data.is_closed_by_user == false;
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_generic_network_session_parameter<struct s_network_session_privacy_mode>::get: [%s] failed to get parameter %d [%s], data not available\n",
            "UNKNOWN", // TODO: c_network_session_parameter_base::get_session_description(privacy_base_parameter)
            this->m_session_parameters.privacy_mode.m_type,
            this->m_session_parameters.privacy_mode.m_name);
        return false;
    }
}

// TEST THIS
e_network_join_refuse_reason c_network_session::can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address joining_peer_address, long peer_index, bool unknown)
{
    typedef e_network_join_refuse_reason(__fastcall* can_accept_player_join_request_ptr)(c_network_session* session, s_player_identifier const* player_identifier, s_transport_secure_address joining_peer_address, long peer_index, bool unknown);
    auto can_accept_player_join_request = reinterpret_cast<can_accept_player_join_request_ptr>(module_base + 0x22F30);
    return can_accept_player_join_request(this, player_identifier, joining_peer_address, peer_index, unknown);
}

bool c_network_session::session_is_full(long joining_peer_count, long joining_player_count)
{
    typedef bool(__fastcall* session_is_full_ptr)(c_network_session* session, long joining_peer_count, long joining_player_count);
    auto session_is_full = reinterpret_cast<session_is_full_ptr>(module_base + 0x22330);
    return session_is_full(this, joining_peer_count, joining_player_count);
}

void c_network_session::disconnect()
{
    typedef void(__fastcall* disconnect_ptr)(c_network_session* session);
    auto disconnect = reinterpret_cast<disconnect_ptr>(module_base + 0x21CC0);
    return disconnect(this);
}

// TODO
void c_network_session::disband_peer() // probably sub_22B50 but it seems like it's missing code?
{
    
}

// TODO
void c_network_session::boot_peer(long peer_index, e_network_session_boot_reason boot_reason)
{

}