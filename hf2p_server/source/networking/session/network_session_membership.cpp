#include "network_session_membership.h"
#include <networking\network_globals.h>
#include <iostream>
#include <networking\network_utilities.h>
#include <networking\session\network_session.h>
#include <networking\session\network_managed_session.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\messages\network_messages_session_membership.h>
#include <math\fast_checksum.h>
#include <text\unicode.h>
#include <fstream>
#include <networking\network_time.h>
#include <game\player_mapping.h>
//#include <format>

char const* network_session_peer_states[k_network_session_peer_state_count] =
{
    "_none",
    "_rejoining",
    "_reserved",
    "_disconnected",
    "_connected",
    "_joining",
    "_joined",
    "_waiting",
    "_established",
};

long c_network_session_membership::get_first_peer() const
{
    for (long peer_index = 0; peer_index < k_network_maximum_machines_per_session; peer_index++)
    {
        if (is_peer_valid(peer_index))
        {
            return peer_index;
        }
    }
    return NONE;
}

long c_network_session_membership::get_next_peer(long peer_index) const
{
    ASSERT(peer_index != NONE);
    for (long next_peer_index = peer_index + 1; next_peer_index < k_network_maximum_machines_per_session; next_peer_index++)
    {
        if (is_peer_valid(next_peer_index))
        {
            return next_peer_index;
        }
    }
    return NONE;
}

long c_network_session_membership::get_first_player() const
{
    for (long player_index = 0; player_index < k_network_maximum_players_per_session; player_index++)
    {
        if (is_player_valid(player_index))
        {
            return player_index;
        }
    }
    return NONE;
}

long c_network_session_membership::get_next_player(long player_index) const
{
    ASSERT(player_index != NONE);
    for (long next_player_index = player_index + 1; next_player_index < k_network_maximum_players_per_session; next_player_index++)
    {
        if (is_player_valid(next_player_index))
        {
            return next_player_index;
        }
    }
    return NONE;
}

long c_network_session_membership::get_peer_from_secure_address(s_transport_secure_address const* secure_address) const
{
    ASSERT(get_session());
    ASSERT(secure_address);
    return DECLFUNC(0x312B0, long, __thiscall, const c_network_session_membership*, s_transport_secure_address const*)(this, secure_address);
}

bool c_network_session_membership::is_peer_valid(long peer_index) const
{
    return m_baseline.peer_valid_mask.test(peer_index);
}

bool c_network_session_membership::is_player_valid(long player_index) const
{
    return m_baseline.player_valid_mask.test(player_index);
}

bool c_network_session_membership::add_peer(long peer_index, e_network_session_peer_state peer_state, ulong joining_network_version_number, s_transport_secure_address const* secure_address, qword join_party_nonce, qword join_nonce)
{
    ASSERT(get_session());
    return DECLFUNC(0x30DE0, bool, __thiscall, c_network_session_membership*, long, e_network_session_peer_state, ulong, s_transport_secure_address const*, qword, qword)(this, peer_index, peer_state, joining_network_version_number, secure_address, join_party_nonce, join_nonce);
}

// Rewritten as the existing find_or_add_player method has peer index 0 baked into it and will ignore the actual peer index arg
long c_network_session_membership::find_or_add_player(long peer_index, s_player_identifier const* player_identifier, bool player_left)
{
    ASSERT(player_identifier);
    ASSERT(get_session());

    c_network_session* session = get_session();
    s_network_session_peer* peer = get_peer(peer_index);
    // $TODO: Test this assert works as intended, secure address wasn't part of this func call in H3
    ASSERT(get_session()->can_accept_player_join_request(player_identifier, &peer->secure_address, peer_index, false) == _network_join_refuse_reason_none);

    long player_index = peer->get_player_index();
    if (player_index != NONE)
    {
        if (get_player(player_index)->player_identifier == *player_identifier)
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: peer #%d [%s] already has correct player [#%d]\n",
                session->get_id_string(),
                peer_index,
                transport_secure_address_get_string(&peer->secure_address),
                player_index);
            return player_index;
        }
        else
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: peer #%d [%s] already has player [#%d] with different identifier, removing\n",
                session->get_id_string(),
                peer_index,
                transport_secure_address_get_string(&peer->secure_address),
                player_index);
            remove_player(player_index);
            player_index = NONE;
        }
    }
    long player_index_from_identifier = get_player_from_identifier(player_identifier);
    if (player_index_from_identifier != NONE)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: player index %d has the same player identifier as the one we're adding, removing them\n",
            session->get_id_string(),
            player_index_from_identifier);
        remove_player(player_index_from_identifier);
    }

    if (session->established())
    {
        ASSERT(get_player_count() < session->get_session_parameters()->m_parameters.session_size.get_max_player_count());
    }

    long next_player_index;
    for (next_player_index = 0; next_player_index < k_network_maximum_players_per_session; next_player_index++)
    {
        if (!is_player_valid(next_player_index))
        {
            break;
        }
    }
    player_index = next_player_index;
    ASSERT(player_index != NONE);
    add_player_internal(player_index, player_identifier, peer_index, m_baseline.player_sequence_number, player_left);
    m_baseline.player_sequence_number = (m_baseline.player_sequence_number + 1) % 0x100000;
    s_network_session_player* player = get_player(player_index);
    player->configuration.client.user_selected_team_index = _game_team_none;
    player->configuration.client.selected_loadout_index = NONE;
    player->configuration.host.team_index = _game_team_none;
    player->configuration.host.assigned_team_index = _game_team_none;
    increment_update();
    ASSERT(player_index != NONE);
    return next_player_index;
}

