#include "network_session.h"
#include <iostream>
#include <networking\messages\network_message_gateway.h>
#include <networking\network_utilities.h>
#include <networking\logic\network_join.h>
#include <networking\session\network_managed_session.h>
#include <networking\transport\transport_shim.h>
#include <hf2p\hf2p_session.h>
#include <networking\network_configuration.h>
#include <cseries\cseries.h>
#include <anvil\backend\user.h>
#include <anvil\backend\lobby.h>
#include <anvil\backend\services\private_service.h>
#include <game\game.h>
#include <networking\messages\network_messages_session_protocol.h>
#include <networking\messages\network_messages_session_membership.h>
#include <game\player_mapping.h>

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

bool c_network_session::acknowledge_join_request(transport_address const* address, e_network_join_refuse_reason reason)
{
    printf("MP/NET/SESSION,CTRL: c_network_session::acknowledge_join_request: [%s] join failed, sending refusal (%s) to '%s'\n",
        get_id_string(),
        network_message_join_refuse_get_reason_string(reason),
        transport_address_get_string(address));
    
    s_network_message_join_refuse message = {};
    get_session_id(&message.session_id);
    message.reason = reason;
    return m_message_gateway->send_message_directed(address, _network_message_join_refuse, sizeof(s_network_message_join_refuse), &message);
}

// TODO: ensure that each peer matches a session ID within the API lobby to prevent random external joins coming in from outside of our matchmaking system
bool c_network_session::handle_join_request(transport_address const* address, s_network_message_join_request const* message)
{
    long executable_type;
    long minimum_network_version;
    long current_network_version;
    network_get_build_identifiers(&executable_type, &minimum_network_version, &current_network_version);

    if (message->current_network_version >= current_network_version)
    {
        if (minimum_network_version >= message->minimum_network_version)
        {
            if (executable_type == message->executable_type)
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] received for %d peers, adding to the join queue\n",
                    get_id_string(),
                    transport_secure_nonce_get_string(message->join_request.join_nonce),
                    message->join_request.joining_peer_count);
                network_join_add_join_to_queue(this, address, &message->join_request);
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] executable type mismatch, peer: %d, host: %d\n",
                    get_id_string(),
                    transport_secure_nonce_get_string(message->join_request.join_nonce),
                    message->executable_type, executable_type);
                acknowledge_join_request(address, _network_join_refuse_reason_executable_type_mismatch);
            }
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
                get_id_string(),
                transport_secure_nonce_get_string(message->join_request.join_nonce),
                message->minimum_network_version, message->current_network_version, current_network_version, minimum_network_version);
            acknowledge_join_request(address, _network_join_refuse_reason_host_version_too_low);
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_join_request: [%s] join-request [%s] version mismatch, peer: %d->%d, host: %d->%d\n",
            get_id_string(),
            transport_secure_nonce_get_string(message->join_request.join_nonce),
            message->minimum_network_version, message->current_network_version, current_network_version, minimum_network_version);
        acknowledge_join_request(address, _network_join_refuse_reason_peer_version_too_low);
    }

    return true;
}

bool c_network_session::handle_peer_connect(transport_address const* outgoing_address, s_network_message_peer_connect const* message)
{
    return DECLFUNC(0x4B2E0, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_peer_connect const*)(this, outgoing_address, message);
}

bool c_network_session::handle_session_disband(transport_address const* outgoing_address, s_network_message_session_disband const* message)
{
    return DECLFUNC(0x4B4D0, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_session_disband const*)(this, outgoing_address, message);
}

bool c_network_session::handle_session_boot(transport_address const* outgoing_address, s_network_message_session_boot const* message)
{
    return DECLFUNC(0x4B560, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_session_boot const*)(this, outgoing_address, message);
}

bool c_network_session::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
    return DECLFUNC(0x4B5F0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_host_decline const*)(this, channel, message);
}

bool c_network_session::handle_time_synchronize(transport_address const* outgoing_address, s_network_message_time_synchronize const* message)
{
    if (!established())
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] not established, can't synchronize time\n",
            get_id_string());
        return false;
    }

    if (is_host())
    {
        long peer_index = m_session_membership.get_peer_from_incoming_address(outgoing_address);
        if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time authority received synchronization from invalid peer index [#%d]\n",
                get_id_string(),
                peer_index);
            return false;
        }
        else if (message->synchronization_stage != _network_time_synchronize_stage_initiate)
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time authority received synchronization stage %d (expected 0)\n",
                get_id_string(),
                message->synchronization_stage);
            return false;
        }
        else if (leaving_session())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] ignoring time request from peer [%s] (currently leaving session)\n",
                get_id_string(),
                get_peer_description(peer_index));
            return false;
        }

        s_network_message_time_synchronize sync_message = {};
        sync_message.synchronization_stage = message->synchronization_stage;
        sync_message.session_id = message->session_id;
        sync_message.client_timestamp[0] = message->client_timestamp[0];
        sync_message.client_timestamp[1] = message->client_timestamp[1];
        sync_message.authority_timestamp[0] = message->authority_timestamp[0];
        sync_message.authority_timestamp[1] = message->authority_timestamp[1];
        sync_message.synchronization_stage = _network_time_synchronize_stage_reply;
        m_message_gateway->send_message_directed(outgoing_address, _network_message_time_synchronize, sizeof(s_network_message_time_synchronize), &sync_message);
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] replying to time request from peer [%s]\n",
            get_id_string(),
            get_peer_description(peer_index));
        return true;
    }
    else if (m_session_membership.host_exists_at_incoming_address(outgoing_address))
    {
        if (message->synchronization_stage != _network_time_synchronize_stage_reply)
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] time client received synchronization stage %d (expected 1)\n",
                get_id_string(),
                message->synchronization_stage);
            return false;
        }

        long time_delta1 = network_time_difference_msec(message->client_timestamp[0], message->authority_timestamp[0]);
        long time_delta2 = network_time_difference_msec(message->client_timestamp[1], message->authority_timestamp[1]);
        long synchronized_time_offset = (time_delta1 + time_delta2) / 2;
        long synchronized_time_epsilon = network_time_difference_msec(synchronized_time_offset, time_delta1);
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] synchronized offset %dmsec (epsilon %dmsec)\n",
            get_id_string(),
            synchronized_time_offset,
            abs(synchronized_time_epsilon));
        time_set(synchronized_time_offset);
        m_time_synchronization_end_time = network_time_get();
        return true;
    }

    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_time_synchronize: [%s] client received from someone other than authority\n",
        get_id_string());
    return false;
}

bool c_network_session::channel_is_authoritative(c_network_channel* channel)
{
    return DECLFUNC(0x227A0, bool, __thiscall, c_network_session*, c_network_channel*)(this, channel);
}

bool c_network_session::handle_membership_update(s_network_message_membership_update const* message)
{
    return m_session_membership.handle_membership_update(message);
}

bool c_network_session::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
    return DECLFUNC(0x4B7C0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_refuse const*)(this, channel, message);
}

bool c_network_session::handle_parameters_update(s_network_message_parameters_update const* message)
{
    return DECLFUNC(0x4B3D0, bool, __thiscall, c_network_session*, s_network_message_parameters_update const*)(this, message);
}

