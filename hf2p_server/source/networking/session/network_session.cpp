#include "network_session.h"
#include <iostream>
#include "..\messages\network_message_gateway.h"
#include "..\messages\network_message_handler.h"
#include "..\network_utilities.h"
#include "..\logic\network_join.h"
#include "network_managed_session.h"
#include "..\transport\transport_shim.h"

bool c_network_session::acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason)
{
    const char* managed_session_index_string = this->get_id_string();
    printf("MP/NET/SESSION,CTRL: c_network_session::acknowledge_join_request: [%s] join failed, sending refusal (%s - possibly inaccurate) to '%s'\n",
        managed_session_index_string,
        network_message_join_refuse_get_reason_string(reason),
        transport_address_get_string(address));
    s_network_message_join_refuse message;
    managed_session_get_id(this->m_managed_session_index, &message.session_id); // returning null? client doesn't seem to listen to the response - TODO, this->m_managed_session_index might be the wrong field
    message.reason = reason;
    return this->m_message_gateway->send_message_directed(address, _network_message_type_join_refuse, 20, &message);
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
                    this->get_id_string(),
                    transport_secure_nonce_get_string(&message->data.join_nonce),
                    message->data.joining_peer_count);
                network_join_add_join_to_queue(this, address, &message->data);
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] executable type mismatch, peer: %d, host: %d\n",
                    this->get_id_string(),
                    transport_secure_nonce_get_string(&message->data.join_nonce),
                    message->executable_type, executable_type);
                this->acknowledge_join_request(address, _network_join_refuse_reason_executable_type_mismatch);
            }
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
                this->get_id_string(),
                transport_secure_nonce_get_string(&message->data.join_nonce),
                message->executable_version, message->compatible_version, compatible_version, executable_version);
            this->acknowledge_join_request(address, _network_join_refuse_reason_host_version_too_low);
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(&message->data.join_nonce),
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