s_network_session_player* c_network_session_membership::add_player_internal(long player_index, s_player_identifier const* player_identifier, long peer_index, long player_sequence_number, bool player_left_game)
{
    ASSERT(!is_player_valid(player_index));
    ASSERT(player_identifier);
    ASSERT(is_peer_valid(peer_index));
    //ASSERT(peer_user_index >= 0 && peer_user_index < k_number_of_users);
    ASSERT(get_player_from_identifier(player_identifier) == NONE);
    const s_network_session_shared_membership* membership = get_current_membership();
    s_network_session_player* player = get_raw_player(player_index);
    s_network_session_player test_player;
    csmemset(&test_player, 0, sizeof(test_player));
    ASSERT(!memcmp(player, &test_player, sizeof(test_player)));

    m_baseline.player_valid_mask.set(player_index, true);
    m_baseline.player_count++;
    player->player_identifier = *player_identifier;
    player->peer_index = peer_index;
    player->player_sequence_number = m_baseline.player_sequence_number;
    player->left_game = player_left_game;
    player->controller_index = k_no_controller;
    if (peer_index != NONE)
    {
        s_network_session_peer* player_peer = get_peer(peer_index);
        //ASSERT(player_peer->player_count < k_network_maximum_players_per_session);
        player_peer->player_mask.set(player_index, true);
        //ASSERT(player_peer->user_player_indices[peer_user_index] == NONE);
    }
    player->configuration.host.team_index = _game_team_none;
    player->configuration.host.assigned_team_index = _game_team_none;
    return player;
}

void c_network_session_membership::update_player_data(long player_index, s_player_configuration const* player_config)
{
    s_network_session_player* player = get_player(player_index);
    csmemcpy(&player->configuration, player_config, sizeof(player->configuration));
    increment_update();
}

long c_network_session_membership::get_peer_from_incoming_address(transport_address const* incoming_address)
{
    ASSERT(get_session());
    ASSERT(incoming_address);
    return DECLFUNC(0x31230, long, __thiscall, c_network_session_membership*, transport_address const*)(this, incoming_address);
}

// Method exists in ms29 but is inlined
void c_network_session_membership::set_peer_connection_state(long peer_index, e_network_session_peer_state state)
{
    ASSERT(state >= 0 && state < k_network_session_peer_state_count);
    ASSERT(peer_index != host_peer_index() || state >= _network_session_peer_state_connected);
    ASSERT(get_session());

    s_network_session_peer* session_peer = get_peer(peer_index);
    session_peer->connection_state = state;
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_connection_state: [%s] peer #%d [%s] set to state %s (%d msec from start)\n",
        get_session()->get_id_string(),
        peer_index,
        transport_secure_address_get_string(&session_peer->secure_address),
        network_session_peer_state_get_string(state),
        network_time_since(get_creation_timestamp(peer_index)));
    if (state == _network_session_peer_state_established)
    {
        set_join_nonce(peer_index, 0);
    }
    increment_update();
}

s_network_session_peer* c_network_session_membership::get_peer(long peer_index)
{
    ASSERT(is_peer_valid(peer_index));
    return &m_baseline.peers[peer_index];
}
const s_network_session_peer* c_network_session_membership::get_peer(long peer_index) const
{
    ASSERT(is_peer_valid(peer_index));
    return &m_baseline.peers[peer_index];
}

s_network_session_player* c_network_session_membership::get_player(long player_index)
{
    ASSERT(is_player_valid(player_index));
    return &m_baseline.players[player_index];
}
const s_network_session_player* c_network_session_membership::get_player(long player_index) const
{
    ASSERT(is_player_valid(player_index));
    return &m_baseline.players[player_index];
}

const char* network_session_peer_state_get_string(e_network_session_peer_state state)
{
    ASSERT(VALID_INDEX(state, k_network_session_peer_state_count));
    return network_session_peer_states[state];
}

