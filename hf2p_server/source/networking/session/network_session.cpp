#include "network_session.h"
#include "assert.h"
#include <iostream>
#include <networking\messages\network_message_gateway.h>
#include <networking\network_utilities.h>
#include <networking\logic\network_join.h>
#include <networking\session\network_managed_session.h>
#include <networking\transport\transport_shim.h>
#include <hf2p\hf2p_session.h>
#include <networking\network_configuration.h>
#include <cseries\cseries.h>
#include <hf2p\user.h>
#include <game\game.h>

char const* k_session_type_strings[k_network_session_type_count] = {
    "none",
    "squad"
    //"group" // No longer exists in ms29
};

char const* k_session_state_strings[k_network_session_state_count] = {
    "none",
    "peer-creating",
    "peer-joining",
    "peer-join-abort",
    "peer-established",
    "peer-leaving",
    "host-established",
    "host-disband",
    // "host-handoff",
    // "host-reestablish",
    // "election"
};

bool c_network_session::acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason)
{
    printf("MP/NET/SESSION,CTRL: c_network_session::acknowledge_join_request: [%s] join failed, sending refusal (%s) to '%s'\n",
        this->get_id_string(),
        network_message_join_refuse_get_reason_string(reason),
        transport_address_get_string(address));
    
    s_network_message_join_refuse message = {};
    this->get_session_id(&message.session_id);
    message.reason = reason;
    return this->m_message_gateway->send_message_directed(address, _network_message_type_join_refuse, sizeof(s_network_message_join_refuse), &message);
}

// TODO: ensure that each peer matches a session ID within the API lobby to prevent random external joins coming in from outside of our matchmaking system
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
                    transport_secure_nonce_get_string(message->data.join_nonce),
                    message->data.joining_peer_count);
                network_join_add_join_to_queue(this, address, &message->data);
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] executable type mismatch, peer: %d, host: %d\n",
                    this->get_id_string(),
                    transport_secure_nonce_get_string(message->data.join_nonce),
                    message->executable_type, executable_type);
                this->acknowledge_join_request(address, _network_join_refuse_reason_executable_type_mismatch);
            }
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
                this->get_id_string(),
                transport_secure_nonce_get_string(message->data.join_nonce),
                message->executable_version, message->compatible_version, compatible_version, executable_version);
            this->acknowledge_join_request(address, _network_join_refuse_reason_host_version_too_low);
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(message->data.join_nonce),
            message->executable_version, message->compatible_version, compatible_version, executable_version);
        this->acknowledge_join_request(address, _network_join_refuse_reason_peer_version_too_low);
    }

    return true;
}

bool c_network_session::handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message)
{
    return DECLFUNC(0x4B2E0, bool, __thiscall, c_network_session*, s_transport_address const*, s_network_message_peer_connect const*)(this, outgoing_address, message);
}

bool c_network_session::handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message)
{
    return DECLFUNC(0x4B4D0, bool, __thiscall, c_network_session*, s_transport_address const*, s_network_message_session_disband const*)(this, outgoing_address, message);
}

bool c_network_session::handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message)
{
    return DECLFUNC(0x4B560, bool, __thiscall, c_network_session*, s_transport_address const*, s_network_message_session_boot const*)(this, outgoing_address, message);
}

bool c_network_session::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
    return DECLFUNC(0x4B5F0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_host_decline const*)(this, channel, message);
}

bool c_network_session::handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message)
{
    if (this->established())
    {
        if (this->is_host())
        {
            long peer_index = this->m_session_membership.get_peer_from_incoming_address(outgoing_address);
            if (peer_index == NONE || peer_index == this->m_session_membership.local_peer_index())
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time authority received synchronization from invalid peer index [#%d]\n",
                    this->get_id_string(),
                    peer_index);
                return false;
            }
            else if (message->synchronization_stage != 0)
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time authority received synchronization stage %d (expected 0)\n",
                    this->get_id_string(),
                    message->synchronization_stage);
                return false;
            }
            else if (this->leaving_session())
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] ignoring time request from peer [%s] (currently leaving session)\n",
                    this->get_id_string(),
                    this->get_peer_description(peer_index));
                return false;
            }
            else
            {
                s_network_message_time_synchronize sync_message = {};
                sync_message.synchronization_stage = message->synchronization_stage;
                sync_message.session_id = message->session_id;
                sync_message.client_timestamp[0] = message->client_timestamp[0];
                sync_message.client_timestamp[1] = message->client_timestamp[1];
                sync_message.authority_timestamp[0] = message->authority_timestamp[0];
                sync_message.authority_timestamp[1] = message->authority_timestamp[1];
                sync_message.synchronization_stage = 1;
                this->m_message_gateway->send_message_directed(outgoing_address, _network_message_type_time_synchronize, sizeof(s_network_message_time_synchronize), &sync_message);
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] replying to time request from peer [%s]\n",
                    this->get_id_string(),
                    this->get_peer_description(peer_index));
                return true;
            }
        }
        else if (this->m_session_membership.host_exists_at_incoming_address(outgoing_address))
        {
            if (message->synchronization_stage == 1)
            {
                long time_delta1 = message->authority_timestamp[0] - message->client_timestamp[0];
                long time_delta2 = message->authority_timestamp[1] - message->client_timestamp[1];
                long synchronized_time_offset = (time_delta1 + time_delta2) / 2;
                long synchronized_time_epsilon = abs(time_delta1 - synchronized_time_offset);
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] synchronized offset %dmsec (epsilon %dmsec)\n",
                    this->get_id_string(),
                    synchronized_time_offset,
                    synchronized_time_epsilon);
                this->time_set(synchronized_time_offset);
                if (this->m_time_exists)
                    this->m_time_synchronization_end_time = this->m_time;
                else
                    this->m_time_synchronization_end_time = timeGetTime();
                return true;
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time client received synchronization stage %d (expected 1)\n",
                    this->get_id_string(),
                    message->synchronization_stage);
                return false;
            }
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] client received from someone other than authority\n",
                this->get_id_string());
            return false;
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] not established, can't synchronize time\n",
            this->get_id_string());
        return false;
    }
    return false;
}

bool c_network_session::channel_is_authoritative(c_network_channel* channel)
{
    return DECLFUNC(0x227A0, bool, __thiscall, c_network_session*, c_network_channel*)(this, channel);
}

bool c_network_session::handle_membership_update(s_network_message_membership_update const* message)
{
    return this->m_session_membership.handle_membership_update(message);
}