void c_network_session::join_accept(s_network_session_join_request const* join_request, s_transport_address const* address)
{
    //typedef long(__cdecl* bit_check_ptr)(long mask, long index); // TODO - CALL FAILS, REWRITE THIS
    //auto bit_check = reinterpret_cast<bit_check_ptr>(module_base + 0xC39D0);

    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] processing join request from %s\n",
        this->get_id_string(),
        transport_address_get_string(address));

    e_network_join_refuse_reason refuse_reason = this->can_accept_join_request(join_request);
    if (refuse_reason != _network_join_refuse_reason_none)
    {
        this->acknowledge_join_request(address, refuse_reason);
        this->abort_pending_join(join_request->join_nonce);
        return;
    }

    for (long i = 0; i < join_request->joining_peer_count; i++)
    {
        // ensure the joining peer doesn't already have membership in the session
        long peer_index = this->m_session_membership.get_peer_from_secure_address(&join_request->joining_peers[i].joining_peer_address);
        if (peer_index == -1)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] -- joining with peer %s\n",
                this->get_id_string(),
                transport_secure_address_get_string(&join_request->joining_peers[i].joining_peer_address));
            
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
                if (managed_session_get_security_information(this->m_managed_session_index, &session_description, &transport_platform)
                    && this->m_session_membership.add_peer(peer_index, _network_session_peer_state_reserved, join_request->joining_peers[i].joining_network_version_number,
                        &join_request->joining_peers[i].joining_peer_address, join_request->join_party_nonce, join_request->join_nonce))
                {
                    xnet_shim_table_add(address, &join_request->joining_peers[i].joining_peer_address, &session_description.session_id);
                    this->m_observer->observer_channel_initiate_connection((e_network_observer_owner)this->m_session_index, this->m_session_membership.get_observer_channel_index(peer_index));

                    // if not refused
                    if (refuse_reason == _network_join_refuse_reason_none)
                    {
                        long user_player_index = join_request->joining_peers[i].user_player_index;
                        if (user_player_index != -1)
                        {
                            long player_index = this->m_session_membership.find_or_add_player(peer_index, &join_request->joining_players[user_player_index], join_request->join_from_recruiting);
                            s_player_configuration* player_config = new s_player_configuration;
                            memset(player_config, 0, sizeof(s_player_configuration));
                            player_config->client.multiplayer_team = -1;
                            player_config->client.unknown_team = -1;
                            s_player_identifier* player_xuid = new s_player_identifier;
                            memset(player_xuid, 0, sizeof(s_player_identifier));
                            player_config->host.player_xuid = *player_xuid;
                            player_config->host.player_team = -1;
                            player_config->host.player_assigned_team = -1;
                            this->m_session_membership.update_player_data(player_index, player_config);
                        }
                        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] '%s' session accepted a new peer %s with %d users\n",
                            this->get_id_string(),
                            this->get_type_string(this->m_session_type),
                            this->get_peer_description(peer_index),
                            this->m_session_membership.m_baseline.peers[peer_index].player_mask/*bit_check(this->m_session_membership.m_peers[peer_index].player_mask, 16)*/); // TODO - wtf is bit check actually meant to do?
                    }
                }
                else
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] host failed to create observer when adding peer [%s] at peer #%d\n",
                        this->get_id_string(),
                        transport_secure_address_get_string(&join_request->joining_peers[i].joining_peer_address),
                        peer_index);
                    refuse_reason = _network_join_refuse_reason_too_many_observers;
                }
            }
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] peer tried to join a session that it's already in\n",
                this->get_id_string());
            refuse_reason = _network_join_refuse_reason_address_invalid;
        }

        if (refuse_reason != _network_join_refuse_reason_none)
        {
            this->acknowledge_join_request(address, refuse_reason);
            this->abort_pending_join(join_request->join_nonce);
            return;
        }
    }

    long peer_from_address = this->m_session_membership.get_peer_from_incoming_address(address);
    if (peer_from_address == -1)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Warning. [%s] lost connect to peer %s during join_accept\n",
            this->get_id_string(),
            this->get_peer_description(peer_from_address));
        this->acknowledge_join_request(address, _network_join_refuse_reason_address_invalid);
        this->abort_pending_join(join_request->join_nonce);
        return;
    }
    long observer_channel_index = this->m_session_membership.get_observer_channel_index(peer_from_address);
    if (observer_channel_index == -1)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] peer %s has invalid observer channel index\n",
            this->get_id_string(),
            this->get_peer_description(peer_from_address));
        this->acknowledge_join_request(address, _network_join_refuse_reason_too_many_observers);
        this->abort_pending_join(join_request->join_nonce);
        return;
    }

    if (this->m_session_membership.m_baseline.peers[peer_from_address].connection_state == _network_session_peer_state_reserved)
        this->m_session_membership.set_peer_connection_state(peer_from_address, _network_session_peer_state_connected);
    this->m_observer->observer_channel_initiate_connection((e_network_observer_owner)this->m_session_index, observer_channel_index);
}

e_network_join_refuse_reason c_network_session::can_accept_join_request(s_network_session_join_request const* join_request)
{
    //typedef long(__cdecl* bit_check_ptr)(long mask, long index); // TODO - call fails, rewrite this
    //auto bit_check = reinterpret_cast<bit_check_ptr>(module_base + 0xC39D0);

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
            refuse_reason = this->can_accept_player_join_request(&join_request->joining_players[user_player_index], &join_request->joining_peers[i].joining_peer_address, peer_index, false);
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
                long peer_player_count = this->m_session_membership.m_baseline.peers[peer_index].player_mask; // bit_check(this->m_session_membership.m_peers[peer_index].player_mask, 16); // TODO - wtf is bit_check actually meant to do?
                has_players = true;
                joining_player_count -= peer_player_count;
            }
            if (!has_players)
                refuse_reason = _network_join_refuse_reason_player_count_zero;
        }
        if (refuse_reason != _network_join_refuse_reason_none)
            return refuse_reason;
    }

    if (!this->established()) // this->m_local_state <= _network_session_state_peer_join_abort
        return refuse_reason;

    if (this->session_is_full(joining_peer_count, joining_player_count))
        return _network_join_refuse_reason_session_full;

    return refuse_reason;
}