void c_network_session_membership::set_join_nonce(long peer_index, qword join_nonce)
{
    get_peer(peer_index)->join_nonce = join_nonce;
}

void c_network_session_membership::increment_update()
{
    m_baseline.update_number++;
    m_player_configuration_version++;
}

long c_network_session_membership::get_player_count() const
{
    return m_baseline.player_count;
}

void c_network_session_membership::idle()
{
    DECLFUNC(0x328E0, void, __thiscall, c_network_session_membership*)(this);
}

bool c_network_session_membership::all_peers_established()
{
    return DECLFUNC(0x20E50, bool, __thiscall, c_network_session_membership*)(this);
}

long c_network_session_membership::get_observer_channel_index(long peer_index) const
{
    ASSERT(is_peer_valid(peer_index));
    return m_local_peer_state[peer_index].channel_index;
}

long c_network_session_membership::get_host_observer_channel_index()
{
    ASSERT(is_peer_valid(host_peer_index()));
    return get_observer_channel_index(host_peer_index());
}

long c_network_session_membership::host_peer_index() const
{
    return m_baseline.host_peer_index;
}

long c_network_session_membership::private_slot_count()
{
    return m_baseline.private_slot_count;
}

long c_network_session_membership::public_slot_count()
{
    return m_baseline.public_slot_count;
}

bool c_network_session_membership::friends_only()
{
    return m_baseline.friends_only;
}

c_network_session* c_network_session_membership::get_session()
{
    return m_session;
}
const c_network_session* c_network_session_membership::get_session() const
{
    return m_session;
}

void c_network_session_membership::set_slot_counts(long private_slot_count, long public_slot_count, bool friends_only)
{
    c_network_session* session = get_session();
    long managed_session_index = session->managed_session_index();
    m_baseline.private_slot_count = private_slot_count;
    m_baseline.public_slot_count = public_slot_count;
    m_baseline.friends_only = friends_only;
    if (session)
    {
        managed_session_modify_slot_counts(managed_session_index, private_slot_count, public_slot_count, friends_only, m_baseline.peer_count);
    }
    increment_update();
}

bool c_network_session_membership::has_peer_ever_been_established(long peer_index)
{
    ASSERT(is_peer_valid(peer_index)); // non-original
    s_network_session_peer* peer = get_peer(peer_index);
    return peer->connection_state == _network_session_peer_state_established || peer->connection_state == _network_session_peer_state_rejoining;
}

// same thing as get_peer w/o the asserts
s_network_session_peer* c_network_session_membership::get_raw_peer(long peer_index)
{
    return &m_baseline.peers[peer_index];
}
const s_network_session_peer* c_network_session_membership::get_raw_peer(long peer_index) const
{
    return &m_baseline.peers[peer_index];
}

// ditto above for get_player
s_network_session_player* c_network_session_membership::get_raw_player(long player_index)
{
    return &m_baseline.players[player_index];
}
const s_network_session_player* c_network_session_membership::get_raw_player(long player_index) const
{
    return &m_baseline.players[player_index];
}

e_network_session_peer_state c_network_session_membership::get_peer_connection_state(long peer_index) const
{
    return get_peer(peer_index)->connection_state;
}

void c_network_session_membership::remove_peer(long peer_index)
{
    ASSERT(get_session());
    s_network_session_peer* raw_peer = get_raw_peer(peer_index);
    char peer_name_string[0x100] = {};
    wchar_string_to_ascii_string(raw_peer->properties.peer_name.get_string(), peer_name_string, 0x100, nullptr);
    // This h3 log doesn't exist in ms23 for some reason
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::remove_peer: [%s] removing peer #%d [%s] name [%s]\n",
        get_session()->get_id_string(),
        peer_index,
        transport_secure_address_get_string(&raw_peer->secure_address),
        peer_name_string);

    DECLFUNC(0x30E50, void, __thiscall, c_network_session_membership*, long)(this, peer_index);

    /* UNFINISHED - i discovered the function still exists midway through rewriting it
    if (get_peer_connection_state(peer_index) >= _network_session_peer_state_connected || get_peer_connection_state(peer_index) == _network_session_peer_state_rejoining)
    {
        s_network_session_peer* peer = get_peer(peer_index);

    }

    remove_peer_internal(peer_index);
    c_network_session_parameters* session_parameters = m_session->get_session_parameters();
    session_parameters->reset_peer_state(peer_index);
    if (m_session->established())
    {
        session_parameters->countdown_timer.set(0, 0, 0, 0);
    }

    // check if host peer is being removed and migrate to a new host functionality not required for HO

    increment_update();
    */
}

const s_network_session_shared_membership* c_network_session_membership::get_current_membership() const
{
    return &m_baseline;
}