bool c_network_session::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
    return DECLFUNC(0x4B7C0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_refuse const*)(this, channel, message);
}

bool c_network_session::handle_parameters_update(s_network_message_parameters_update const* message)
{
    return DECLFUNC(0x4B3D0, bool, __thiscall, c_network_session*, s_network_message_parameters_update const*)(this, message);
}

e_network_join_refuse_reason c_network_session::can_accept_any_join_request()
{
    if (!this->get_session_parameters()->privacy_mode.get_allowed())
        return _network_join_refuse_reason_none;

    s_network_session_privacy_mode* privacy_mode = this->get_session_parameters()->privacy_mode.get();
    switch (privacy_mode->closed_mode.get())
    {
        case _network_session_closed_campaign_in_progress:
            return _network_join_refuse_reason_campaign_in_progress;
        case _network_session_closed_survival_in_progress:
            return _network_join_refuse_reason_survival_in_progress;
        case _network_session_closed_film_in_progress:
            return _network_join_refuse_reason_film_in_progress;
        //case _network_session_closed_in_matchmaking: // code removed since ms23, enums remain
        //    return _network_join_refuse_reason_in_matchmaking;
    }

    if (privacy_mode->is_closed_by_user)
        return _network_join_refuse_reason_session_full;

    return _network_join_refuse_reason_none;
}

void c_network_session::join_accept(s_network_session_join_request const* join_request, s_transport_address const* address)
{
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
        if (peer_index == NONE)
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
                s_transport_session_description session_description{};
                if (managed_session_get_security_information(this->managed_session_index(), &session_description, nullptr)
                    && this->m_session_membership.add_peer(peer_index, _network_session_peer_state_reserved, join_request->joining_peers[i].joining_network_version_number,
                        &join_request->joining_peers[i].joining_peer_address, join_request->join_party_nonce, join_request->join_nonce))
                {
                    XNetAddEntry(address, &join_request->joining_peers[i].joining_peer_address, &session_description.session_id);
                    this->m_observer->observer_channel_initiate_connection(this->observer_owner(), this->m_session_membership.get_observer_channel_index(peer_index));

                    // if not refused
                    if (refuse_reason == _network_join_refuse_reason_none)
                    {
                        long user_player_index = join_request->joining_peers[i].user_player_index;
                        if (user_player_index != -1)
                        {
                            long player_index = this->m_session_membership.find_or_add_player(peer_index, &join_request->joining_players[user_player_index], join_request->join_from_recruiting);
                            
                            s_player_configuration player_config;
                            csmemset(&player_config.client, 0, sizeof(s_player_configuration_from_client));
                            player_config.host.player_appearance.emblem_info = s_emblem_info();
                            csmemset(&player_config, 0, sizeof(s_player_configuration)); // makes these top 2 line redundant, but source does this?
                            player_config.client.user_selected_team_index = _game_team_none;
                            player_config.client.selected_loadout_index = NONE;
                            player_config.host.user_xuid = user_xuid_from_secure_address(&join_request->joining_peers[i].joining_peer_address);
                            player_config.host.team_index = _game_team_none;
                            player_config.host.user_selected_team_index = _game_team_none;

                            this->m_session_membership.update_player_data(player_index, &player_config);
                        }
                        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] '%s' session accepted a new peer %s with %d users\n",
                            this->get_id_string(),
                            this->get_type_string(this->session_type()),
                            this->get_peer_description(peer_index),
                            this->m_session_membership.get_peer(peer_index)->player_mask.count_bits_set());
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

    if (this->m_session_membership.get_peer_connection_state(peer_from_address) == _network_session_peer_state_reserved)
        this->m_session_membership.set_peer_connection_state(peer_from_address, _network_session_peer_state_connected);
    this->m_observer->observer_channel_initiate_connection(this->observer_owner(), observer_channel_index);
}

e_network_join_refuse_reason c_network_session::can_accept_join_request(s_network_session_join_request const* join_request)
{
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
        if (player_identifier_is_valid(&join_request->joining_players[user_player_index]))
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
                long peer_player_count = this->m_session_membership.get_peer(peer_index)->player_mask.count_bits_set();
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

void c_network_session::abort_pending_join(qword join_nonce)
{
    if (this->m_session_membership.get_peer(this->m_session_membership.host_peer_index())->join_nonce == join_nonce)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] the aborted join [%s] contains the host peer, disconnecting the session\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(join_nonce));
        this->disconnect();
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] aborting join [%s]\n",
            this->get_id_string(),
            transport_secure_nonce_get_string(join_nonce));

        for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
        {
            if (this->m_session_membership.get_peer(i)->join_nonce == join_nonce)
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
    assert(this->established() && this->is_host());

    if (!this->get_session_parameters()->privacy_mode.get_allowed())
        return true;
    s_network_session_privacy_mode* privacy_mode = this->get_session_parameters()->privacy_mode.get();
    return privacy_mode->closed_mode == _network_session_closed_none && !privacy_mode->is_closed_by_user;

}

e_network_join_refuse_reason c_network_session::can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool unknown)
{
    assert(player_identifier);
    assert(player_identifier_is_valid(player_identifier));

    long player_count = 0;
    e_network_join_refuse_reason refuse_reason = this->can_accept_any_join_request();
    if (refuse_reason != _network_join_refuse_reason_none)
        return refuse_reason;
    long player_index_from_identifier = m_session_membership.get_player_from_identifier(player_identifier);
    if (peer_index != NONE && m_session_membership.is_peer_valid(peer_index))
    {
        s_network_session_peer* peer = m_session_membership.get_peer(peer_index);
        player_count = peer->player_mask.count_bits_set();
        if (!m_session_membership.is_player_in_player_add_queue(player_identifier) && peer->get_player_index() == NONE)
        {
            player_count++;
        }
        for (long i = 0; i < NUMBEROF(c_network_session_membership::m_player_add_queue); i++)
        {
            s_player_add_queue_entry* player_add_queue_entry = m_session_membership.get_player_add_queue_entry(i);
            if (player_add_queue_entry != nullptr && player_add_queue_entry->peer_index == peer_index && peer->get_player_index() == NONE)
            {
                player_count++;
            }
        }
        refuse_reason = _network_join_refuse_reason_none;
    }

    if (game_is_dedicated_server() && user_xuid_from_secure_address(joining_peer_address) == USER_INVALID)
        return _network_join_refuse_reason_address_invalid;

    if (peer_index != NONE && !m_session_membership.is_peer_valid(peer_index))
        return _network_join_refuse_reason_session_booted;

    // removed since ms23
    //if (m_session_type == _network_session_type_group && unknown)
    //    return _network_join_refuse_reason_in_matchmaking;

    if (player_count > 4)
        return _network_join_refuse_reason_alpha_split_screen;

    if (player_index_from_identifier == NONE)
    {
        if ((peer_index == NONE
            || m_session_membership.is_peer_valid(peer_index)
            && m_session_membership.get_peer(peer_index)->get_player_index() == NONE)
            && this->session_is_full(0, 1))
        {
            return _network_join_refuse_reason_session_full;
        }
    }
    else if (m_session_membership.get_player(player_index_from_identifier)->peer_index != peer_index)
    {
        return _network_join_refuse_reason_player_add_failed;
    }
    return refuse_reason;
}