e_network_join_refuse_reason c_network_session::can_accept_any_join_request() const
{
    if (!get_session_parameters()->m_parameters.privacy_mode.get_allowed())
    {
        return _network_join_refuse_reason_none;
    }

    const s_network_session_privacy_mode* privacy_mode = get_session_parameters()->m_parameters.privacy_mode.get();
    switch (privacy_mode->closed_mode)
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
    {
        return _network_join_refuse_reason_session_full;
    }

    return _network_join_refuse_reason_none;
}

void c_network_session::join_accept(s_network_session_join_request const* join_request, transport_address const* address)
{
    ASSERT(join_request);
    ASSERT(established() && is_host());
    ASSERT(!membership_is_locked());

    printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] processing join request from %s\n",
        get_id_string(),
        transport_address_get_string(address));

    e_network_join_refuse_reason refuse_reason = can_accept_join_request(join_request);
    for (long joining_peer_index = 0; joining_peer_index < join_request->joining_peer_count; joining_peer_index++)
    {
        long session_peer_index = m_session_membership.get_peer_from_secure_address(&join_request->joining_peers[joining_peer_index].joining_peer_address);
        
        // ensure the joining peer doesn't already have membership in the session
        if (session_peer_index != NONE)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] peer tried to join a session that it's already in\n",
                get_id_string());
            refuse_reason = _network_join_refuse_reason_address_invalid;
            break;
        }

        printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] -- joining with peer %s\n",
            get_id_string(),
            transport_secure_address_get_string(&join_request->joining_peers[joining_peer_index].joining_peer_address));
        
        // Search for available peer index to assign to new join
        for (long available_peer_index = 0; available_peer_index < k_network_maximum_machines_per_session; available_peer_index++)
        {
            // if peer is invalid
            if (!m_session_membership.is_peer_valid(available_peer_index))
            {
                session_peer_index = available_peer_index;
                break;
            }
        }
        if (session_peer_index == NONE)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. Failed to find available peer index -- this should always work\n");
            refuse_reason = _network_join_refuse_reason_session_full;
            break;
        }

        s_transport_session_description session_description{};
        if (!managed_session_get_security_information(managed_session_index(), &session_description, nullptr) ||
            !m_session_membership.add_peer(session_peer_index, _network_session_peer_state_reserved, join_request->joining_peers[joining_peer_index].joining_network_version_number,
                &join_request->joining_peers[joining_peer_index].joining_peer_address, join_request->party_nonce, join_request->join_nonce))
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] host failed to create observer when adding peer [%s] at peer #%d\n",
                get_id_string(),
                transport_secure_address_get_string(&join_request->joining_peers[joining_peer_index].joining_peer_address),
                session_peer_index);
            refuse_reason = _network_join_refuse_reason_too_many_observers;
            break;
        }

        XNetAddEntry(address, &join_request->joining_peers[joining_peer_index].joining_peer_address, &session_description.session_id);
        m_observer->observer_channel_initiate_connection(observer_owner(), m_session_membership.get_observer_channel_index(session_peer_index));

        // if not refused
        if (refuse_reason == _network_join_refuse_reason_none)
        {
            long user_player_index = join_request->joining_peers[joining_peer_index].user_player_index;
            if (user_player_index != NONE)
            {
                long player_index = m_session_membership.find_or_add_player(session_peer_index, &join_request->joining_players[user_player_index].joining_peer_player_id, join_request->join_to_public_slots);
                
                s_player_configuration player_config;
                // called by above constructor
                //csmemset(&player_config.client, 0, sizeof(s_player_configuration_from_client));
                //player_config.host.player_appearance.emblem_info = s_emblem_info();
                csmemset(&player_config, 0, sizeof(s_player_configuration));
                player_config.client.user_selected_team_index = _game_team_none;
                player_config.client.selected_loadout_index = NONE;
                player_config.host.user_xuid = user_xuid_from_secure_address(&join_request->joining_peers[joining_peer_index].joining_peer_address);
                player_config.host.team_index = _game_team_none;
                player_config.host.assigned_team_index = _game_team_none;

                m_session_membership.update_player_data(player_index, &player_config);
            }
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: [%s] '%s' session accepted a new peer %s with %d users\n",
                get_id_string(),
                get_type_string(session_type()),
                get_peer_description(session_peer_index),
                m_session_membership.get_peer(session_peer_index)->player_mask.count_bits_set());
        }
    }

    if (!refuse_reason)
    {
        long peer_index = m_session_membership.get_peer_from_incoming_address(address);
        if (peer_index == NONE)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Warning. [%s] lost connect to peer %s during join_accept\n",
                get_id_string(),
                get_peer_description(peer_index));
            refuse_reason = _network_join_refuse_reason_address_invalid;
        }
        else
        {
            long observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
            if (observer_channel_index == NONE)
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::join_accept: Error. [%s] peer %s has invalid observer channel index\n",
                    get_id_string(),
                    get_peer_description(peer_index));
                refuse_reason = _network_join_refuse_reason_too_many_observers;
            }
            else
            {
                if (m_session_membership.get_peer_connection_state(peer_index) == _network_session_peer_state_reserved)
                {
                    m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_connected);
                }
                m_observer->observer_channel_initiate_connection(observer_owner(), observer_channel_index);
            }
        }
    }

    if (refuse_reason)
    {
        acknowledge_join_request(address, refuse_reason);
        abort_pending_join(join_request->join_nonce);
    }
}

e_network_join_refuse_reason c_network_session::can_accept_join_request(s_network_session_join_request const* join_request)
{
    if (!is_host())
    {
        return _network_join_refuse_reason_not_joinable;
    }
    if (!join_allowed_by_privacy())
    {
        return _network_join_refuse_reason_session_privacy;
    }

    e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
    long joining_peer_count = join_request->joining_peer_count;
    long joining_player_count = 0;
    bool has_players = false;

    for (long i = 0; i < join_request->joining_peer_count; i++)
    {
        long peer_index = m_session_membership.get_peer_from_secure_address(&join_request->joining_peers[i].joining_peer_address);
        long user_player_index = join_request->joining_peers[i].user_player_index;
        if (player_identifier_is_valid(&join_request->joining_players[user_player_index].joining_peer_player_id))
        {
            joining_player_count++;
            has_players = true;
            refuse_reason = can_accept_player_join_request(&join_request->joining_players[user_player_index].joining_peer_player_id, &join_request->joining_peers[i].joining_peer_address, peer_index, false);
        }
        else
        {
            has_players = false;
        }

        if (refuse_reason == _network_join_refuse_reason_none)
        {
            if (peer_index != NONE)
            {
                joining_peer_count--;
                long peer_player_count = m_session_membership.get_peer(peer_index)->player_mask.count_bits_set();
                has_players = true;
                joining_player_count -= peer_player_count;
            }
            if (!has_players)
            {
                refuse_reason = _network_join_refuse_reason_player_count_zero;
            }
        }
        if (refuse_reason != _network_join_refuse_reason_none)
        {
            return refuse_reason;
        }
    }

    if (!established())
    {
        return refuse_reason;
    }

    if (session_is_full(joining_peer_count, joining_player_count))
    {
        return _network_join_refuse_reason_session_full;
    }

    return refuse_reason;
}