// Halo Online tracks the transmitted membership struct for each peer individually
const s_network_session_shared_membership* c_network_session_membership::get_transmitted_membership(long peer_index) const
{
    return &m_transmitted[peer_index];
}

void c_network_session_membership::set_membership_update_number(long peer_index, long update_number)
{
    ASSERT(is_peer_valid(peer_index));
    m_local_peer_state[peer_index].expected_update_number = update_number;
}

void c_network_session_membership::build_membership_update(long peer_index, const s_network_session_shared_membership* membership, const s_network_session_shared_membership* baseline, s_network_message_membership_update* message)
{
    ASSERT(get_session());
    c_network_session* session = get_session();
    ASSERT(session->is_host() && session->established());
    ASSERT(!baseline || baseline->update_number != NONE);
    ASSERT(!baseline || baseline->update_number != membership->update_number);

    c_flags<long, long, k_network_maximum_machines_per_session> peer_info_updated_mask;
    peer_info_updated_mask.clear();
    c_flags<long, long, k_network_maximum_machines_per_session> peers_removed_mask;
    peers_removed_mask.clear();
    
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: [%s] building new update\n", session->get_id_string());
    csmemset(message, 0, sizeof(s_network_message_membership_update));
    managed_session_get_id(session->managed_session_index(), &message->session_id);
    message->update_number = membership->update_number;
    message->incremental_update_number = NONE;

    if (baseline)
    {
        message->incremental_update_number = baseline->update_number;
        message->baseline_checksum = m_transmitted_checksums[peer_index];
    }
    
    // loops copied from h3debug & reach tag debug, because ms23's decompiled code is an unabstracted mess
    for (long i = 0; i < k_network_maximum_machines_per_session; i++)
    {
        const s_network_session_peer* membership_peer = &membership->peers[i];
        const s_network_session_peer* baseline_peer = NULL;
        if (baseline && baseline->peer_valid_mask.test(i))
        {
            baseline_peer = &baseline->peers[i];
        }
    
        if (!baseline_peer || membership->peer_valid_mask.test(i))
        {
            if (membership->peer_valid_mask.test(i))
            {
                s_network_message_membership_update_peer* update_data = &message->peer_updates[message->peer_update_count];

                // Check if peer info has updated
                update_data->peer_info_updated = !baseline_peer
                    || membership_peer->party_nonce != baseline_peer->party_nonce
                    || membership_peer->join_nonce != baseline_peer->join_nonce
                    || membership_peer->join_start_time != baseline_peer->join_start_time;

                // Check if peer properties have updated
                update_data->peer_properties_updated = update_data->peer_info_updated || membership_peer->properties != baseline_peer->properties;

                if (update_data->peer_info_updated || update_data->peer_properties_updated || membership_peer->connection_state != baseline_peer->connection_state)
                {
                    message->peer_update_count++;
                    update_data->peer_index = i;
                    update_data->peer_update_type = 1;
                    update_data->peer_connection_state = membership_peer->connection_state;
                    ASSERT(message->incremental_update_number != NONE || update_data->peer_info_updated);

                    if (update_data->peer_info_updated)
                    {
                        update_data->network_version_number = membership_peer->version;
                        update_data->peer_address = membership_peer->secure_address;
                        update_data->peer_party_nonce = membership_peer->party_nonce;
                        update_data->peer_join_nonce = membership_peer->join_nonce;
                        update_data->peer_creation_timestamp = membership_peer->join_start_time;
                        peer_info_updated_mask.set(i, true);
                    }

                    if (update_data->peer_properties_updated)
                    {
                        const s_network_session_peer_properties* peer_properties = NULL;
                        if (!update_data->peer_info_updated)
                        {
                            peer_properties = &baseline_peer->properties;
                        }
                        build_peer_properties_update(&membership_peer->properties, peer_properties, &update_data->peer_properties);
                    }
                }
            }
        }
        else
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: removing peer #%d [%s]\n",
                i,
                transport_secure_address_get_string(&membership_peer->secure_address));
            message->peer_updates[message->peer_update_count].peer_index = i;
            message->peer_updates[message->peer_update_count].peer_update_type = 0;
            message->peer_update_count++;
            peers_removed_mask.set(i, true);
        }
    }
    for (long i = 0; i < k_network_maximum_players_per_session; i++)
    {
        const s_network_session_player* membership_player = &membership->players[i];
        const s_network_session_player* baseline_player = NULL;
        if (baseline && baseline->player_valid_mask.test(i))
        {
            baseline_player = &baseline->players[i];
        }
        
        if (!baseline_player || membership->player_valid_mask.test(i))
        {
            if (membership->player_valid_mask.test(i) && (!baseline_player || peer_info_updated_mask.test(membership_player->peer_index) || *membership_player != *baseline_player))
            {
                s_network_message_membership_update_player* update_data = &message->player_updates[message->player_update_count++];
                ASSERT(message->player_update_count <= NUMBEROF(message->player_updates));

                update_data->player_index = i;
                update_data->update_type = 1;
                update_data->player_location_updated = (!baseline_player
                    || peer_info_updated_mask.test(membership_player->peer_index)
                    || membership_player->player_identifier != baseline_player->player_identifier
                    || membership_player->peer_index != baseline_player->peer_index
                    || membership_player->player_sequence_number != baseline_player->player_sequence_number
                    || membership_player->left_game != baseline_player->left_game);

                if (update_data->player_location_updated)
                {
                    update_data->identifier = membership_player->player_identifier;
                    update_data->peer_index = (ushort)membership_player->peer_index;
                    update_data->player_addition_number = membership_player->player_sequence_number;
                    update_data->player_occupies_a_public_slot = membership_player->left_game;
                }

                update_data->player_properties_updated = (update_data->player_location_updated
                    || (membership_player->desired_configuration_version != baseline_player->desired_configuration_version
                    || membership_player->controller_index != baseline_player->controller_index
                    || membership_player->configuration != baseline_player->configuration
                    || membership_player->player_voice_settings != baseline_player->player_voice_settings));

                if (update_data->player_properties_updated)
                {
                    update_data->player_update_number = membership_player->desired_configuration_version;
                    update_data->controller_index = membership_player->controller_index;
                    update_data->player_data = membership_player->configuration;
                    update_data->player_voice = membership_player->player_voice_settings;
                }

                ASSERT(update_data->player_location_updated || update_data->player_properties_updated);
                printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: adding player [#%d]\n", i);
            }
        }
        else if (!peers_removed_mask.test(peer_index) && !peer_info_updated_mask.test(peer_index))
        {
            s_network_message_membership_update_player* player_update = &message->player_updates[message->player_update_count++];
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: player removed [#%d]\n", i);
            ASSERT(message->player_update_count <= NUMBEROF(message->player_updates));
            player_update->player_index = i;
            player_update->update_type = 0;
        }
    }
    if (!baseline || membership->player_sequence_number != baseline->player_sequence_number)
    {
        message->player_addition_number_updated = true;
        message->player_addition_number = membership->player_sequence_number;
    }
    if (!baseline || membership->leader_peer_index != baseline->leader_peer_index)
    {
        message->leader_updated = true;
        message->leader_peer_index = membership->leader_peer_index;
    }
    if (!baseline || membership->host_peer_index != baseline->host_peer_index)
    {
        message->host_updated = true;
        message->host_peer_index = membership->host_peer_index;
    }
    if (!baseline
        || membership->private_slot_count != baseline->private_slot_count
        || membership->public_slot_count != baseline->public_slot_count
        || membership->friends_only != baseline->friends_only
        || membership->are_slots_locked != baseline->are_slots_locked)
    {
        message->slot_counts_updated = true;
        message->private_slot_count = membership->private_slot_count;
        message->public_slot_count = membership->public_slot_count;
        message->friends_only = membership->friends_only;
        message->are_slots_locked = membership->are_slots_locked;
    }
    message->checksum = fast_checksum<s_network_session_shared_membership>(fast_checksum_new(), membership);

    //std::ofstream wf(std::format("membership_updates\\membership_update_{}.bin", message->update_number), std::ios::out | std::ios::binary);
    //wf.write((char*)membership, sizeof(s_network_session_shared_membership));
    //wf.close();
}

void c_network_session_membership::build_peer_properties_update(const s_network_session_peer_properties* membership_properties, const s_network_session_peer_properties* baseline_properties, s_network_message_membership_update_peer_properties* peer_properties_update)
{
    if (!baseline_properties
        || !membership_properties->peer_name.is_equal(baseline_properties->peer_name.get_string())
        || !membership_properties->peer_session_name.is_equal(baseline_properties->peer_session_name.get_string()))
    {
        peer_properties_update->peer_name_updated = true;
        peer_properties_update->peer_name.set(membership_properties->peer_name.get_string());
        peer_properties_update->peer_session_name.set(membership_properties->peer_session_name.get_string());
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer properties names changed\n");
    }
    peer_properties_update->game_start_error = membership_properties->game_start_error;
    if (!baseline_properties || membership_properties->peer_map != baseline_properties->peer_map)
    {
        peer_properties_update->peer_map_id_updated = true;
        peer_properties_update->peer_map_id = membership_properties->peer_map;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer properties map id changed\n");
    }
    if (!baseline_properties
        || membership_properties->peer_map_status != baseline_properties->peer_map_status
        || membership_properties->peer_map_progress_percentage != baseline_properties->peer_map_progress_percentage)
    {
        peer_properties_update->peer_map_updated = true;
        peer_properties_update->peer_map_status = membership_properties->peer_map_status;
        peer_properties_update->peer_map_progress_percentage = membership_properties->peer_map_progress_percentage;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer properties map changed\n");
    }
    if (!baseline_properties || membership_properties->peer_mp_map_mask != baseline_properties->peer_mp_map_mask)
    {
        peer_properties_update->available_multiplayer_map_mask_updated = true;
        peer_properties_update->available_multiplayer_map_mask = membership_properties->peer_mp_map_mask;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: available peer properties map mask changed\n");
    }
    if (!baseline_properties || membership_properties->peer_game_instance != baseline_properties->peer_game_instance)
    {
        peer_properties_update->peer_game_instance_updated = true;
        peer_properties_update->peer_game_instance = membership_properties->peer_game_instance;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer game instance changed\n");
    }
    if (!baseline_properties
        || membership_properties->connectivity_badness_rating != baseline_properties->connectivity_badness_rating
        || membership_properties->host_badness_rating != baseline_properties->host_badness_rating
        || membership_properties->client_badness_rating != baseline_properties->client_badness_rating
        || membership_properties->connectivity.peer_connectivity_mask != baseline_properties->connectivity.peer_connectivity_mask
        || membership_properties->connectivity.peer_probe_mask != baseline_properties->connectivity.peer_probe_mask
        || membership_properties->connectivity.latency_minimum_msec != baseline_properties->connectivity.latency_minimum_msec
        || membership_properties->connectivity.latency_average_msec != baseline_properties->connectivity.latency_average_msec
        || membership_properties->connectivity.latency_maximum_msec != baseline_properties->connectivity.latency_maximum_msec
        || membership_properties->language != baseline_properties->language)
    {
        peer_properties_update->peer_connection_updated = true;
        peer_properties_update->connectivity_badness_rating = membership_properties->connectivity_badness_rating;
        peer_properties_update->host_badness_rating = membership_properties->host_badness_rating;
        peer_properties_update->client_badness_rating = membership_properties->client_badness_rating;
        peer_properties_update->connectivity.peer_connectivity_mask = membership_properties->connectivity.peer_connectivity_mask;
        peer_properties_update->connectivity.peer_probe_mask = membership_properties->connectivity.peer_probe_mask;
        peer_properties_update->connectivity.latency_minimum_msec = membership_properties->connectivity.latency_minimum_msec;
        peer_properties_update->connectivity.latency_average_msec = membership_properties->connectivity.latency_average_msec;
        peer_properties_update->connectivity.latency_maximum_msec = membership_properties->connectivity.latency_maximum_msec;
        peer_properties_update->language = membership_properties->language;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer properties connectivity changed\n");
    }

    if (!baseline_properties
        || membership_properties->determinism_version != baseline_properties->determinism_version
        || membership_properties->determinism_compatible_version != baseline_properties->determinism_compatible_version)
    {
        peer_properties_update->versions_updated = true;
        peer_properties_update->determinism_version = membership_properties->determinism_version;
        peer_properties_update->determinism_compatible_version = membership_properties->determinism_compatible_version;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: peer properties versions changed\n");
    }
    if (!baseline_properties || membership_properties->flags != baseline_properties->flags)
    {
        peer_properties_update->flags_updated = true;
        peer_properties_update->flags = membership_properties->flags;
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_peer_properties_update: flags changed\n");
    }
}

void c_network_session_membership::set_peer_address(long peer_index, s_transport_secure_address const* secure_address)
{
    ASSERT(is_host());

    s_network_session_peer* peer = get_peer(peer_index);
    if (peer->secure_address != *secure_address)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_address: [%s] secure address for peer #%d changed to [%s]\n",
            get_session()->get_id_string(),
            peer_index,
            transport_secure_address_get_string(secure_address));
        peer->secure_address = *secure_address;
        increment_update();
    }
}