bool c_network_session::session_is_full(long joining_peer_count, long joining_player_count)
{
    if (this->established())
    {
        long max_peer_count = m_session_parameters.session_size.get_max_peer_count();
        long max_player_count = m_session_parameters.session_size.get_max_player_count();
        assert(m_session_membership.has_membership());
        assert(max_peer_count > 0 && max_peer_count <= k_network_maximum_machines_per_session);
        assert(max_player_count > 0 && max_player_count <= k_network_maximum_players_per_session);
        return m_session_membership.get_peer_count() + joining_peer_count > max_peer_count
            || m_session_membership.get_player_count() + joining_player_count > max_player_count;
    }
    return false;
}

void c_network_session::disconnect()
{
    DECLFUNC(0x21CC0, void, __thiscall, c_network_session*)(this);
}

void c_network_session::disband_peer(long peer_index)
{
    if (this->m_session_membership.has_peer_ever_been_established(peer_index))
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] disbanding peer [%s]\n",
            this->get_id_string(),
            this->get_peer_description(peer_index));
        s_network_message_session_disband disband_message = {};
        this->get_session_id(&disband_message.session_id);
        long observer_index = this->m_session_membership.get_observer_channel_index(peer_index);
        if (observer_index != NONE)
        {
            c_network_observer* observer = this->m_observer;
            if (observer->observer_channel_connected(this->observer_owner(), observer_index))
                this->m_observer->observer_channel_send_message(this->observer_owner(), observer_index, false, _network_message_type_session_disband, sizeof(s_network_message_session_disband), &disband_message);
            this->m_observer->observer_channel_send_message(this->observer_owner(), observer_index, true, _network_message_type_session_disband, sizeof(s_network_message_session_disband), &disband_message);
        }
        this->m_session_membership.remove_peer(peer_index);
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] marking joining peer [%s] for future disbandment\n",
            this->get_id_string(),
            this->get_peer_description(peer_index));
        this->m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_disconnected);
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
        s_network_message_session_boot boot_message = {};
        this->get_session_id(&boot_message.session_id);
        boot_message.reason = boot_reason;
        long observer_index = this->m_session_membership.get_observer_channel_index(peer_index);
        if (observer_index != -1)
        {
            c_network_observer* observer = this->m_observer;
            if (observer->observer_channel_connected(this->observer_owner(), observer_index))
                this->m_observer->observer_channel_send_message(this->observer_owner(), observer_index, false, _network_message_type_session_boot, sizeof(s_network_message_session_boot), &boot_message);
            this->m_observer->observer_channel_send_message(this->observer_owner(), observer_index, true, _network_message_type_session_boot, sizeof(s_network_message_session_boot), &boot_message);
        }
        this->m_session_membership.remove_peer(peer_index);
    }
}

const char* c_network_session::get_type_string(e_network_session_type session_type)
{
    if (VALID_INDEX(session_type, k_network_session_type_count))
        return k_session_type_strings[session_type];
    else
        return "<unknown>";
}

const char* c_network_session::get_state_string()
{
    return k_session_state_strings[this->current_local_state()];
}

const char* c_network_session::get_peer_description(long peer_index)
{
    static char peer_description[0x4B];
    if (this->established() && peer_index != -1 && this->m_session_membership.is_peer_valid(peer_index) && !this->m_session_membership.get_peer(peer_index)->properties.peer_name.is_empty())
        csnzprintf(peer_description, 0x4B, "#%02d:%S:%s", peer_index, this->m_session_membership.get_peer(peer_index)->properties.peer_name.get_string(), transport_secure_address_get_mac_string(this->m_session_membership.get_peer_address(peer_index)));
    else
        csnzprintf(peer_description, 0x4B, "#%02d", peer_index);
    return peer_description;
}

const char* c_network_session::get_id_string()
{
    return managed_session_get_id_string(this->managed_session_index());
}

bool c_network_session::is_peer_joining_this_session()
{
    return DECLFUNC(0x224F0, bool, __thiscall, c_network_session*)(this);
}

void c_network_session::idle()
{
    if (!this->disconnected())
    {
        this->m_session_membership.idle();
        if (this->established() && this->m_session_membership.all_peers_established() && this->m_disconnection_policy.get() == _network_session_disconnection_waiting_for_establishment)
            this->m_disconnection_policy = _network_session_disconnection_allowed;
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
            this->process_pending_joins();
        if (this->is_host())
        {
            if (!this->membership_is_locked())
            {
                for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
                {
                    long observer_channel_index = this->m_session_membership.get_observer_channel_index(i);
                    if (observer_channel_index != NONE)
                    {
                        e_network_observer_owner observer_owner = this->observer_owner();
                        if (this->m_observer->observer_channel_dead(observer_owner, observer_channel_index))
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::idle: [%s] peer [%s]/%s channel died, disbanding\n",
                                this->get_id_string(), this->get_peer_description(i), this->m_observer->get_name(observer_channel_index));
                            this->disband_peer(i);
                        }
                    }
                }

                if (this->established() && this->session_type() == _network_session_type_squad)
                {
                    long private_slot_count = 0;
                    long public_slot_count = 0;
                    bool friends_only = false;
                    long maximum_player_count = 0;
                    if (this->get_session_parameters()->session_size.get_allowed())
                    {
                        maximum_player_count = this->get_maximum_player_count();
                        if (maximum_player_count < private_slot_count)
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::idle: maximum session player count is smaller than the set of players already in the session, %d < %d\n",
                                maximum_player_count, private_slot_count);
                            maximum_player_count = private_slot_count;
                        }
                    }
                    if (this->get_session_parameters()->privacy_mode.get_allowed())
                    {
                        s_network_session_privacy_mode* privacy_mode_parameter = this->get_session_parameters()->privacy_mode.get();
                        e_network_session_closed_status closed_mode = privacy_mode_parameter->closed_mode;
                        bool is_closed_by_user = privacy_mode_parameter->is_closed_by_user;
                        if (closed_mode == _network_session_closed_none && !is_closed_by_user)
                        {
                            e_network_game_privacy privacy_mode = privacy_mode_parameter->privacy_mode;
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
                    if (this->m_session_membership.private_slot_count() != private_slot_count || this->m_session_membership.public_slot_count() != public_slot_count || this->m_session_membership.friends_only() != friends_only)
                        this->m_session_membership.set_slot_counts(private_slot_count, public_slot_count, friends_only);
                }
            }
        }
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        else if (this->established())
        {
            long observer_channel_index = this->m_session_membership.get_host_observer_channel_index();
            if (this->m_observer->observer_channel_dead(this->observer_owner(), observer_channel_index))
                this->handle_disconnection();
            if (this->established())
                this->check_to_send_time_synchronization();
        }
        this->idle_observer_state();
        // ==== SECTION CUT FROM CLIENT BUILDS ====
        if (this->is_host())
            this->check_to_send_membership_update();
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        this->get_session_parameters()->update();
        if (this->get_session_parameters()->session_size.set_allowed())
        {
            c_network_session_membership* membership = this->get_session_membership();
            long player_count = membership->get_player_count();
            if (player_count > this->get_session_parameters()->session_size.get_max_player_count())
                this->get_session_parameters()->session_size.set_max_player_count(player_count);
        }
    }
}