void c_network_session::abort_pending_join(uint64_t join_nonce)
{
    if (this->m_session_membership.m_baseline.peers[this->m_session_membership.m_baseline.host_peer_index].join_nonce == join_nonce)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] the aborted join [%s] contains the host peer, disconnecting the session\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(&join_nonce));
        this->disconnect();
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] aborting join [%s]\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(&join_nonce));

        for (long i = this->m_session_membership.get_first_peer(); i != -1; i = this->m_session_membership.get_next_peer(i))
        {
            if (this->m_session_membership.m_baseline.peers[i].join_nonce == join_nonce)
                this->boot_peer(i, _network_session_boot_reason_join_aborted);
        }
    }
}

bool c_network_session::is_host()
{
    return this->current_local_state() == _network_session_state_host_established || this->current_local_state() == _network_session_state_host_disband;
}

bool c_network_session::join_allowed_by_privacy()
{
    if (!this->m_session_parameters.privacy_mode.get_allowed())
        return true;

    if (this->m_session_parameters.privacy_mode.get_allowed())
    {
        s_network_session_privacy_mode* privacy_parameter = &this->m_session_parameters.privacy_mode.m_data;
        return this->m_session_parameters.privacy_mode.m_data.closed_mode == _network_session_closed_none
            && !this->m_session_parameters.privacy_mode.m_data.is_closed_by_user;
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_generic_network_session_parameter<struct s_network_session_privacy_mode>::get: [%s] failed to get parameter %d [%s], data not available\n",
            this->m_session_parameters.privacy_mode.get_session_description(),
            this->m_session_parameters.privacy_mode.m_type,
            this->m_session_parameters.privacy_mode.m_name);
        return false;
    }
}

e_network_join_refuse_reason c_network_session::can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool unknown)
{
    typedef e_network_join_refuse_reason(__thiscall* can_accept_player_join_request_ptr)(c_network_session* session, s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool unknown);
    auto can_accept_player_join_request = reinterpret_cast<can_accept_player_join_request_ptr>(module_base + 0x22F30);
    return can_accept_player_join_request(this, player_identifier, joining_peer_address, peer_index, unknown);
}

bool c_network_session::session_is_full(long joining_peer_count, long joining_player_count)
{
    typedef bool(__thiscall* session_is_full_ptr)(c_network_session* session, long joining_peer_count, long joining_player_count);
    auto session_is_full = reinterpret_cast<session_is_full_ptr>(module_base + 0x22330);
    return session_is_full(this, joining_peer_count, joining_player_count);
}

void c_network_session::disconnect()
{
    typedef void(__fastcall* disconnect_ptr)(c_network_session* session);
    auto disconnect = reinterpret_cast<disconnect_ptr>(module_base + 0x21CC0);
    return disconnect(this);
}

void c_network_session::disband_peer(long peer_index)
{
    if (this->get_session_membership()->has_peer_ever_been_established(peer_index))
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] disbanding peer [%s]\n",
            this->get_id_string(),
            this->get_peer_description(peer_index));
        s_network_message_session_disband disband_message;
        memset(&disband_message, 0, sizeof(s_network_message_session_disband));
        managed_session_get_id(this->managed_session_index(), &disband_message.session_id);
        long observer_index = this->m_session_membership.m_baseline.peer_channels[peer_index].channel_index;
        if (observer_index != -1)
        {
            auto observer = this->m_observer;
            if (observer->observer_channel_connected(this->m_session_index, observer_index))
                this->m_observer->observer_channel_send_message(this->m_session_index, observer_index, false, _network_message_type_session_disband, sizeof(s_network_message_session_disband), &disband_message);
            this->m_observer->observer_channel_send_message(this->m_session_index, observer_index, true, _network_message_type_session_disband, sizeof(s_network_message_session_disband), &disband_message);
        }
        this->get_session_membership()->remove_peer(peer_index);
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] marking joining peer [%s] for future disbandment\n",
            this->get_id_string(),
            this->get_peer_description(peer_index));
        this->get_session_membership()->set_peer_connection_state(peer_index, _network_session_peer_state_disconnected);
    }
}