void c_network_session_membership::set_peer_properties(long peer_index, s_network_session_peer_properties const* peer_properties)
{
    ASSERT(get_session());

    s_network_session_peer* peer = get_peer(peer_index);
    const char* id_string = "shadow";
    if (get_session())
    {
        id_string = get_session()->get_id_string();
    }
    if (peer->properties != *peer_properties)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties\n");
        peer->properties = *peer_properties;
        // observer quality_statistics_notify_established_connectivity removed as estimated bandwidth fields were removed from s_network_session_peer_properties
        increment_update();
    }
    else
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties discarded as irrelevant (from peer #%d [%s])\n",
            id_string,
            peer_index,
            transport_secure_address_get_string(&peer->secure_address));
    }
}

void c_network_session_membership::copy_current_to_transmitted(long peer_index, s_network_session_shared_membership* current_membership)
{
    m_transmitted[peer_index] = *current_membership;
    m_transmitted_checksums[peer_index] = fast_checksum<s_network_session_shared_membership>(fast_checksum_new(), &m_transmitted[peer_index]);
}

qword c_network_session_membership::get_join_nonce(long peer_index) const
{
    return get_peer(peer_index)->join_nonce;
}

bool c_network_session_membership::is_player_in_player_add_queue(s_player_identifier const* player_identifier) const
{
    ASSERT(player_identifier);
    return find_player_in_player_add_queue(player_identifier) != NONE;
}