e_network_session_state c_network_session::current_local_state()
{
    assert(m_local_state >= 0 && m_local_state < k_network_session_state_count);
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
            throw std::runtime_error("unreachable");
            return false;
    }
}

bool c_network_session::membership_is_locked()
{
    assert(is_host());
    bool locked = current_local_state() == _network_session_state_host_disband; /*|| current_local_state() == _network_session_state_host_handoff || current_local_state() == _network_session_state_host_reestablish*/
    assert(established() || locked);
    return locked;
}

c_network_session_membership* c_network_session::get_session_membership()
{
    assert(established());
    assert(m_session_membership.has_membership());
    assert(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
    assert(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
    return &m_session_membership;
}

void c_network_session::idle_peer_creating()
{
    DECLFUNC(0x3E800, void, __thiscall, c_network_session*)(this);
}

void c_network_session::idle_peer_joining()
{
    DECLFUNC(0x3E8B0, void, __thiscall, c_network_session*)(this);
}

void c_network_session::idle_peer_join_abort()
{
    DECLFUNC(0x3EA00, void, __thiscall, c_network_session*)(this);
}

void c_network_session::idle_peer_leaving()
{
    DECLFUNC(0x3EA60, void, __thiscall, c_network_session*)(this);
}

void c_network_session::process_pending_joins()
{
    e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
    short managed_session_status = 0;
    managed_session_get_status(&managed_session_status, this->managed_session_index());
    if ((managed_session_status & 0x40) != 0 || ((byte)managed_session_status) < 0)
    {
        managed_session_reset_players_add_status(this->managed_session_index());
        if (player_identifier_is_valid(&this->m_player_we_are_adding.player_identifier))
        {
            refuse_reason = _network_join_refuse_reason_player_add_failed;
            if ((managed_session_status & 0x40) != 0)
            {
                refuse_reason = this->can_accept_player_join_request(&this->m_player_we_are_adding.player_identifier, &this->m_player_we_are_adding.secure_address, this->m_player_we_are_adding.peer_output_user_index, true);
                if (refuse_reason == _network_join_refuse_reason_none && !this->m_session_membership.is_player_in_player_add_queue(&this->m_player_we_are_adding.player_identifier))
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] player '%s' went missing from our add queue\n",
                        this->get_id_string(),
                        player_identifier_get_string(&this->m_player_we_are_adding.player_identifier));
                    refuse_reason = _network_join_refuse_reason_player_add_failed;
                }
                if (refuse_reason != _network_join_refuse_reason_none)
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] single player '%s' added but can no longer join because %s, removing\n",
                        this->get_id_string(),
                        player_identifier_get_string(&this->m_player_we_are_adding.player_identifier),
                        network_message_join_refuse_get_reason_string(refuse_reason));
                    managed_session_remove_players(this->managed_session_index(), (qword*)&this->m_player_we_are_adding.player_identifier, 1);
                }
                else
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] single player '%s' added\n",
                        this->get_id_string(),
                        player_identifier_get_string(&this->m_player_we_are_adding.player_identifier));
                    this->m_session_membership.commit_player_from_player_add_queue(&this->m_player_we_are_adding.player_identifier);
                }
            }
            else
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] XSessionJoinRemote failed for single player add\n",
                    this->get_id_string());
            }
            this->finalize_single_player_add(refuse_reason);
        }
        else
        {
            long peer_index = NONE;
            for (peer_index = this->m_session_membership.get_first_peer(); peer_index != NONE; peer_index = this->m_session_membership.get_next_peer(peer_index))
            {
                if (this->m_session_membership.get_peer_connection_state(peer_index) == _network_session_peer_state_joining)
                    break;
            }
            if (peer_index != NONE)
            {
                qword join_nonce = this->m_session_membership.get_join_nonce(peer_index);
                if ((managed_session_status & 0x40) != 0)
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] marking all peers in join [%s] as joined\n",
                        this->get_id_string(),
                        transport_secure_nonce_get_string(join_nonce));
                    for (peer_index = this->m_session_membership.get_first_peer(); peer_index != NONE; peer_index = this->m_session_membership.get_next_peer(peer_index))
                    {
                        if (this->m_session_membership.get_peer_connection_state(peer_index) == _network_session_peer_state_joining && join_nonce == this->m_session_membership.get_join_nonce(peer_index))
                        {
                            if (peer_index == this->m_session_membership.host_peer_index()) // host_peer_index
                                this->m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_waiting);
                            else
                                this->m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_joined);
                        }
                    }
                }
                else
                {
                    if (!this->join_nonce_is_from_clone_join_or_is_hosts(join_nonce) || managed_session_is_master_session(this->managed_session_index()))
                    {
                        if (this->join_nonce_is_from_clone_join_or_is_hosts(join_nonce))
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] players in local host join [%s] could not be added, recreating the session\n",
                                this->get_id_string(),
                                transport_secure_nonce_get_string(join_nonce));
                            for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
                            {
                                if (i == this->m_session_membership.host_peer_index())
                                    this->m_session_membership.set_peer_connection_state(i, _network_session_peer_state_connected);
                                else
                                    this->boot_peer(i, _network_session_boot_reason_recreating_session);
                            }
                            managed_session_reset_session(this->managed_session_index(), true);
                        }
                        else
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] XSessionJoinRemote failed, aborting join\n",
                                this->get_id_string());
                            this->abort_pending_join(join_nonce);
                        }
                    }
                    else
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] players in squad host join [%s] could not be added, disconnecting\n",
                            this->get_id_string(),
                            transport_secure_nonce_get_string(join_nonce));
                        this->disconnect();
                    }
                }
            }
            else
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] we completed a player modify that we didn't request\n",
                    this->get_id_string());
            }
        }
    }
    if (!this->disconnected())
    {
        long unknown1 = 0;
        for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
        {
            e_network_session_peer_state connection_state = this->m_session_membership.get_peer_connection_state(i);
            if (connection_state == _network_session_peer_state_waiting || connection_state == _network_session_peer_state_connected && (managed_session_status & 0x20) == 0)
            {
                unknown1 |= 1 << i;
            }
        }
        long while_loop_counter = 0;
        long unknown2 = 1;
        while (unknown1)
        {
            s_network_session_peer* peer = this->m_session_membership.get_peer(while_loop_counter);
            if ((unknown2 & unknown1) != 0)
            {
                bool peer_states_match = true;
                long unknown6 = ~unknown2 & unknown1;
                long unknown7 = unknown6;
                long unknown8 = unknown6;
                for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
                {
                    if (this->m_session_membership.get_peer(i)->join_nonce == peer->join_nonce)
                    {
                        unknown8 &= ~(1 << i);
                        if (this->m_session_membership.get_current_membership()->peers[i].connection_state != peer->connection_state)
                            peer_states_match = false;
                    }
                }
                unknown7 = unknown8;
                if (peer_states_match)
                {
                    if (peer->connection_state == _network_session_peer_state_connected)
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] adding the join [%s] to the XSession\n",
                            this->get_id_string(),
                            transport_secure_nonce_get_string(peer->join_nonce));
                        this->add_pending_join_to_session(peer->join_nonce);
                        managed_session_status |= 0x20u;
                    }
                    else
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] marking all peers in join [%s] as established\n",
                            this->get_id_string(),
                            transport_secure_nonce_get_string(peer->join_nonce));
                        for (long i = while_loop_counter; i != NONE; i = this->m_session_membership.get_next_peer(i))
                        {
                            if (this->m_session_membership.get_peer(i)->join_nonce == peer->join_nonce)
                                this->m_session_membership.set_peer_connection_state(i, _network_session_peer_state_established);
                        }
                        // m_matchmaking_establish_time would be set here in h3/ms23, which of course we don't have
                    }
                }
                unknown1 = unknown7;
            }
            while_loop_counter++;
            unknown2 *= 2;
        }
    }

    if (!c_network_session::disconnected() && (managed_session_status & 0x20) == 0)
    {
        s_player_add_queue_entry* add_queue_entry = this->m_session_membership.get_first_player_from_player_add_queue();
        bool bool_array[] = { true };
        if (add_queue_entry != nullptr)
        {
            s_network_session_peer* peer = this->m_session_membership.get_peer(add_queue_entry->peer_index);
            this->m_player_we_are_adding.player_identifier = add_queue_entry->player_identifier;
            this->m_player_we_are_adding.secure_address = peer->secure_address;
            this->m_player_we_are_adding.peer_output_user_index = add_queue_entry->peer_index;
            e_network_join_refuse_reason refuse_reason = this->can_accept_player_join_request
            (
                &this->m_player_we_are_adding.player_identifier,
                &this->m_player_we_are_adding.secure_address,
                add_queue_entry->peer_index,
                true
            );
            if (refuse_reason != _network_join_refuse_reason_none)
            {
                this->finalize_single_player_add(refuse_reason);
            }
            else
            {
                qword xuids[] = { user_xuid_from_secure_address(&peer->secure_address) };
                managed_session_add_players(this->managed_session_index(), xuids, bool_array, NUMBEROF(xuids));
            }
        }
    }

    if (!c_network_session::disconnected() && !c_network_session::membership_is_locked())
    {
        long peer_pending_join_timeout = get_network_configuration()->group_peer_pending_join_timeout;
        if (this->session_type() == _network_session_type_squad)
            peer_pending_join_timeout = get_network_configuration()->squad_peer_pending_join_timeout;
        for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
        {
            qword join_nonce = this->m_session_membership.get_join_nonce(i);
            long peer_creation_timestamp = this->m_session_membership.get_creation_timestamp(i);
            long time_since_creation = network_get_time() - peer_creation_timestamp;
            if (this->host_join_nonce_valid()
                && this->join_nonce_is_from_clone_join_or_is_hosts(join_nonce)
                || this->m_session_membership.get_peer_connection_state(i) >= _network_session_peer_state_established
                || peer_pending_join_timeout != NONE
                || time_since_creation < peer_pending_join_timeout)
            {
                if ((!this->host_join_nonce_valid() || !this->join_nonce_is_from_clone_join_or_is_hosts(join_nonce)) && this->m_session_membership.get_peer_connection_state(i) == _network_session_peer_state_disconnected)
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] pending joins for [%s] failed because the peer disconnect, removing\n",
                        this->get_id_string(),
                        transport_secure_nonce_get_string(join_nonce));
                    this->abort_pending_join(join_nonce);
                }
            }
            else
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] pending joins for [%s] timed out (%d msec > %d msec), removing\n",
                    this->get_id_string(),
                    transport_secure_nonce_get_string(join_nonce),
                    time_since_creation,
                    peer_pending_join_timeout);
                this->abort_pending_join(join_nonce);
            }
        }
    }
}