void c_network_session::abort_pending_join(qword join_nonce)
{
    ASSERT(is_host());
    const c_network_session_membership* membership = &m_session_membership;

    if (m_session_membership.get_peer(m_session_membership.host_peer_index())->join_nonce == join_nonce)
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] the aborted join [%s] contains the host peer, disconnecting the session\n",
            get_id_string(),
            transport_secure_nonce_get_string(join_nonce));
        disconnect();
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::abort_pending_join: [%s] aborting join [%s]\n",
            get_id_string(),
            transport_secure_nonce_get_string(join_nonce));

        for (long peer_index = m_session_membership.get_first_peer(); peer_index != NONE; peer_index = m_session_membership.get_next_peer(peer_index))
        {
            s_network_session_peer* peer = m_session_membership.get_peer(peer_index);
            if (peer->join_nonce == join_nonce)
            {
                ASSERT(membership->get_peer_connection_state(peer_index) < _network_session_peer_state_established);
                ASSERT(peer_index != membership->host_peer_index());
                boot_peer(peer_index, _network_session_boot_for_join_abort);
            }
        }
    }
}

bool c_network_session::is_host() const
{
    e_network_session_state session_state = current_local_state();
    if (session_state == _network_session_state_host_established || session_state == _network_session_state_host_disband)
    {
        ASSERT(m_session_membership.local_peer_index() == m_session_membership.host_peer_index());
        return true;
    }
    else if (established())
    {
        ASSERT(m_session_membership.local_peer_index() != m_session_membership.host_peer_index());
    }

    return false;
}

bool c_network_session::join_allowed_by_privacy() const
{
    ASSERT(established() && is_host());

    if (!get_session_parameters()->m_parameters.privacy_mode.get_allowed())
    {
        return true;
    }

    const s_network_session_privacy_mode* privacy_mode = get_session_parameters()->m_parameters.privacy_mode.get();
    return privacy_mode->closed_mode == _network_session_closed_none && !privacy_mode->is_closed_by_user;

}

e_network_join_refuse_reason c_network_session::can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool player_add_not_join) const
{
    ASSERT(player_identifier);
    ASSERT(player_identifier_is_valid(player_identifier));

    long player_count = 0;
    e_network_join_refuse_reason refuse_reason = can_accept_any_join_request();
    if (refuse_reason != _network_join_refuse_reason_none)
    {
        return refuse_reason;
    }
    long player_index_from_identifier = m_session_membership.get_player_from_identifier(player_identifier);
    if (peer_index != NONE && m_session_membership.is_peer_valid(peer_index))
    {
        const s_network_session_peer* peer = m_session_membership.get_peer(peer_index);
        player_count = peer->player_mask.count_bits_set();
        if (!m_session_membership.is_player_in_player_add_queue(player_identifier) && peer->get_player_index() == NONE)
        {
            player_count++;
        }
        for (long queue_index = 0; queue_index < NUMBEROF(c_network_session_membership::m_player_add_queue); queue_index++)
        {
            const s_player_add_queue_entry* player_add_queue_entry = m_session_membership.get_player_add_queue_entry(queue_index);
            if (player_add_queue_entry != nullptr && player_add_queue_entry->peer_index == peer_index && peer->get_player_index() == NONE)
            {
                player_count++;
            }
        }
        refuse_reason = _network_join_refuse_reason_none;
    }

    if (game_is_dedicated_server() && user_xuid_from_secure_address(joining_peer_address) == USER_INVALID)
    {
        return _network_join_refuse_reason_address_invalid;
    }
    if (peer_index != NONE && !m_session_membership.is_peer_valid(peer_index))
    {
        return _network_join_refuse_reason_session_boot;
    }
    // removed since ms23
    //if (m_session_type == _network_session_type_group && player_add_not_join)
    //{
    //    return _network_join_refuse_reason_in_matchmaking;
    //}
    if (player_count > 4)
    {
        return _network_join_refuse_reason_no_split_screen_on_alpha;
    }
    if (player_index_from_identifier == NONE)
    {
        if ((peer_index == NONE
            || m_session_membership.is_peer_valid(peer_index)
            && m_session_membership.get_peer(peer_index)->get_player_index() == NONE)
            && session_is_full(0, 1))
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

bool c_network_session::session_is_full(long joining_peer_count, long joining_player_count) const
{
    if (established())
    {
        long max_peer_count = m_session_parameters.m_parameters.session_size.get_max_peer_count();
        long max_player_count = m_session_parameters.m_parameters.session_size.get_max_player_count();
        ASSERT(m_session_membership.has_membership());
        ASSERT(max_peer_count > 0 && max_peer_count <= k_network_maximum_machines_per_session);
        ASSERT(max_player_count > 0 && max_player_count <= k_network_maximum_players_per_session);
        return m_session_membership.get_peer_count() + joining_peer_count > max_peer_count || m_session_membership.get_player_count() + joining_player_count > max_player_count;
    }
    return false;
}

void c_network_session::disconnect()
{
    DECLFUNC(0x21CC0, void, __thiscall, c_network_session*)(this);

    // unregister the game server on the API
    if (game_is_dedicated_server())
    {
        s_transport_secure_address server_identifier;
        anvil_get_server_identifier(&server_identifier);
        c_backend::private_service::unregister_game_server::request(transport_secure_address_get_string(&server_identifier));
    }
}

void c_network_session::disband_peer(long peer_index)
{
    ASSERT(established() && is_host());
    ASSERT(m_session_membership.local_peer_index() != peer_index);
    if (!m_session_membership.has_peer_ever_been_established(peer_index))
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] marking joining peer [%s] for future disbandment\n", get_id_string(), get_peer_description(peer_index));
        m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_disconnected);
        return;
    }

    printf("MP/NET/SESSION,CTRL: c_network_session::disband_peer: [%s] disbanding peer [%s]\n", get_id_string(), get_peer_description(peer_index));
    s_network_message_session_disband disband_message;
    csmemset(&disband_message, 0, sizeof(disband_message));
    get_session_id(&disband_message.session_id);
    long observer_index = m_session_membership.get_observer_channel_index(peer_index);
    if (observer_index != NONE)
    {
        c_network_observer* observer = m_observer;
        if (observer->observer_channel_connected(observer_owner(), observer_index))
        {
            m_observer->observer_channel_send_message(observer_owner(), observer_index, false, _network_message_session_disband, sizeof(s_network_message_session_disband), &disband_message);
        }
        m_observer->observer_channel_send_message(observer_owner(), observer_index, true, _network_message_session_disband, sizeof(s_network_message_session_disband), &disband_message);
    }
    m_session_membership.remove_peer(peer_index);
}

void c_network_session::boot_peer(long peer_index, e_network_session_boot_reason boot_reason)
{
    ASSERT(established() && is_host());

    if (membership_is_locked())
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::boot_peer: [%s] unable to boot peer [#%d] membership is locked\n", get_id_string(), peer_index);
    }
    else
    {
        printf("MP/NET/SESSION,CTRL: c_network_session::boot_peer: [%s] booting peer [#%d] [reason %d]\n", get_id_string(), peer_index, boot_reason);
        s_network_message_session_boot boot_message = {};
        get_session_id(&boot_message.session_id);
        boot_message.reason = boot_reason;
        long observer_index = m_session_membership.get_observer_channel_index(peer_index);
        if (observer_index != NONE)
        {
            c_network_observer* observer = m_observer;
            if (observer->observer_channel_connected(observer_owner(), observer_index))
            {
                m_observer->observer_channel_send_message(observer_owner(), observer_index, false, _network_message_session_boot, sizeof(s_network_message_session_boot), &boot_message);
            }
            m_observer->observer_channel_send_message(observer_owner(), observer_index, true, _network_message_session_boot, sizeof(s_network_message_session_boot), &boot_message);
        }
        m_session_membership.remove_peer(peer_index);
    }
}