void c_network_session::boot_peer(long peer_index, e_network_session_boot_reason boot_reason)
{
    if (this->membership_is_locked())
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::boot_peer: [%s] unable to boot peer [#%d] membership is locked\n",
            this->get_id_string(),
            peer_index);
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::boot_peer: [%s] booting peer [#%d] [reason %d]\n",
            this->get_id_string(),
            peer_index,
            boot_reason);
        s_network_message_session_boot boot_message;
        memset(&boot_message, 0, sizeof(s_network_message_session_boot));
        managed_session_get_id(this->managed_session_index(), &boot_message.session_id);
        boot_message.reason = boot_reason;
        long observer_index = this->m_session_membership.m_baseline.peer_channels[peer_index].channel_index;
        if (observer_index != -1)
        {
            auto observer = this->m_observer;
            if (observer->observer_channel_connected(this->m_session_index, observer_index))
                this->m_observer->observer_channel_send_message(this->m_session_index, observer_index, false, _network_message_type_session_boot, sizeof(s_network_message_session_boot), &boot_message);
            this->m_observer->observer_channel_send_message(this->m_session_index, observer_index, true, _network_message_type_session_boot, sizeof(s_network_message_session_boot), &boot_message);
        }
        this->get_session_membership()->remove_peer(peer_index);
    }
}

const char* c_network_session::get_type_string(e_network_session_type session_type)
{
    const char* type_string = "<unknown>";
    switch (session_type)
    {
        case _network_session_type_none:
            type_string = "none";
            break;
        case _network_session_type_group:
            type_string = "group";
            break;
        case _network_session_type_squad:
            type_string = "squad";
            break;
    }
    return type_string;
}

// FUNC TODO: mac address & unknown peer field
const char* c_network_session::get_peer_description(long peer_index)
{
    static char peer_description[51];
    sprintf_s(peer_description, "#%02d", peer_index);
    return peer_description;
}

const char* c_network_session::get_id_string()
{
    return managed_session_get_id_string(this->m_managed_session_index);
}

bool c_network_session::is_peer_joining_this_session()
{
    typedef bool(__fastcall* is_peer_joining_this_session_ptr)(c_network_session* session_ptr);
    auto is_peer_joining_this_session = reinterpret_cast<is_peer_joining_this_session_ptr>(module_base + 0x224F0);
    return is_peer_joining_this_session(this);
}