e_network_observer_owner c_network_session::observer_owner()
{
    assert(m_session_index >= 0 && m_session_index < k_network_maximum_sessions);
    return static_cast<e_network_observer_owner>(m_session_index);
}

long c_network_session::get_maximum_player_count()
{
    return DECLFUNC(0x22E80, long, __thiscall, c_network_session*)(this);
}

void c_network_session::handle_disconnection()
{
    printf("MP/NET/SESSION,CTRL: c_network_session::handle_disconnection: [%s] disconnected from session host. Session Type %s\n",
        this->get_id_string(),
        get_type_string(this->m_session_type.get()));
    hf2p_handle_disconnection(); // unknown saber function, wasn't in ms23
    this->disconnect();
}

void c_network_session::check_to_send_time_synchronization()
{
    DECLFUNC(0x22B50, void, __thiscall, c_network_session*)(this);
}

void c_network_session::idle_observer_state()
{
    DECLFUNC(0x3EAC0, void, __thiscall, c_network_session*)(this);
}

void c_network_session::check_to_send_membership_update()
{
    for (long peer_index = this->m_session_membership.get_first_peer(); peer_index != NONE; peer_index = m_session_membership.get_next_peer(peer_index))
    {
        long peer_channel_index = this->m_session_membership.get_observer_channel_index(peer_index);
        long peer_update_number = this->m_session_membership.get_membership_update_number(peer_index);
        bool peer_needs_reestablishment = this->m_session_membership.get_peer_needs_reestablishment(peer_index);
        if (peer_channel_index != NONE && this->m_session_membership.get_peer_connection_state(peer_index) >= _network_session_peer_state_connected && peer_needs_reestablishment != true)
        {
            c_network_observer* observer = this->m_observer;
            c_network_observer::s_channel_observer* channel_observer = &observer->m_channel_observers[peer_channel_index];
            if (observer->observer_channel_connected(this->observer_owner(), peer_channel_index))
            {
                if (peer_update_number != this->m_session_membership.update_number())
                {
                    if (observer->observer_channel_backlogged(this->observer_owner(), peer_channel_index, _network_message_type_membership_update))
                    {
                        observer->observer_channel_set_waiting_on_backlog(this->observer_owner(), peer_channel_index, _network_message_type_membership_update);
                    }
                    else
                    {
                        // create new shared membership containing only the host and target peer
                        // HO uniquely doesn't sync other peer information with peers, likely as host migration was removed
                        s_network_session_shared_membership shared_membership = *this->m_session_membership.get_current_membership();
                        shared_membership.leader_peer_index = shared_membership.host_peer_index;
                        shared_membership.peer_count = 2;
                        shared_membership.peer_valid_mask.clear();
                        
                        // invalidate non host && target peers
                        shared_membership.peer_valid_mask.set(shared_membership.host_peer_index, true);
                        shared_membership.peer_valid_mask.set(peer_index, true);

                        // remove all invalid peers
                        for (long peer_index = 0; peer_index < k_network_maximum_machines_per_session; peer_index++)
                        {
                            if (!shared_membership.peer_valid_mask.test(peer_index))
                                memset(&shared_membership.peers[peer_index], 0, sizeof(s_network_session_peer));
                        }

                        // transfer ownership of all players not belonging to the target peer to the host peer
                        for (long shared_player_index = 0; shared_player_index < k_network_maximum_players_per_session; shared_player_index++)
                        {
                            // if the current player is valid && doesn't belong to the target peer
                            if (shared_membership.player_valid_mask.test(shared_player_index) && shared_membership.players[shared_player_index].peer_index != peer_index)
                            {
                                // add the current player to the host peer's player mask && set the current player's peer index to host's
                                shared_membership.peers[shared_membership.host_peer_index].player_mask.set(shared_player_index, true);
                                shared_membership.players[shared_player_index].peer_index = shared_membership.host_peer_index;
                            }
                        }
                        
                        bool send_complete_update = false;
                        s_network_session_shared_membership* transmitted_membership = this->m_session_membership.get_transmitted_membership(peer_index);
                        if (peer_update_number == -1 || peer_update_number != transmitted_membership->update_number)
                        {
                            send_complete_update = true;
                            transmitted_membership = nullptr;
                        }
                        s_network_message_membership_update membership_update_message;
                        this->m_session_membership.build_membership_update(peer_index, &shared_membership, transmitted_membership, &membership_update_message);
                        if (send_complete_update)
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::check_to_send_membership_update: [%s] sending complete update #-1->[#%d] to peer [#%d]\n",
                                this->get_id_string(),
                                this->m_session_membership.update_number(),
                                peer_index);
                        }
                        else
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::check_to_send_membership_update: [%s] sending incremental update [#%d]->[#%d] to peer [#%d]\n",
                                this->get_id_string(),
                                peer_update_number,
                                this->m_session_membership.update_number(),
                                peer_index);
                        }
                        if (this->m_observer->observer_channel_connected(this->observer_owner(), peer_channel_index))
                        {
                            if (observer->observer_channel_backlogged(this->observer_owner(), peer_channel_index, _network_message_type_membership_update))
                            {
                                observer->observer_channel_set_waiting_on_backlog(this->observer_owner(), peer_channel_index, _network_message_type_membership_update);
                            }
                            else
                            {
                                this->m_session_membership.set_membership_update_number(peer_index, this->m_session_membership.update_number());
                                long observer_index = this->m_session_membership.get_observer_channel_index(peer_index);
                                if (observer_index != -1)
                                {
                                    // send_message_to_peer
                                    observer->observer_channel_send_message(this->observer_owner(), observer_index, false, _network_message_type_membership_update, sizeof(s_network_message_membership_update), &membership_update_message);
                                }
                            }
                        }
                        this->m_session_membership.copy_current_to_transmitted(peer_index, &shared_membership);
                    }
                }
            }
        }
    }
}