const char* c_network_session::get_type_string(e_network_session_type session_type)
{
    if (!VALID_INDEX(session_type, k_network_session_type_count))
    {
        return "<unknown>";
    }

    return k_session_type_strings[session_type];
}

char const* c_network_session::get_state_string() const
{
    e_network_session_state current_state = c_network_session::current_local_state();
    ASSERT(current_state >= 0 && current_state < k_network_session_state_count);
    return k_session_state_strings[current_state];
}

const char* c_network_session::get_peer_description(long peer_index) const
{
    static char peer_description[0x4B];
    if (established() && peer_index != NONE && m_session_membership.is_peer_valid(peer_index) && !m_session_membership.get_peer(peer_index)->properties.peer_name.is_empty())
    {
        csnzprintf(peer_description, 0x4B, "#%02d:%S:%s", peer_index,
            m_session_membership.get_peer(peer_index)->properties.peer_name.get_string(),
            transport_secure_address_get_mac_string(m_session_membership.get_peer_address(peer_index)));
    }
    else
    {
        csnzprintf(peer_description, 0x4B, "#%02d", peer_index);
    }
    return peer_description;
}

char const* c_network_session::get_id_string() const
{
    return managed_session_get_id_string(managed_session_index());
}

bool c_network_session::is_peer_joining_this_session() const
{
    return DECLFUNC(0x224F0, bool, __thiscall, c_network_session const*)(this);
}

void c_network_session::idle()
{
    if (!disconnected())
    {
        m_session_membership.idle();
        ASSERT(m_time_exists);
        if (established() && m_session_membership.all_peers_established() && m_disconnection_policy == _network_session_disconnection_waiting_for_establishment)
        {
            m_disconnection_policy = _network_session_disconnection_allowed;
        }
        switch (current_local_state())
        {
            case _network_session_state_peer_creating:
                idle_peer_creating();
                break;
            case _network_session_state_peer_joining:
                idle_peer_joining();
                break;
            case _network_session_state_peer_join_abort:
                idle_peer_join_abort();
                break;
            case _network_session_state_peer_leaving:
                idle_peer_leaving();
                break;
            case _network_session_state_host_disband:
                disconnect();
                break;
            default:
                break;
        }
        // ==== SECTION CUT FROM CLIENT BUILDS ====
        if (is_host() && !membership_is_locked())
        {
            process_pending_joins();
        }
        if (is_host())
        {
            if (!membership_is_locked())
            {
                for (long peer_index = m_session_membership.get_first_peer(); peer_index != NONE; peer_index = m_session_membership.get_next_peer(peer_index))
                {
                    long observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
                    if (observer_channel_index != NONE)
                    {
                        if (m_observer->observer_channel_dead(observer_owner(), observer_channel_index))
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::idle: [%s] peer [%s]/%s channel died, disbanding\n",
                                get_id_string(), get_peer_description(peer_index), m_observer->get_name(observer_channel_index));
                            disband_peer(peer_index);
                        }
                    }
                }

                if (established() && session_type() == _network_session_type_squad)
                {
                    long private_slot_count = 0;
                    long public_slot_count = 0;
                    bool friends_only = false;
                    long maximum_player_count = 0;
                    if (m_session_parameters.m_parameters.session_size.get_allowed())
                    {
                        maximum_player_count = get_maximum_player_count();
                        if (maximum_player_count < private_slot_count)
                        {
                            printf("MP/NET/SESSION,CTRL: c_network_session::idle: maximum session player count is smaller than the set of players already in the session, %d < %d\n",
                                maximum_player_count, private_slot_count);
                            maximum_player_count = private_slot_count;
                        }
                    }
                    if (m_session_parameters.m_parameters.privacy_mode.get_allowed())
                    {
                        const s_network_session_privacy_mode* privacy_mode_parameter = m_session_parameters.m_parameters.privacy_mode.get();
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
                    if (m_session_membership.private_slot_count() != private_slot_count ||
                        m_session_membership.public_slot_count() != public_slot_count ||
                        m_session_membership.friends_only() != friends_only)
                    {
                        m_session_membership.set_slot_counts(private_slot_count, public_slot_count, friends_only);
                    }
                }
            }
        }
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        else if (established())
        {
            long observer_channel_index = m_session_membership.get_host_observer_channel_index();
            ASSERT(observer_channel_index != NONE);
            ASSERT(!m_session_membership.is_host());
            if (m_observer->observer_channel_dead(observer_owner(), observer_channel_index))
            {
                handle_disconnection();
            }
            if (established())
            {
                check_to_send_time_synchronization();
            }
        }
        idle_observer_state();
        // ==== SECTION CUT FROM CLIENT BUILDS ====
        if (is_host())
        {
            check_to_send_membership_update();
        }
        // ==== END OF SECTION CUT FROM CLIENT BUILDS ====
        m_session_parameters.update();
        if (m_session_parameters.m_parameters.session_size.set_allowed())
        {
            const c_network_session_membership* membership = get_session_membership();
            long player_count = membership->get_player_count();
            if (player_count > m_session_parameters.m_parameters.session_size.get_max_player_count())
            {
                m_session_parameters.m_parameters.session_size.set_max_player_count(player_count);
            }
        }
    }
}

e_network_session_state c_network_session::current_local_state() const
{
    ASSERT(m_local_state >= 0 && m_local_state < k_network_session_state_count);
    return m_local_state;
}

bool c_network_session::disconnected() const
{
    return current_local_state() == _network_session_state_none;
}

bool c_network_session::established() const
{
    switch (current_local_state())
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
            ASSERT("unreachable");
            return false;
    }
}

bool c_network_session::membership_is_locked() const
{
    ASSERT(is_host());
    bool locked = current_local_state() == _network_session_state_host_disband; /*|| current_local_state() == _network_session_state_host_handoff || current_local_state() == _network_session_state_host_reestablish*/
    ASSERT(established() || locked);
    return locked;
}