long c_network_session_membership::find_player_in_player_add_queue(s_player_identifier const* player_identifier) const
{
    return DECLFUNC(0x32D90, long, __thiscall, const c_network_session_membership*, s_player_identifier const* player_identifier)(this, player_identifier);
}

void c_network_session_membership::remove_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    ASSERT(player_identifier);
    DECLFUNC(0x32C00, void, __thiscall, c_network_session_membership*, s_player_identifier const* player_identifier)(this, player_identifier);
}

void c_network_session_membership::commit_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    ASSERT(player_identifier);
    ASSERT(get_session());

    bool player_left_game = false;
    s_player_add_queue_entry* queue_entry = &m_player_add_queue[m_player_add_queue_current_index];
    ASSERT(memcmp(&queue_entry->player_identifier, player_identifier, sizeof(*player_identifier)) == 0);
    // $TODO: Test this assert works as intended, secure address wasn't part of this func call in H3
    const s_network_session_peer* peer = get_peer(queue_entry->peer_index);
    ASSERT(get_session()->can_accept_player_join_request(player_identifier, &peer->secure_address, queue_entry->peer_index, false) == _network_join_refuse_reason_none);

    long queue_player_index = peer->get_player_index();
    if (queue_player_index != NONE)
    {
        player_left_game = get_player(queue_player_index)->left_game;
    }
    long player_index = find_or_add_player(queue_entry->peer_index, &queue_entry->player_identifier, player_left_game);
    ASSERT(player_index != NONE);
    set_player_properties(player_index, queue_entry->player_update_number, queue_entry->controller_index, &queue_entry->client_configuration, queue_entry->voice_settings);
    m_player_add_queue_current_index = (m_player_add_queue_current_index + 1) % 4;
}