void c_network_session::idle()
{
    if (!this->disconnected())
    {
        this->get_session_membership()->idle();
        if (this->established() && this->get_session_membership()->all_peers_established() && !this->m_disconnection_policy)
            this->m_disconnection_policy = 1;
        switch (this->current_local_state())
        {
            case _network_session_state_peer_creating:
                this->idle_peer_creating();
                break;
            case _network_session_state_peer_joining:
                this->idle_peer_joining();
                break;
            case _network_session_state_peer_join_abort:
                this->idle_peer_join_abort();
                break;
            case _network_session_state_peer_leaving:
                this->idle_peer_leaving();
                break;
            case _network_session_state_host_disband:
                this->disconnect();
                break;
            default:
                break;
        }
        // ==== SECTION CUT FROM CLIENT BUILDS ====
        if (this->is_host() && !this->membership_is_locked())
            this->process_pending_joins(); // MISSING FUNC TODO
        if (this->established())
        {
            for (long i = this->get_session_membership()->get_first_peer(); i != -1; i = this->get_session_membership()->get_next_peer(i))
            {
                long observer_channel_index = this->get_session_membership()->get_observer_channel_index(i);
                if (observer_channel_index != -1)
                {
                    e_network_observer_owner observer_owner = this->observer_owner();
                    if (this->m_observer->observer_channel_dead(observer_owner, observer_channel_index))
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::idle: [%s] peer [%s]/%s channel died, disbanding\n",
                            this->get_id_string(), this->get_peer_description(i), this->m_observer->get_name(observer_channel_index));
                    }
                    this->disband_peer(i);
                }
            }

            if (this->established() && this->m_session_type == _network_session_type_group)
            {
                long private_slot_count = 0;
                long public_slot_count = 0;
                bool friends_only = false;
                long maximum_player_count = 0;
                if (this->m_session_parameters.session_size.get_allowed())
                {
                    maximum_player_count = this->get_maximum_player_count();
                    if (maximum_player_count < private_slot_count)
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::idle: maximum session player count is smaller than the set of players already in the session, %d < %d\n",
                            maximum_player_count, private_slot_count);
                        maximum_player_count = private_slot_count;
                    }
                }
                if (this->m_session_parameters.privacy_mode.get_allowed())
                {
                    auto privacy_mode_parameter = this->m_session_parameters.privacy_mode.m_data;
                    e_network_session_closed_status closed_mode = privacy_mode_parameter.closed_mode;
                    bool is_closed_by_user = privacy_mode_parameter.is_closed_by_user;
                    if (closed_mode == _network_session_closed_none && !is_closed_by_user)
                    {
                        e_network_game_privacy privacy_mode = privacy_mode_parameter.privacy_mode;
                        if (privacy_mode == _network_game_privacy_open_to_public)
                        {
                            friends_only = false;
                            public_slot_count = maximum_player_count;
                            private_slot_count = 0;
                        }
                        else if (privacy_mode == _network_game_privacy_open_to_friends)
                        {
                            friends_only = true;
                            public_slot_count = maximum_player_count;
                            private_slot_count = 0;
                        }
                        else if (privacy_mode == _network_game_privacy_invite_only)
                        {
                            friends_only = false;
                            public_slot_count = 0;
                            private_slot_count = maximum_player_count;
                        }
                        else if (privacy_mode == _network_game_privacy_closed)
                        {
                            friends_only = false;
                            public_slot_count = 0;
                            private_slot_count = 0;
                        }
                    }
                }
                if (this->m_session_membership.private_slot_count() != private_slot_count ||
                    this->m_session_membership.public_slot_count() != public_slot_count ||
                    this->m_session_membership.friends_only() != friends_only)
                    this->m_session_membership.set_slot_counts(private_slot_count, public_slot_count, friends_only);
            }
        }
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        if (!this->is_host() && this->established())
        {
            long observer_channel_index = this->m_session_membership.get_host_observer_channel_index();
            if (this->m_observer->observer_channel_dead(this->observer_owner(), observer_channel_index))
            {
                this->handle_disconnection();
            }
        }
        if (this->established())
            this->check_to_send_time_synchronization();
        this->idle_observer_state();
        // ==== SECTION CUT FROM CLIENT BUILDS ====
        if (this->is_host())
            this->check_to_send_membership_update(); // MISSING FUNC TODO
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        this->m_session_parameters.update();
        if (this->m_session_parameters.session_size.set_allowed())
        {
            auto membership = this->get_session_membership();
            long player_count = membership->get_player_count();
            if (player_count > this->m_session_parameters.session_size.get_max_player_count())
                this->m_session_parameters.session_size.set_max_player_count(player_count);
        }
    }
}

e_network_session_state c_network_session::current_local_state()
{
    // TODO - ASSERT m_local_state>=0 && m_local_state<k_network_session_state_count
    return this->m_local_state;
}

bool c_network_session::disconnected()
{
    return this->current_local_state() == _network_session_state_none;
}

bool c_network_session::established()
{
    switch (this->current_local_state())
    {
        case _network_session_state_none:
        case _network_session_state_peer_creating:
        case _network_session_state_peer_joining:
        case _network_session_state_peer_join_abort:
        case k_network_session_state_count:
            return false;
        case _network_session_state_peer_established:
        case _network_session_state_peer_leaving:
        case _network_session_state_host_established:
        case _network_session_state_host_disband:
        //case _network_session_state_host_handoff:
        //case _network_session_state_host_reestablish:
        //case _network_session_state_election:
            return true;
        default:
            return false; // TODO - 'unreachable' ASSERT HERE
    }
}

bool c_network_session::membership_is_locked()
{
    // TODO - is_host() ASSERT HERE
    return this->current_local_state() == _network_session_state_host_disband/* || this->current_local_state() == _network_session_state_host_handoff || this->current_local_state() == _network_session_state_host_reestablish*/;
    // TODO - established() & locked ASSERT HERE
}

c_network_session_membership* c_network_session::get_session_membership()
{
    // TODO - ASSERTS
    return &this->m_session_membership;
}

void c_network_session::idle_peer_creating()
{
    void(__thiscall * idle_peer_creating)(c_network_session* session) = reinterpret_cast<decltype(idle_peer_creating)>(module_base + 0x3E800);
    idle_peer_creating(this);
}