c_network_session_membership const* c_network_session::get_session_membership() const
{
    ASSERT(established());
    ASSERT(m_session_membership.has_membership());
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
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
    c_network_session_membership* membership = &m_session_membership;

    e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
    c_managed_session_status_flags session_flags = managed_session_get_status(m_managed_session_index);
    ASSERT(is_host());
    if (session_flags.test(_managed_session_players_add_succeeded_bit) || session_flags.test(_managed_session_players_add_failed_bit))
    {
        managed_session_reset_players_add_status(m_managed_session_index);
        if (player_identifier_is_valid(&m_player_we_are_adding.player_identifier))
        {
            refuse_reason = _network_join_refuse_reason_player_add_failed;
            if (session_flags.test(_managed_session_players_add_succeeded_bit))
            {
                refuse_reason = can_accept_player_join_request(&m_player_we_are_adding.player_identifier, &m_player_we_are_adding.secure_address, m_player_we_are_adding.peer_output_user_index, true);
                if (refuse_reason == _network_join_refuse_reason_none && !membership->is_player_in_player_add_queue(&m_player_we_are_adding.player_identifier))
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] player '%s' went missing from our add queue\n",
                        get_id_string(),
                        player_identifier_get_string(&m_player_we_are_adding.player_identifier));
                    refuse_reason = _network_join_refuse_reason_player_add_failed;
                }
                if (refuse_reason != _network_join_refuse_reason_none)
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] single player '%s' added but can no longer join because %s, removing\n",
                        get_id_string(),
                        player_identifier_get_string(&m_player_we_are_adding.player_identifier),
                        network_message_join_refuse_get_reason_string(refuse_reason));
                    managed_session_remove_players(m_managed_session_index, (qword*)&m_player_we_are_adding.player_identifier, 1);
                }
                else
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] single player '%s' added\n",
                        get_id_string(),
                        player_identifier_get_string(&m_player_we_are_adding.player_identifier));
                    membership->commit_player_from_player_add_queue(&m_player_we_are_adding.player_identifier);
                }
            }
            else
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] XSessionJoinRemote failed for single player add\n", get_id_string());
            }
            finalize_single_player_add(refuse_reason);
        }
        else
        {
            long peer_index = NONE;
            for (peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
            {
                if (membership->get_peer_connection_state(peer_index) == _network_session_peer_state_joining)
                {
                    break;
                }
            }
            if (peer_index != NONE)
            {
                qword join_nonce = membership->get_join_nonce(peer_index);
                if (session_flags.test(_managed_session_players_add_succeeded_bit))
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] marking all peers in join [%s] as joined\n",
                        get_id_string(),
                        transport_secure_nonce_get_string(join_nonce));
                    for (peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
                    {
                        if (membership->get_peer_connection_state(peer_index) == _network_session_peer_state_joining && join_nonce == membership->get_join_nonce(peer_index))
                        {
                            if (peer_index == membership->host_peer_index())
                            {
                                membership->set_peer_connection_state(peer_index, _network_session_peer_state_waiting);
                            }
                            else
                            {
                                membership->set_peer_connection_state(peer_index, _network_session_peer_state_joined);
                            }
                        }
                    }
                }
                else
                {
                    if (!join_nonce_is_from_clone_join_or_is_hosts(join_nonce) || managed_session_is_master_session(m_managed_session_index))
                    {
                        if (join_nonce_is_from_clone_join_or_is_hosts(join_nonce))
                        {
                            ASSERT(session_flags.test(_managed_session_players_add_failed_bit));
                            printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] players in local host join [%s] could not be added, recreating the session\n",
                                get_id_string(),
                                transport_secure_nonce_get_string(join_nonce));
                            for (long kick_peer_index = membership->get_first_peer(); kick_peer_index != NONE; kick_peer_index = membership->get_next_peer(kick_peer_index))
                            {
                                if (kick_peer_index == membership->host_peer_index())
                                {
                                    ASSERT(membership->get_join_nonce(kick_peer_index) == m_host_join_nonce);
                                    membership->set_peer_connection_state(kick_peer_index, _network_session_peer_state_connected);
                                }
                                else
                                {
                                    ASSERT(membership->get_join_nonce(kick_peer_index) != m_host_join_nonce);
                                    boot_peer(kick_peer_index, _network_session_boot_for_join);
                                }
                            }
                            managed_session_reset_session(m_managed_session_index, true);
                        }
                        else
                        {
                            ASSERT(session_flags.test(_managed_session_players_add_failed_bit));
                            printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] XSessionJoinRemote failed, aborting join\n",
                                get_id_string());
                            abort_pending_join(join_nonce);
                        }
                    }
                    else
                    {
                        ASSERT(session_flags.test(_managed_session_players_add_failed_bit));
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] players in squad host join [%s] could not be added, disconnecting\n",
                            get_id_string(),
                            transport_secure_nonce_get_string(join_nonce));
                        disconnect();
                    }
                }
            }
            else
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] we completed a player modify that we didn't request\n",
                    get_id_string());
            }
        }
    }
    if (!disconnected())
    {
        dword waiting_peers_mask = 0; // k_network_maximum_machines_per_session
        for (long i = membership->get_first_peer(); i != NONE; i = membership->get_next_peer(i))
        {
            e_network_session_peer_state connection_state = membership->get_peer_connection_state(i);
            // Check if peer is waiting or connected and isn't currently add in progress
            if (connection_state == _network_session_peer_state_waiting ||
                connection_state == _network_session_peer_state_connected &&
                !session_flags.test(_managed_session_players_add_in_progress_bit))
            {
                SET_BIT(waiting_peers_mask, i, true);
            }
        }
        for (long peer_index = 0; waiting_peers_mask != 0; peer_index++)
        {
            long peer_mask = FLAG(peer_index);
            s_network_session_peer* peer = membership->get_peer(peer_index);
            // If the current peer iteration is in the waiting mask
            if (TEST_MASK(peer_mask, waiting_peers_mask))
            {
                bool peer_states_match = true;
                e_network_session_peer_state state = membership->get_peer_connection_state(peer_index);

                // Remove the current peer iteration from the waiting peers mask
                SET_MASK(waiting_peers_mask, peer_mask, false);

                qword join_nonce = peer->join_nonce;
                ASSERT(membership->is_peer_valid(peer_index));
                ASSERT(state == _network_session_peer_state_waiting || state == _network_session_peer_state_connected);
                ASSERT(join_nonce != 0);
                ASSERT(state != _network_session_peer_state_connected || !session_flags.test(_managed_session_players_add_in_progress_bit));
                ASSERT(state != _network_session_peer_state_connected || !session_flags.test(_managed_session_players_add_in_progress_bit));

                // Search for all member peers for one with a matching join nonce
                for (long membership_peer_index = membership->get_first_peer(); membership_peer_index != NONE; membership_peer_index = membership->get_next_peer(membership_peer_index))
                {
                    if (membership->get_peer(membership_peer_index)->join_nonce == peer->join_nonce)
                    {
                        // Remove peer from waiting mask if already a member
                        SET_BIT(waiting_peers_mask, membership_peer_index, false);
                        if (membership->get_current_membership()->peers[membership_peer_index].connection_state != peer->connection_state)
                        {
                            peer_states_match = false;
                        }
                    }
                }
                if (peer_states_match)
                {
                    if (peer->connection_state == _network_session_peer_state_connected)
                    {
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] adding the join [%s] to the XSession\n",
                            get_id_string(),
                            transport_secure_nonce_get_string(peer->join_nonce));
                        add_pending_join_to_session(peer->join_nonce);
                        session_flags.set(_managed_session_players_add_in_progress_bit, true);
                    }
                    else
                    {
                        ASSERT(state == _network_session_peer_state_waiting);
                        printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: [%s] marking all peers in join [%s] as established\n",
                            get_id_string(),
                            transport_secure_nonce_get_string(peer->join_nonce));
                        for (long peer_index_in_join = peer_index; peer_index_in_join != NONE; peer_index_in_join = membership->get_next_peer(peer_index_in_join))
                        {
                            if (membership->get_peer(peer_index_in_join)->join_nonce == peer->join_nonce)
                            {
                                membership->set_peer_connection_state(peer_index_in_join, _network_session_peer_state_established);
                            }
                        }
                        // m_matchmaking_establish_time would be set here in h3/ms23, which of course we don't have
                    }
                }
            }
        }
    }

    if (!disconnected() && !session_flags.test(_managed_session_players_add_in_progress_bit))
    {
        s_player_add_queue_entry* add_queue_entry = membership->get_first_player_from_player_add_queue();
        bool xuids_left_game[] = { true };
        if (add_queue_entry != nullptr)
        {
            s_network_session_peer* peer = membership->get_peer(add_queue_entry->peer_index);
            m_player_we_are_adding.player_identifier = add_queue_entry->player_identifier;
            m_player_we_are_adding.secure_address = peer->secure_address;
            m_player_we_are_adding.peer_output_user_index = add_queue_entry->peer_index;
            e_network_join_refuse_reason refuse_reason = can_accept_player_join_request
            (
                &m_player_we_are_adding.player_identifier,
                &m_player_we_are_adding.secure_address,
                add_queue_entry->peer_index,
                true
            );
            if (refuse_reason != _network_join_refuse_reason_none)
            {
                finalize_single_player_add(refuse_reason);
            }
            else
            {
                qword xuids[] = { user_xuid_from_secure_address(&peer->secure_address) };
                managed_session_add_players(managed_session_index(), xuids, xuids_left_game, NUMBEROF(xuids));
            }
        }
    }

    if (!disconnected() && !membership_is_locked())
    {
        ulong pending_join_timeout = get_network_configuration()->life_cycle.joining.join_timeout_msec;
        if (session_type() == _network_session_type_squad)
        {
            pending_join_timeout = get_network_configuration()->life_cycle.joining.join_remote_squad_player_reservation_timeout_msec;
        }

        for (long peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
        {
            qword join_nonce = membership->get_join_nonce(peer_index);
            long peer_creation_timestamp = membership->get_creation_timestamp(peer_index);

            if (!join_nonce_is_from_clone_join_or_is_hosts(join_nonce) && membership->get_peer_connection_state(peer_index) == _network_session_peer_state_disconnected)
            {
                printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] pending joins for [%s] failed because the peer disconnect, removing\n",
                    get_id_string(),
                    transport_secure_nonce_get_string(join_nonce));

                abort_pending_join(join_nonce);
                continue;
            }

            if (!join_nonce_is_from_clone_join_or_is_hosts(join_nonce) &&
                membership->get_peer_connection_state(peer_index) < _network_session_peer_state_established &&
                pending_join_timeout != NONE)
            {
                ulong time_since_creation = network_time_since(peer_creation_timestamp);
                if (time_since_creation >= pending_join_timeout)
                {
                    printf("MP/NET/SESSION,CTRL: c_network_session::process_pending_joins: Warning. [%s] pending joins for [%s] timed out (%d msec > %d msec), removing\n",
                        get_id_string(),
                        transport_secure_nonce_get_string(join_nonce),
                        network_time_since(peer_creation_timestamp),
                        pending_join_timeout);

                    abort_pending_join(join_nonce);
                    continue;
                }
            }
        }
    }
}