long c_network_session::managed_session_index()
{
    assert(m_managed_session_index != NONE);
    return m_managed_session_index;
}

bool c_network_session::join_abort(s_transport_address const* incoming_address, qword join_nonce)
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

bool c_network_session::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
    if (this->established() && this->is_host())
    {
        long channel_index = this->m_observer->observer_channel_find_by_network_channel(this->observer_owner(), channel);
        long peer_index = this->m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == this->m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_properties: [%s] peer-properties received from invalid peer [#%d]\n",
                this->get_id_string(),
                peer_index);
            return false;
        }
        else
        {
            if (this->m_session_membership.get_peer_connection_state(peer_index) == _network_session_peer_state_joined)
                this->m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_waiting);
            this->m_session_membership.set_peer_address(peer_index, &message->secure_address);
            this->m_session_membership.set_peer_properties(peer_index, &message->peer_properties);
            return true;
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_properties: [%s] peer-properties received but not host, can't update properties (state %s)\n",
            this->get_id_string(),
            this->get_state_string());
        return false;
    }

    return false;
}

bool c_network_session::peer_request_properties_update(s_transport_secure_address const* secure_address, s_network_session_peer_properties const* peer_properties)
{
    if (this->established())
    {
        if (this->is_host())
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::peer_request_properties_update: [%s] applying peer-properties locally\n", this->get_id_string());
            this->m_session_membership.set_peer_address(this->m_session_membership.local_peer_index(), secure_address);
            this->m_session_membership.set_peer_properties(this->m_session_membership.local_peer_index(), peer_properties);
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::peer_request_properties_update: [%s] sending peer-properties request\n", this->get_id_string());
            s_network_message_peer_properties message = {};
            this->get_session_id(&message.session_id);
            message.secure_address = *secure_address;
            memcpy(&message.peer_properties, peer_properties, sizeof(s_network_session_peer_properties));
            long channel_index = this->m_session_membership.m_peer_channels[this->m_session_membership.host_peer_index()].channel_index;
            if (channel_index != NONE)
                this->m_observer->observer_channel_send_message(this->observer_owner(), channel_index, false, _network_message_type_peer_properties, sizeof(s_network_message_peer_properties), &message);
        }
        return true;
    }
    return false;
}