void c_network_session::idle_peer_joining()
{
    void(__thiscall * idle_peer_joining)(c_network_session* session) = reinterpret_cast<decltype(idle_peer_joining)>(module_base + 0x3E8B0);
    idle_peer_joining(this);
}

void c_network_session::idle_peer_join_abort()
{
    void(__thiscall * idle_peer_join_abort)(c_network_session* session) = reinterpret_cast<decltype(idle_peer_join_abort)>(module_base + 0x3EA00);
    idle_peer_join_abort(this);
}

void c_network_session::idle_peer_leaving()
{
    void(__thiscall * idle_peer_leaving)(c_network_session* session) = reinterpret_cast<decltype(idle_peer_leaving)>(module_base + 0x3EA60);
    idle_peer_leaving(this);
}

// FUNC TODO
void c_network_session::process_pending_joins()
{

}

e_network_observer_owner c_network_session::observer_owner()
{
    // TODO ASSERT m_session_index>=0 && m_session_index<k_network_maximum_sessions
    return this->m_session_index;
}

long c_network_session::get_maximum_player_count()
{
    long(__thiscall * get_maximum_player_count)(c_network_session* session) = reinterpret_cast<decltype(get_maximum_player_count)>(module_base + 0x22E80);
    return get_maximum_player_count(this);
}

void c_network_session::handle_disconnection()
{
    void(__cdecl * hf2p_handle_disconnection)() = reinterpret_cast<decltype(hf2p_handle_disconnection)>(module_base + 0x2F29C0);

    printf("MP/NET/SESSION,CTRL: c_network_session::handle_disconnection: [%s] disconnected from session host. Session Type %s\n",
        this->get_id_string(),
        get_type_string(this->m_session_type));
    hf2p_handle_disconnection(); // unknown saber function, wasn't in ms23
    this->disconnect();
}

void c_network_session::check_to_send_time_synchronization()
{
    void(__thiscall * check_to_send_time_synchronization)(c_network_session* session) = reinterpret_cast<decltype(check_to_send_time_synchronization)>(module_base + 0x22B50);
    check_to_send_time_synchronization(this);
}

void c_network_session::idle_observer_state()
{
    void(__thiscall * idle_observer_state)(c_network_session* session) = reinterpret_cast<decltype(idle_observer_state)>(module_base + 0x3EAC0);
    idle_observer_state(this);
}

// FUNC TODO
void c_network_session::check_to_send_membership_update()
{
    for (long i = this->get_session_membership()->get_first_peer(); i != -1; i = get_session_membership()->get_next_peer(i))
    {
        auto peer_channel = &this->get_session_membership()->m_baseline.peer_channels[i];
        if (peer_channel->channel_index != -1 && this->get_session_membership()->get_peer_connection_state(i) >= _network_session_peer_state_connected && !peer_channel->flags)
        {
            auto observer = this->m_observer;
            auto channel_observer = &observer->m_channel_observers[peer_channel->channel_index];
            if (observer->observer_channel_connected(this->observer_owner(), peer_channel->channel_index))
            {
                if (peer_channel->expected_update_number != this->get_session_membership()->m_baseline.update_number)
                {
                    if (observer->observer_channel_backlogged(this->observer_owner(), peer_channel->channel_index, _network_message_type_membership_update))
                    {
                        observer->observer_channel_set_waiting_on_backlog(this->observer_owner(), peer_channel->channel_index, _network_message_type_membership_update);
                    }
                    else
                    {
                        // fucky shit resides here - TODO

                        this->get_session_membership()->build_membership_update(i, membership, baseline, update_message); // TODO - the rest of these args
                        // const struct s_network_session_shared_membership *membership
                        // const struct s_network_session_shared_membership *baseline
                        // struct s_network_message_membership_update *message)

                    }
                }
            }
        }
    }
}

long c_network_session::managed_session_index()
{
    // TODO ASSERTS
    return this->m_managed_session_index;
}

bool c_network_session::join_abort(s_transport_address const* incoming_address, int64_t join_nonce)
{
    network_join_remove_join_from_queue(join_nonce);
    if (this->membership_is_locked())
        return false;
    this->abort_pending_join(join_nonce);
    return true;
}

c_network_session_parameters* c_network_session::get_session_parameters()
{
    return &this->m_session_parameters;
}