e_network_observer_owner c_network_session::observer_owner() const
{
    ASSERT(m_session_index >= 0 && m_session_index < k_network_maximum_sessions);
    return static_cast<e_network_observer_owner>(m_session_index);
}

long c_network_session::get_maximum_player_count() const
{
    return DECLFUNC(0x22E80, long, __thiscall, c_network_session const*)(this);
}

void c_network_session::handle_disconnection()
{
    printf("MP/NET/SESSION,CTRL: c_network_session::handle_disconnection: [%s] disconnected from session host. Session Type %s\n", get_id_string(), get_type_string(m_session_type));
    hf2p_handle_disconnection(); // unknown saber function, new since ms23
    disconnect();
}

void c_network_session::check_to_send_time_synchronization()
{
    ASSERT(established() && !is_host());
    long observer_channel_index = m_session_membership.get_host_observer_channel_index();
    ASSERT(observer_channel_index != NONE);
    DECLFUNC(0x22B50, void, __thiscall, c_network_session*)(this);
}

void c_network_session::idle_observer_state()
{
    DECLFUNC(0x3EAC0, void, __thiscall, c_network_session*)(this);
}

void c_network_session::check_to_send_membership_update()
{
    ASSERT(established() && is_host());

    for (long peer_index = m_session_membership.get_first_peer(); peer_index != NONE; peer_index = m_session_membership.get_next_peer(peer_index))
    {
        long observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
        long membership_update_number = m_session_membership.get_membership_update_number(peer_index);
        ASSERT(!m_session_membership.get_peer_needs_reestablishment(peer_index) || (membership_update_number == NONE));

        if (observer_channel_index != NONE &&
            m_session_membership.get_peer_connection_state(peer_index) >= _network_session_peer_state_connected &&
            !m_session_membership.get_peer_needs_reestablishment(peer_index))
        {
            c_network_observer* observer = m_observer;
            c_network_observer::s_channel_observer* channel_observer = &observer->m_channel_observers[observer_channel_index];
            if (observer->observer_channel_connected(observer_owner(), observer_channel_index))
            {
                if (membership_update_number != m_session_membership.update_number())
                {
                    if (observer->observer_channel_backlogged(observer_owner(), observer_channel_index, _network_message_membership_update))
                    {
                        observer->observer_channel_set_waiting_on_backlog(observer_owner(), observer_channel_index, _network_message_membership_update);
                    }
                    else
                    {
                        // create new shared membership containing only the host and target peer
                        // In Halo Online, P2P hosting isn't a thing, so clients don't need to know the information of every other connected peer
                        // This also saves us leaking client IP addresses
                        s_network_session_shared_membership shared_membership = *m_session_membership.get_current_membership();
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
                            {
                                csmemset(&shared_membership.peers[peer_index], 0, sizeof(s_network_session_peer));
                            }
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
                        const s_network_session_shared_membership* transmitted_membership = m_session_membership.get_transmitted_membership(peer_index);
                        if (membership_update_number == NONE || membership_update_number != transmitted_membership->update_number)
                        {
                            send_complete_update = true;
                            transmitted_membership = nullptr;
                        }
                        s_network_message_membership_update membership_update_message;
                        m_session_membership.build_membership_update(peer_index, &shared_membership, transmitted_membership, &membership_update_message);
                        if (send_complete_update)
                        {
                            ASSERT(membership_update_number == NONE || membership_update_number != m_session_membership.get_transmitted_membership(peer_index)->update_number);
                            printf("MP/NET/SESSION,CTRL: c_network_session::check_to_send_membership_update: [%s] sending complete update #-1->[#%d] to peer [#%d]\n",
                                get_id_string(),
                                m_session_membership.update_number(),
                                peer_index);
                        }
                        else
                        {
                            ASSERT(membership_update_number == m_session_membership.get_transmitted_membership(peer_index)->update_number);
                            printf("MP/NET/SESSION,CTRL: c_network_session::check_to_send_membership_update: [%s] sending incremental update [#%d]->[#%d] to peer [#%d]\n",
                                get_id_string(),
                                membership_update_number,
                                m_session_membership.update_number(),
                                peer_index);
                        }
                        ASSERT(observer_channel_index != NONE);
                        if (m_observer->observer_channel_connected(observer_owner(), observer_channel_index))
                        {
                            if (observer->observer_channel_backlogged(observer_owner(), observer_channel_index, _network_message_membership_update))
                            {
                                observer->observer_channel_set_waiting_on_backlog(observer_owner(), observer_channel_index, _network_message_membership_update);
                            }
                            else
                            {
                                ASSERT(m_observer->observer_channel_connected(observer_owner(), observer_channel_index));
                                m_session_membership.set_membership_update_number(peer_index, m_session_membership.update_number());
                                long observer_index = m_session_membership.get_observer_channel_index(peer_index);
                                if (observer_index != NONE)
                                {
                                    // send_message_to_peer
                                    observer->observer_channel_send_message(observer_owner(), observer_index, false, _network_message_membership_update, sizeof(s_network_message_membership_update), &membership_update_message);
                                }
                            }
                        }
                        m_session_membership.copy_current_to_transmitted(peer_index, &shared_membership);
                    }
                }
            }
        }
    }
}