void c_network_session_membership::set_player_properties(long player_index, long player_update_number, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings)
{
    DECLFUNC(0x31C10, void, __thiscall, c_network_session_membership*, long, long, long, s_player_configuration_from_client const*, long)(this, player_index, player_update_number, controller_index, player_data_from_client, voice_settings);
}

long c_network_session_membership::get_creation_timestamp(long peer_index)
{
    return get_peer(peer_index)->join_start_time;
}

s_player_add_queue_entry* c_network_session_membership::get_first_player_from_player_add_queue()
{
    if (m_player_add_queue_current_index == m_player_add_queue_count)
    {
        return NULL;
    }
    else
    {
        return &m_player_add_queue[m_player_add_queue_current_index];
    }
}

const s_player_add_queue_entry* c_network_session_membership::get_player_add_queue_entry(long queue_index) const
{
    bool index_in_bounds = m_player_add_queue_current_index <= m_player_add_queue_count;
    if (m_player_add_queue_current_index < m_player_add_queue_count)
    {
        if (queue_index >= m_player_add_queue_current_index && queue_index < m_player_add_queue_count)
        {
            return &m_player_add_queue[queue_index];
        }
        index_in_bounds = m_player_add_queue_current_index <= m_player_add_queue_count;
    }

    if (!index_in_bounds && (queue_index >= m_player_add_queue_current_index || queue_index < m_player_add_queue_count))
    {
        return &m_player_add_queue[queue_index];
    }

    return NULL;
}

void c_network_session_membership::remove_player(long player_index)
{
    ASSERT(get_session());
    c_network_session* session = get_session();
    s_network_session_player* player = get_player(player_index);
    ASSERT(session->established() && session->is_host());
    if (get_peer_connection_state(player->peer_index) >= _network_session_peer_state_joining)
    {
        managed_session_remove_players(get_session()->managed_session_index(), &player->configuration.host.user_xuid, 1);
    }
    remove_player_internal(player_index);
    increment_update();
}

void c_network_session_membership::remove_player_internal(long player_index)
{
    DECLFUNC(0x31B80, void, __thiscall, c_network_session_membership*, long)(this, player_index);
}

long c_network_session_membership::get_player_from_identifier(s_player_identifier const* player_identifier) const
{
    ASSERT(get_session());
    ASSERT(player_identifier);

    // Funny how similar the address is to the call above, really confused me for a moment
    return DECLFUNC(0x318B0, long, __thiscall, const c_network_session_membership*, s_player_identifier const*)(this, player_identifier);
}