c_network_session_membership* c_network_session::get_session_membership_unsafe()
{
    if (!this->disconnected() && this->m_session_membership.has_membership())
    {
        assert(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
        assert(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
        return &this->m_session_membership;
    }
    return nullptr;
}

bool c_network_session::join_nonce_is_from_clone_join_or_is_hosts(qword join_nonce)
{
    return join_nonce == this->m_host_join_nonce;
}

void c_network_session::finalize_single_player_add(e_network_join_refuse_reason refuse_reason)
{
    if (refuse_reason != _network_join_refuse_reason_none)
        this->m_session_membership.remove_player_from_player_add_queue(&this->m_player_we_are_adding.player_identifier);
    if (this->m_player_we_are_adding.peer_output_user_index == this->m_session_membership.host_peer_index())
    {
        if (refuse_reason != _network_join_refuse_reason_none)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: Warning [%s] host player '%s' add failed because of '%s'\n",
                this->get_id_string(),
                player_identifier_get_string(&this->m_player_we_are_adding.player_identifier),
                network_message_join_refuse_get_reason_string(refuse_reason));
            this->m_local_user_player_add.refuse_reason = refuse_reason;
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: [%s] host player '%s' add succeeded\n",
                this->get_id_string(),
                player_identifier_get_string(&this->m_player_we_are_adding.player_identifier));
            this->m_local_user_player_add.refuse_reason = _network_join_refuse_reason_none;
        }
    }
    else
    {
        s_network_message_player_refuse message = {};
        this->get_session_id(&message.session_id);
        long channel_index = this->m_session_membership.get_observer_channel_index(this->m_player_we_are_adding.peer_output_user_index);
        if (channel_index != -1)
            this->m_observer->observer_channel_send_message(this->observer_owner(), channel_index, false, _network_message_type_player_refuse, sizeof(s_network_message_player_refuse), &message);
        if (refuse_reason != _network_join_refuse_reason_none)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: Warning. [%s] informing peer %d that its player '%s' add failed because '%s'\n",
                this->get_id_string(),
                this->m_player_we_are_adding.peer_output_user_index,
                player_identifier_get_string(&this->m_player_we_are_adding.player_identifier),
                network_message_join_refuse_get_reason_string(refuse_reason));
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: [%s] informing peer %d that its player '%s' add succeeded\n",
                this->get_id_string(),
                this->m_player_we_are_adding.peer_output_user_index,
                player_identifier_get_string(&this->m_player_we_are_adding.player_identifier));
        }
    }
    memset(&this->m_player_we_are_adding.player_identifier, 0, sizeof(s_player_identifier));
    memset(&this->m_player_we_are_adding.secure_address, 0, sizeof(s_transport_secure_address));
    this->m_player_we_are_adding.peer_output_user_index = 0;
}

e_network_session_type c_network_session::session_type()
{
    assert(m_session_type >= 0 && m_session_type < k_network_session_type_count);
    return m_session_type.get();
}

bool c_network_session::host_join_nonce_valid()
{
    return this->m_host_join_nonce != -1;
}

void c_network_session::add_pending_join_to_session(qword join_nonce)
{
    bool players_left[k_network_maximum_players_per_session] = {};
    qword player_xuids[k_network_maximum_players_per_session] = {};
    long xuid_count = 0;
    for (long i = this->m_session_membership.get_first_peer(); i != NONE; i = this->m_session_membership.get_next_peer(i))
    {
        if (this->m_session_membership.get_join_nonce(i) == join_nonce)
        {
            long player_index = this->m_session_membership.get_peer(i)->get_player_index();
            if (player_index != NONE)
            {
                player_xuids[xuid_count] = this->m_session_membership.get_player(player_index)->configuration.host.user_xuid;
                players_left[xuid_count] = !this->m_session_membership.get_player(player_index)->left_game;
                xuid_count++;
            }
            this->m_session_membership.set_peer_connection_state(i, _network_session_peer_state_joining);
        }
    }
    managed_session_add_players(this->managed_session_index(), player_xuids, players_left, xuid_count);
}

long c_network_session::session_index()
{
    assert(m_session_index >= 0 && m_session_index < k_network_maximum_sessions);
    return m_session_index;
}

bool c_network_session::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
    long channel_index = this->m_observer->observer_channel_find_by_network_channel(this->observer_owner(), channel);
    long peer_index = this->m_session_membership.get_peer_from_observer_channel(channel_index);
    if (this->is_host())
    {
        if (peer_index == NONE && peer_index == this->m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received as host from invalid peer [#%d] (range [0,%d-1] us [#%d])\n",
                this->get_id_string(),
                peer_index,
                this->m_session_membership.get_peer_count(),
                this->m_session_membership.local_peer_index());
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received from peer [%s]\n",
                this->get_id_string(),
                this->get_peer_description(peer_index));
            this->m_session_membership.set_peer_needs_reestablishment(peer_index, false);
            return true;
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received but not host (%s)\n",
            this->get_id_string(),
            this->get_state_string());
    }

    if ((this->is_host()) || (channel_index == NONE))
        return false;

    s_network_message_host_decline decline_message = {};
    this->get_session_id(&decline_message.session_id);
    decline_message.peer_exists = peer_index != NONE;
    decline_message.session_exists = true;
    if (this->established())
    {
        decline_message.host_exists = true;
        decline_message.host_address = *this->m_session_membership.get_peer_address(this->m_session_membership.host_peer_index());
    }
    this->m_observer->observer_channel_send_message(this->observer_owner(), channel_index, false, _network_message_type_host_decline, sizeof(s_network_message_host_decline), &decline_message);
    return true;
}