long c_network_session::managed_session_index() const
{
    ASSERT(m_managed_session_index != NONE);
    return m_managed_session_index;
}

bool c_network_session::join_abort(transport_address const* incoming_address, qword join_nonce)
{
    ASSERT(incoming_address);
    ASSERT(established());
    ASSERT(is_host());

    network_join_remove_join_from_queue(join_nonce);
    if (membership_is_locked())
    {
        return false;
    }
    abort_pending_join(join_nonce);
    return true;
}

c_network_session_parameters* c_network_session::get_session_parameters()
{
    return &m_session_parameters;
}

const c_network_session_parameters* c_network_session::get_session_parameters() const
{
    return &m_session_parameters;
}

bool c_network_session::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
    if (established() && is_host())
    {
        long channel_index = m_observer->observer_channel_find_by_network_channel(observer_owner(), channel);
        long peer_index = m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_properties: [%s] peer-properties received from invalid peer [#%d]\n", get_id_string(), peer_index);
            return false;
        }
        else
        {
            if (m_session_membership.get_peer_connection_state(peer_index) == _network_session_peer_state_joined)
            {
                m_session_membership.set_peer_connection_state(peer_index, _network_session_peer_state_waiting);
            }
            m_session_membership.set_peer_address(peer_index, &message->secure_address);
            m_session_membership.set_peer_properties(peer_index, &message->peer_properties);
            return true;
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_properties: [%s] peer-properties received but not host, can't update properties (state %s)\n",
            get_id_string(),
            get_state_string());
        return false;
    }

    return false;
}

bool c_network_session::peer_request_properties_update(s_transport_secure_address const* secure_address, s_network_session_peer_properties const* peer_properties)
{
    ASSERT(peer_properties->peer_map_status >= 0 && peer_properties->peer_map_status < k_network_session_map_status_count);

    if (established())
    {
        if (is_host())
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::peer_request_properties_update: [%s] applying peer-properties locally\n", get_id_string());
            m_session_membership.set_peer_address(m_session_membership.local_peer_index(), secure_address);
            m_session_membership.set_peer_properties(m_session_membership.local_peer_index(), peer_properties);
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::peer_request_properties_update: [%s] sending peer-properties request\n", get_id_string());
            s_network_message_peer_properties message = {};
            get_session_id(&message.session_id);
            message.secure_address = *secure_address;
            memcpy(&message.peer_properties, peer_properties, sizeof(s_network_session_peer_properties));
            long channel_index = m_session_membership.m_local_peer_state[m_session_membership.host_peer_index()].channel_index;
            if (channel_index != NONE)
            {
                m_observer->observer_channel_send_message(observer_owner(), channel_index, false, _network_message_peer_properties, sizeof(s_network_message_peer_properties), &message);
            }
        }
        return true;
    }
    return false;
}

c_network_session_membership const* c_network_session::get_session_membership_unsafe() const
{
    if (!disconnected() && m_session_membership.has_membership())
    {
        ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
        ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
        return &m_session_membership;
    }
    return NULL;
}

bool c_network_session::join_nonce_is_from_clone_join_or_is_hosts(qword join_nonce) const
{
    return join_nonce == m_host_join_nonce;
}

void c_network_session::finalize_single_player_add(e_network_join_refuse_reason refuse_reason)
{
    if (refuse_reason != _network_join_refuse_reason_none)
    {
        m_session_membership.remove_player_from_player_add_queue(&m_player_we_are_adding.player_identifier);
    }

    ASSERT(m_player_we_are_adding.peer_output_user_index != NONE);

    if (m_player_we_are_adding.peer_output_user_index == m_session_membership.host_peer_index())
    {
        if (refuse_reason != _network_join_refuse_reason_none)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: Warning [%s] host player '%s' add failed because of '%s'\n",
                get_id_string(),
                player_identifier_get_string(&m_player_we_are_adding.player_identifier),
                network_message_join_refuse_get_reason_string(refuse_reason));
            m_local_user_player_add.refuse_reason = refuse_reason;
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: [%s] host player '%s' add succeeded\n",
                get_id_string(),
                player_identifier_get_string(&m_player_we_are_adding.player_identifier));
            m_local_user_player_add.refuse_reason = _network_join_refuse_reason_none;
        }
    }
    else
    {
        s_network_message_player_refuse message = {};
        get_session_id(&message.session_id);
        long channel_index = m_session_membership.get_observer_channel_index(m_player_we_are_adding.peer_output_user_index);
        if (channel_index != NONE)
        {
            m_observer->observer_channel_send_message(observer_owner(), channel_index, false, _network_message_player_refuse, sizeof(s_network_message_player_refuse), &message);
        }
        if (refuse_reason != _network_join_refuse_reason_none)
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: Warning. [%s] informing peer %d that its player '%s' add failed because '%s'\n",
                get_id_string(),
                m_player_we_are_adding.peer_output_user_index,
                player_identifier_get_string(&m_player_we_are_adding.player_identifier),
                network_message_join_refuse_get_reason_string(refuse_reason));
        }
        else
        {
            printf("MP/NET/SESSION,CTRL: c_network_session::finalize_single_player_add: [%s] informing peer %d that its player '%s' add succeeded\n",
                get_id_string(),
                m_player_we_are_adding.peer_output_user_index,
                player_identifier_get_string(&m_player_we_are_adding.player_identifier));
        }
    }
    csmemset(&m_player_we_are_adding.player_identifier, 0, sizeof(s_player_identifier));
    csmemset(&m_player_we_are_adding.secure_address, 0, sizeof(s_transport_secure_address));
    m_player_we_are_adding.peer_output_user_index = 0;
}

e_network_session_type c_network_session::session_type() const
{
    ASSERT(m_session_type >= 0 && m_session_type < k_network_session_type_count);
    return m_session_type;
}