bool c_network_session_membership::add_player_to_player_add_queue(s_player_identifier const* player_identifier, long peer_index, long peer_user_index, long controller_index, s_player_configuration_from_client* player_data_from_client, long voice_settings)
{
    ASSERT(player_identifier);
    ASSERT(peer_index >= 0 && peer_index < k_network_maximum_machines_per_session);
    ASSERT(peer_user_index >= 0 && peer_user_index < k_number_of_users);
    ASSERT(player_data_from_client);

    return DECLFUNC(0x32B40, bool, __thiscall, c_network_session_membership*, s_player_identifier const*, long, long, long, s_player_configuration_from_client*, long)(this, player_identifier, peer_index, peer_user_index, controller_index, player_data_from_client, voice_settings);
}

long c_network_session_membership::local_peer_index() const
{
    return m_local_peer_index;
}

long c_network_session_membership::get_peer_count() const
{
    return get_current_membership()->peer_count;
}

void c_network_session_membership::set_peer_needs_reestablishment(long peer_index, bool needs_reestablishment)
{
    ASSERT(is_peer_valid(peer_index));
    m_local_peer_state[peer_index].needs_reestablishment = needs_reestablishment;
}

const s_transport_secure_address* c_network_session_membership::get_peer_address(long peer_index) const
{
    return &get_peer(peer_index)->secure_address;
}

long c_network_session_membership::get_peer_from_observer_channel(long channel_index)
{
    return DECLFUNC(0x31180, long, __thiscall, c_network_session_membership*, long)(this, channel_index);
}

bool c_network_session_membership::host_exists_at_incoming_address(transport_address const* incoming_address)
{
    ASSERT(get_session());
    ASSERT(incoming_address);
    return DECLFUNC(0x31370, bool, __thiscall, c_network_session_membership*, transport_address const*)(this, incoming_address);
}

long c_network_session_membership::leader_peer_index()
{
    return get_current_membership()->leader_peer_index;
}

bool c_network_session_membership::handle_membership_update(s_network_message_membership_update const* message)
{
    ASSERT(get_session());
    c_network_session* session = get_session();
    ASSERT((session->established() || session->peer_joining()) && !session->is_host());

    return DECLFUNC(0x31DD0, bool, __thiscall, c_network_session_membership*, s_network_message_membership_update const*)(this, message);
}

long c_network_session_membership::update_number() const
{
    return get_current_membership()->update_number;
}

long c_network_session_membership::get_membership_update_number(long peer_index)
{
    ASSERT(is_peer_valid(peer_index));
    return m_local_peer_state[peer_index].expected_update_number;
}

bool c_network_session_membership::get_peer_needs_reestablishment(long peer_index)
{
    ASSERT(is_peer_valid(peer_index));
    return m_local_peer_state[peer_index].needs_reestablishment;
}

bool c_network_session_membership::has_membership() const
{
    return update_number() != NONE;
}

void c_network_session_membership::player_data_updated()
{
    increment_update();
}

long c_network_session_membership::get_peer_from_unique_identifier(s_transport_unique_identifier const* unique_identifier) const
{
    ASSERT(get_session());
    const c_network_session* session = get_session();
    ASSERT(unique_identifier);
    if (m_session->current_local_state())
    {
        for (long peer_index = get_first_peer(); peer_index != NONE; peer_index = get_next_peer(peer_index))
        {
            const s_network_session_peer* peer = get_peer(peer_index);
            s_transport_unique_identifier unique_identifier_from_peer;
            transport_secure_address_extract_identifier(&peer->secure_address, &unique_identifier_from_peer);
            if (!csmemcmp(&unique_identifier_from_peer, unique_identifier, sizeof(s_transport_unique_identifier)))
            {
                return peer_index;
            }
        }
    }
    return NONE;
}

bool c_network_session_membership::is_host() const
{
    if (has_membership() && host_peer_index() != NONE)
    {
        return host_peer_index() == m_local_peer_index;
    }
    return false;
}

// user_player_indices doesn't exist in HO so I'm unsure if this method does either
// It's only used in an assert so I'll opt to disable it for now
//const s_network_session_player* c_network_session_membership::get_player_from_peer(long peer_index, long user_index) const
//{
//    const s_network_session_peer* peer = get_peer(peer_index);
//    peer->
//    ASSERT(user_index < k_number_of_users);
//    long player_index = peer->user_player_indices[user_index];
//    if (player_index == NONE)
//    {
//        return NULL;
//    }
//    return get_player(player_index);
//}

long s_network_session_peer::get_player_index() const
{
    return index_from_mask(player_mask.get_bits_direct(), player_mask.MAX_COUNT);
}