bool c_network_session::leaving_session()
{
    switch (this->current_local_state())
    {
        case _network_session_state_peer_join_abort:
        case _network_session_state_peer_leaving:
        case _network_session_state_host_disband:
            return true;
        default:
            return false;;
    }
}

void c_network_session::time_set(ulong time)
{
    this->m_time_exists = true;
    this->m_time = time;
}

bool c_network_session::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
    if (this->established() && this->is_host())
    {
        long channel_index = this->m_observer->observer_channel_find_by_network_channel(this->observer_owner(), channel);
        long peer_index = this->m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == this->m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received from invalid peer [#%d]\n",
                this->get_id_string(),
                peer_index);
            return false;
        }
        else
        {
            long player_index = this->m_session_membership.get_current_membership()->peers[peer_index].get_player_index();
            if (player_index == NONE)
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received but no player associated with peer [#%d]\n",
                    this->get_id_string(),
                    peer_index);
                return false;
            }
            else
            {
                this->m_session_membership.set_player_properties(player_index, message->player_update_number, message->controller_index, &message->player_from_client, message->player_voice);
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties accepted for peer/player [#%d]/[#%d]\n",
                    this->get_id_string(),
                    peer_index,
                    player_index);
                return true;
            }
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received but not host, can't update players\n",
            this->get_id_string());
        return false;
    }

    return false;
}

bool c_network_session::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
    if (this->established() && this->is_host())
    {
        long channel_index = this->m_observer->observer_channel_find_by_network_channel(this->observer_owner(), channel);
        long peer_index = this->m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == this->m_session_membership.local_peer_index())
        {
            printf("MP/NET/SESSION,PARAMS: c_network_session::handle_parameters_request: [%s] parameters-request received from invalid peer %d\n",
                this->get_id_string(),
                peer_index);
        }
        else
        {
            if (this->get_session_parameters()->handle_change_request((peer_index == this->m_session_membership.leader_peer_index()), message))
            {
                return true;
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_parameters_request: [%s] failed to handle parameters-request received from peer %d\n",
                    this->get_id_string(),
                    peer_index);
            }
        }
    }
    else
    {
        printf("MP/NET/SESSION,PARAMS: c_network_session::handle_parameters_request: [%s] parameters-request received and we are not in a state to handle [%s]\n",
            this->get_id_string(),
            this->get_state_string());
    }
    return false;
}

bool c_network_session::compare_session_id(s_transport_secure_identifier const* secure_id)
{
    return managed_session_compare_id(this->managed_session_index(), secure_id);
}

bool c_network_session::get_session_id(s_transport_secure_identifier* secure_id)
{
    return managed_session_get_id(this->managed_session_index(), secure_id);
}

void c_network_session::peer_request_player_add(const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings)
{
    DECLFUNC(0x21EF0, void, __thiscall, c_network_session*, const s_player_identifier*, long user_index, long controller_index, s_player_configuration_from_client*, long)(this, player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}

long c_network_session::get_session_membership_update_number()
{
    return this->m_session_membership.m_baseline.update_number;
}

c_network_session_membership* c_network_session::get_session_membership_for_update()
{
    assert(this->established());
    assert(this->is_host());
    assert(m_session_membership.has_membership());
    assert(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
    assert(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
    return &m_session_membership;
}

long c_network_session::find_peer_by_machine_identifier(s_machine_identifier const* machine_identifier)
{
    return m_session_membership.get_peer_from_unique_identifier(&machine_identifier->unique_identifiers);
}

bool c_network_session::host_boot_machine(long peer_index, e_network_session_boot_reason reason)
{
    if (is_host())
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::host_boot_machine: [%s] booting machine [#%d] locally [reason %d]\n",
            managed_session_get_id_string(m_managed_session_index),
            peer_index,
            reason);
        if (m_session_membership.local_peer_index() == peer_index)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::host_boot_machine: we are the host and are being booted, leaving...\n");
            // halox no longer used & gutted, we won't need this call
            //user_interface_networking_notify_booted_from_session(m_session_type, reason);
            initiate_leave_protocol(false);
        }
        else
        {
            boot_peer(peer_index, reason);
        }
        return true;
    }
    return false;
}

void c_network_session::initiate_leave_protocol(bool leave_immediately)
{
    DECLFUNC(0x21C30, void, __thiscall, c_network_session*, bool)(this, leave_immediately);
}

bool c_network_session::handle_leave_request(s_transport_address const* outgoing_address)
{
    assert(is_host());
    long peer_index = m_session_membership.get_peer_from_incoming_address(outgoing_address);
    if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_leave_request: [%s] leave-request received from an incorrect peer [%s] (invalid or local)\n",
            managed_session_get_id_string(m_managed_session_index),
            get_peer_description(peer_index));
        return true;
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_leave_request: [%s] leave-request received from peer [%s]\n",
            managed_session_get_id_string(m_managed_session_index),
            get_peer_description(peer_index));
        return handle_leave_internal(peer_index);
    }
}

bool c_network_session::handle_leave_internal(long peer_index)
{
    assert(is_host());
    assert(established());
    assert(peer_index != NONE);
    assert(peer_index != m_session_membership.local_peer_index());
    long channel_index = m_session_membership.get_observer_channel_index(peer_index);
    if (m_session_class == _network_session_class_xbox_live)
    {
        m_observer->quality_statistics_notify_peer_left_gracefully(observer_owner(), channel_index);
    }
    if (membership_is_locked() || !m_session_membership.get_current_membership()->peer_valid_mask.test(peer_index))
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::handle_leave_internal: Warning. [%s] leave-request from peer [%s] denied, session membership is locked (state %s)\n",
            managed_session_get_id_string(m_managed_session_index),
            get_peer_description(peer_index),
            get_state_string());
        return false;
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::handle_leave_internal: %s leave-request accepted for peer [%s]\n",
            managed_session_get_id_string(m_managed_session_index),
            get_peer_description(peer_index));
        s_network_message_leave_acknowledge message;
        csmemset(&message, 0, sizeof(message));
        managed_session_get_id(m_managed_session_index, &message.session_id);
        m_observer->observer_channel_send_message(observer_owner(), channel_index, true, _network_message_type_leave_acknowledge, sizeof(message), &message);
        return true;
    }
}