void c_network_session::add_pending_join_to_session(qword join_nonce)
{
    c_network_session_membership* membership = &m_session_membership;

    bool players_left[k_network_maximum_players_per_session] = {};
    qword player_xuids[k_network_maximum_players_per_session] = {};
    long xuid_count = 0;
    for (long peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
    {
        if (membership->get_join_nonce(peer_index) == join_nonce)
        {
            ASSERT(membership->get_peer_connection_state(peer_index) == _network_session_peer_state_connected);
            long player_index = membership->get_peer(peer_index)->get_player_index();
            if (player_index != NONE)
            {
                player_xuids[xuid_count] = membership->get_player(player_index)->configuration.host.user_xuid;
                players_left[xuid_count] = !membership->get_player(player_index)->left_game;
                xuid_count++;
            }
            membership->set_peer_connection_state(peer_index, _network_session_peer_state_joining);
        }
    }
    managed_session_add_players(managed_session_index(), player_xuids, players_left, xuid_count);
    ASSERT(managed_session_get_status(m_managed_session_index).test(_managed_session_players_add_in_progress_bit));
}

long c_network_session::session_index() const
{
    ASSERT(m_session_index >= 0 && m_session_index < k_network_maximum_sessions);
    return m_session_index;
}

bool c_network_session::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
    ASSERT(message);
    ASSERT(!disconnected());
    ASSERT(managed_session_compare_id(m_managed_session_index, &message->session_id));

    long channel_index = m_observer->observer_channel_find_by_network_channel(observer_owner(), channel);
    long peer_index = m_session_membership.get_peer_from_observer_channel(channel_index);
    if (is_host())
    {
        if (peer_index == NONE && peer_index == m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received as host from invalid peer [#%d] (range [0,%d-1] us [#%d])\n",
                get_id_string(),
                peer_index,
                m_session_membership.get_peer_count(),
                m_session_membership.local_peer_index());
        }
        else
        {
            ASSERT(peer_index != m_session_membership.host_peer_index());
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received from peer [%s]\n",
                get_id_string(),
                get_peer_description(peer_index));
            m_session_membership.set_peer_needs_reestablishment(peer_index, false);
            return true;
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_peer_establish: [%s] peer-establish received but not host (%s)\n",
            get_id_string(),
            get_state_string());
    }

    if ((is_host()) || (channel_index == NONE))
    {
        return false;
    }

    s_network_message_host_decline decline_message = {};
    get_session_id(&decline_message.session_id);
    decline_message.peer_exists = peer_index != NONE;
    decline_message.session_exists = true;
    if (established())
    {
        ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
        ASSERT(m_session_membership.host_peer_index() != m_session_membership.local_peer_index());
        decline_message.host_exists = true;
        decline_message.host_address = *m_session_membership.get_peer_address(m_session_membership.host_peer_index());
    }
    m_observer->observer_channel_send_message(observer_owner(), channel_index, false, _network_message_host_decline, sizeof(s_network_message_host_decline), &decline_message);
    return true;
}

bool c_network_session::leaving_session() const
{
    switch (current_local_state())
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
    m_time_exists = true;
    m_time_offset = time;
}

bool c_network_session::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
    if (established() && is_host())
    {
        long channel_index = m_observer->observer_channel_find_by_network_channel(observer_owner(), channel);
        long peer_index = m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received from invalid peer [#%d]\n", get_id_string(), peer_index);
            return false;
        }
        else
        {
            long player_index = m_session_membership.get_current_membership()->peers[peer_index].get_player_index();
            if (player_index == NONE)
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received but no player associated with peer [#%d]\n",
                    get_id_string(),
                    peer_index);
                return false;
            }
            else
            {
                m_session_membership.set_player_properties(player_index, message->player_update_number, message->controller_index, &message->player_from_client, message->player_voice);
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties accepted for peer/player [#%d]/[#%d]\n",
                    get_id_string(),
                    peer_index,
                    player_index);
                return true;
            }
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_player_properties: [%s] player-properties received but not host, can't update players\n", get_id_string());
        return false;
    }

    return false;
}

bool c_network_session::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
    ASSERT(message);

    if (established() && is_host())
    {
        long channel_index = m_observer->observer_channel_find_by_network_channel(observer_owner(), channel);
        long peer_index = m_session_membership.get_peer_from_observer_channel(channel_index);
        if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
        {
            printf("MP/NET/SESSION,PARAMS: c_network_session::handle_parameters_request: [%s] parameters-request received from invalid peer %d\n", get_id_string(), peer_index);
        }
        else
        {
            if (get_session_parameters()->handle_change_request((peer_index == m_session_membership.leader_peer_index()), message))
            {
                return true;
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::handle_parameters_request: [%s] failed to handle parameters-request received from peer %d\n",
                    get_id_string(),
                    peer_index);
            }
        }
    }
    else
    {
        printf("MP/NET/SESSION,PARAMS: c_network_session::handle_parameters_request: [%s] parameters-request received and we are not in a state to handle [%s]\n",
            get_id_string(),
            get_state_string());
    }
    return false;
}

bool c_network_session::compare_session_id(s_transport_secure_identifier const* secure_id) const
{
    return managed_session_compare_id(managed_session_index(), secure_id);
}

bool c_network_session::get_session_id(s_transport_secure_identifier* secure_id) const
{
    return managed_session_get_id(managed_session_index(), secure_id);
}

void c_network_session::peer_request_player_add(const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* player_data_from_client, long voice_settings)
{
    ASSERT(user_index >= 0 && user_index < k_number_of_users);
    ASSERT(player_identifier);
    ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
    ASSERT(player_data_from_client);
    //ASSERT(m_session_membership.get_player_from_peer(m_session_membership.local_peer_index(), user_index) == NULL);

    DECLFUNC(0x21EF0, void, __thiscall, c_network_session*, const s_player_identifier*, long user_index, long controller_index, s_player_configuration_from_client*, long)(this, player_identifier, user_index, controller_index, player_data_from_client, voice_settings);
}

long c_network_session::get_session_membership_update_number() const
{
    ASSERT(established());
    ASSERT(m_session_membership.has_membership());
    return m_session_membership.m_baseline.update_number;
}

c_network_session_membership* c_network_session::get_session_membership_for_update()
{
    ASSERT(established());
    ASSERT(is_host());
    ASSERT(m_session_membership.has_membership());
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
    return &m_session_membership;
}

long c_network_session::find_peer_by_machine_identifier(s_machine_identifier const* machine_identifier) const
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

bool c_network_session::handle_leave_request(transport_address const* outgoing_address)
{
    ASSERT(is_host());
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
    ASSERT(is_host());
    ASSERT(established());
    ASSERT(peer_index != NONE);
    ASSERT(peer_index != m_session_membership.local_peer_index());
    long observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
    if (m_session_class == _network_session_class_xbox_live)
    {
        m_observer->quality_statistics_notify_peer_left_gracefully(observer_owner(), observer_channel_index);
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
        ASSERT(observer_channel_index != NONE);
        s_network_message_leave_acknowledge message;
        csmemset(&message, 0, sizeof(message));
        managed_session_get_id(m_managed_session_index, &message.session_id);
        m_observer->observer_channel_send_message(observer_owner(), observer_channel_index, true, _network_message_leave_acknowledge, sizeof(message), &message);
        return true;
    }
}

bool c_network_session::peer_joining() const
{
    return current_local_state() == _network_session_state_peer_joining;
}