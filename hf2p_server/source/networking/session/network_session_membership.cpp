#include "network_session_membership.h"
#include "..\network_globals.h"
#include <iostream>
#include "..\network_utilities.h"
#include "network_session.h"
#include "network_managed_session.h"
#include "..\messages\network_message_type_collection.h"
#include "..\..\math\fast_checksum.h"
#include "..\..\text\unicode.h"
#include "assert.h"

char const* network_session_peer_states[k_network_session_peer_state_count] = {
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

long c_network_session_membership::get_first_peer()
{
    for (long i = 0; i < k_network_maximum_machines_per_session; i++)
    {
        if (this->is_peer_valid(i))
            return i;
    }
    return -1;
}

long c_network_session_membership::get_next_peer(long peer_index)
{
    for (long i = peer_index + 1; i < k_network_maximum_machines_per_session; i++)
    {
        if (this->is_peer_valid(i))
            return i;
    }
    return -1;
}

long c_network_session_membership::get_first_player()
{
    for (long i = 0; i < k_network_maximum_players_per_session; i++)
    {
        if (this->is_player_valid(i))
            return i;
    }
    return -1;
}

long c_network_session_membership::get_next_player(long player_index)
{
    for (long i = player_index + 1; i < k_network_maximum_players_per_session; i++)
    {
        if (this->is_player_valid(i))
            return i;
    }
    return -1;
}

long c_network_session_membership::get_peer_from_secure_address(s_transport_secure_address const* secure_address)
{
    typedef long(__thiscall* get_peer_from_secure_address_ptr)(c_network_session_membership* session_membership, s_transport_secure_address const* secure_address);
    auto get_peer_from_secure_address = reinterpret_cast<get_peer_from_secure_address_ptr>(module_base + 0x312B0);
    return get_peer_from_secure_address(this, secure_address);
}

bool c_network_session_membership::is_peer_valid(long peer_index)
{
    return (this->get_current_membership()->peer_valid_mask >> peer_index) & 1;
}

bool c_network_session_membership::is_player_valid(long player_index)
{
    return (this->get_current_membership()->player_valid_flags >> player_index) & 1;
}

bool c_network_session_membership::add_peer(long peer_index, e_network_session_peer_state peer_state, ulong joining_network_version_number, s_transport_secure_address const* secure_address, qword join_party_nonce, qword join_nonce)
{
    typedef long(__thiscall* add_peer_ptr)(c_network_session_membership* session_membership, long peer_index, e_network_session_peer_state peer_state, ulong joining_network_version_number, s_transport_secure_address const* secure_address, qword join_party_nonce, qword join_nonce);
    auto add_peer_call = reinterpret_cast<add_peer_ptr>(module_base + 0x30DE0);
    return add_peer_call(this, peer_index, peer_state, joining_network_version_number, secure_address, join_party_nonce, join_nonce);
}

long c_network_session_membership::find_or_add_player(long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting)
{
    // rewritten because the existing find_or_add_player method has peer index 0 baked into it and will ignore the actual peer index arg
    //typedef long(__thiscall* find_or_add_player_ptr)(c_network_session_membership* session_membership, long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting);
    //auto find_or_add_player_call = reinterpret_cast<find_or_add_player_ptr>(module_base + 0x31950);
    //return find_or_add_player_call(this, peer_index, player_identifier, join_from_recruiting);

    long(__fastcall* get_player_index_from_mask)(ulong* player_mask, long number_of_bits) = reinterpret_cast<decltype(get_player_index_from_mask)>(module_base + 0xC3C10);
    auto peer = this->get_peer(peer_index);
    long player_index = get_player_index_from_mask(&peer->player_mask, 16);
    if (player_index != -1)
    {
        if (csmemcmp(&this->get_player(player_index)->player_identifier, player_identifier, sizeof(s_player_identifier)) == 0)
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: peer #%d [%s] already has correct player [#%d]\n",
                this->get_session()->get_id_string(),
                peer_index,
                transport_secure_address_get_string(&peer->secure_address),
                player_index);
            return player_index;
        }
        else
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: peer #%d [%s] already has player [#%d] with different identifier, removing\n",
                this->get_session()->get_id_string(), 
                peer_index,
                transport_secure_address_get_string(&peer->secure_address),
                player_index);
            this->remove_player(player_index);
            player_index = -1;
        }
    }
    long player_index_from_identifier = this->get_player_from_identifier(player_identifier);
    if (player_index_from_identifier != -1)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::find_or_add_player: [%s] find_or_add_player: player index %d has the same player identifier as the one we're adding, removing them\n",
            this->get_session()->get_id_string(),
            player_index_from_identifier);
        this->remove_player(player_index_from_identifier);
    }
    long next_player_index;
    for (next_player_index = 0; next_player_index < k_network_maximum_players_per_session; next_player_index++)
    {
        if (!this->is_player_valid(next_player_index))
            break;
    }
    this->add_player_internal(next_player_index, player_identifier, peer_index, this->get_current_membership()->player_sequence_number, join_from_recruiting);
    this->get_current_membership()->player_sequence_number = (this->get_current_membership()->player_sequence_number + 1) % 0x100000;
    this->get_player(next_player_index)->configuration.client.user_selected_team_index = -1;
    this->get_player(next_player_index)->configuration.client.active_armor_loadout = -1;
    this->get_player(next_player_index)->configuration.client.player_is_griefer = false;
    this->get_player(next_player_index)->configuration.host.team_index = -1;
    this->get_player(next_player_index)->configuration.host.user_selected_team_index = -1;
    this->increment_update();
    return next_player_index;
}

s_network_session_player* c_network_session_membership::add_player_internal(long player_index, s_player_identifier const* player_identifier, long peer_index, long player_sequence_number, bool player_occupies_a_public_slot)
{
    long mask_unknown = 1 << (player_index & 0x1F);
    *(&this->get_current_membership()->player_valid_flags + (player_index >> 5)) |= mask_unknown;
    this->get_current_membership()->player_count++;
    auto player = &this->get_current_membership()->players[player_index];
    player->player_identifier = *player_identifier;
    player->peer_index = peer_index;
    player->player_sequence_number = this->m_baseline.player_sequence_number;
    player->player_occupies_a_public_slot = player_occupies_a_public_slot;
    player->controller_index = -1;
    if (peer_index != -1)
        *(&this->get_peer(peer_index)->player_mask + (player_index >> 5)) |= mask_unknown;
    player->configuration.host.team_index = -1;
    player->configuration.host.user_selected_team_index = -1;
    return player;
}

void c_network_session_membership::update_player_data(long player_index, s_player_configuration const* player_config)
{
    memmove(&this->m_baseline.players[player_index].configuration, player_config, sizeof(s_player_configuration));
    this->increment_update();
}

long c_network_session_membership::get_peer_from_incoming_address(s_transport_address const* incoming_address)
{
    typedef long(__thiscall* get_peer_from_incoming_address_ptr)(c_network_session_membership* session_membership, s_transport_address const* incoming_address);
    auto get_peer_from_incoming_address_call = reinterpret_cast<get_peer_from_incoming_address_ptr>(module_base + 0x31230);
    return get_peer_from_incoming_address_call(this, incoming_address);
}

void c_network_session_membership::set_peer_connection_state(long peer_index, e_network_session_peer_state state) // inlined in ms29
{
    s_network_session_peer* session_peer = this->get_peer(peer_index);
    session_peer->connection_state = state;
    ulong time_elapsed = network_get_time() - session_peer->join_start_time;
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_connection_state: [%s] peer #%d [%s] set to state %s (%d msec from start)\n",
        this->get_session()->get_id_string(),
        peer_index,
        transport_secure_address_get_string(&session_peer->secure_address),
        network_session_peer_state_get_string(state),
        time_elapsed);
    if (state == _network_session_peer_state_established)
        this->set_join_nonce(peer_index, 0);
    this->increment_update();
}

s_network_session_peer* c_network_session_membership::get_peer(long peer_index)
{
    assert(is_peer_valid(peer_index));
    return &m_baseline.peers[peer_index];
}

s_network_session_player* c_network_session_membership::get_player(long player_index)
{
    assert(is_player_valid(player_index));
    return &m_baseline.players[player_index];
}

const char* network_session_peer_state_get_string(e_network_session_peer_state state)
{
    return network_session_peer_states[state];
}

void c_network_session_membership::set_join_nonce(long peer_index, qword join_nonce)
{
    this->get_peer(peer_index)->join_nonce = join_nonce;
}

void c_network_session_membership::increment_update()
{
    this->m_baseline.update_number++;
    this->m_player_configuration_version++;
}

long c_network_session_membership::get_player_count()
{
    return this->m_baseline.player_count;
}

void c_network_session_membership::idle()
{
    void(__thiscall* idle)(c_network_session_membership* membership) = reinterpret_cast<decltype(idle)>(module_base + 0x328E0);
    idle(this);
}

bool c_network_session_membership::all_peers_established()
{
    bool(__thiscall* all_peers_established)(c_network_session_membership* membership) = reinterpret_cast<decltype(all_peers_established)>(module_base + 0x20E50);
    return all_peers_established(this);
}

long c_network_session_membership::get_observer_channel_index(long peer_index)
{
    assert(is_peer_valid(peer_index));
    return m_peer_channels[peer_index].channel_index;
}

long c_network_session_membership::get_host_observer_channel_index()
{
    assert(is_peer_valid(host_peer_index()));
    return get_observer_channel_index(host_peer_index());
}

long c_network_session_membership::host_peer_index()
{
    return this->m_baseline.host_peer_index;
}

long c_network_session_membership::private_slot_count()
{
    return this->m_baseline.private_slot_count;
}

long c_network_session_membership::public_slot_count()
{
    return this->m_baseline.public_slot_count;
}

bool c_network_session_membership::friends_only()
{
    return this->m_baseline.friends_only;
}

c_network_session* c_network_session_membership::get_session()
{
    return this->m_session;
}

void c_network_session_membership::set_slot_counts(long private_slot_count, long public_slot_count, bool friends_only)
{
    auto session = this->get_session();
    long managed_session_index = session->managed_session_index();
    this->m_baseline.private_slot_count = private_slot_count;
    this->m_baseline.public_slot_count = public_slot_count;
    this->m_baseline.friends_only = friends_only;
    if (session)
        managed_session_modify_slot_counts(managed_session_index, private_slot_count, public_slot_count, friends_only, this->m_baseline.peer_count);
    this->increment_update();
}

bool c_network_session_membership::has_peer_ever_been_established(long peer_index)
{
    auto peer = this->get_peer(peer_index);
    return (peer->connection_state == _network_session_peer_state_established || peer->connection_state == _network_session_peer_state_rejoining);
}

s_network_session_peer* c_network_session_membership::get_raw_peer(long peer_index)
{
    // same thing as get_peer w/o the asserts
    return &this->m_baseline.peers[peer_index];
}

e_network_session_peer_state c_network_session_membership::get_peer_connection_state(long peer_index)
{
    return this->get_peer(peer_index)->connection_state;
}

void c_network_session_membership::remove_peer(long peer_index)
{
    auto raw_peer = this->get_raw_peer(peer_index);
    auto session = this->get_session();
    // this h3 log doesn't exist in ms23 for some reason
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::remove_peer: [%s] removing peer #%d [%s] name [%S]\n",
        session->get_id_string(),
        peer_index,
        transport_secure_address_get_string(&raw_peer->secure_address),
        raw_peer->properties.peer_name); // originally calls wchar_string_to_ascii_string, but we might as well just print the original wchar string

    void(__thiscall* remove_peer)(c_network_session_membership* membership, long peer_index) = reinterpret_cast<decltype(remove_peer)>(module_base + 0x30E50);
    return remove_peer(this, peer_index);

    /* UNFINISHED - i discovered the function still exists midway through rewriting it
    if (this->get_peer_connection_state(peer_index) >= _network_session_peer_state_connected || this->get_peer_connection_state(peer_index) == _network_session_peer_state_rejoining)
    {
        auto peer = this->get_peer(peer_index);

    }

    this->remove_peer_internal(peer_index);
    auto session_parameters = session->get_session_parameters();
    session_parameters->reset_peer_state(peer_index);
    if (session->established())
    {
        session_parameters->countdown_timer.set(0, 0, 0, 0);
    }

    // check if host peer is being removed and migrate to a new host functionality not required for HO

    this->increment_update();
    */
}

s_network_session_shared_membership* c_network_session_membership::get_current_membership()
{
    return &this->m_baseline;
}

s_network_session_shared_membership* c_network_session_membership::get_transmitted_membership(long peer_index)
{
    return &this->m_transmitted[peer_index];
}

void c_network_session_membership::set_membership_update_number(long peer_index, long update_number)
{
    assert(is_peer_valid(peer_index));
    m_peer_channels[peer_index].expected_update_number = update_number;
}

// TODO - figure out what the unknowns are doing
void c_network_session_membership::build_membership_update(long peer_index, s_network_session_shared_membership* membership, s_network_session_shared_membership* baseline, s_network_message_membership_update* message)
{
    long unknown1 = 0;
    long unknown2 = 1;
    long unknown3 = 0;
    
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: [%s] building new update\n",
        this->get_session()->get_id_string());
    memset(message, 0, sizeof(s_network_message_membership_update));
    this->get_session()->get_session_id(&message->session_id);
    message->update_number = membership->update_number;
    message->incremental_update_number = -1;
    if (baseline != nullptr)
    {
        message->incremental_update_number = baseline->update_number;
        message->baseline_checksum = this->m_transmitted_checksums[peer_index];
    }
    
    // loops copied from h3debug & reach tag debug, because ms23's decompiled code is an unabstracted mess
    for (size_t i = 0; i < k_network_maximum_machines_per_session; i++)
    {
        s_network_session_peer* membership_peer = &membership->peers[i];
        s_network_session_peer* baseline_peer = nullptr;
        if (baseline != nullptr && ((baseline->peer_valid_mask >> i) & 1)) // test peer bit
            baseline_peer = &baseline->peers[i];
    
        if (baseline_peer == nullptr || ((membership->peer_valid_mask >> i) & 1))
        {
            if ((membership->peer_valid_mask >> i) & 1)
            {
                bool peer_info_updated = (baseline_peer == nullptr
                    || membership_peer->party_nonce != baseline_peer->party_nonce
                    || membership_peer->join_nonce != baseline_peer->join_nonce
                    || membership_peer->join_start_time != baseline_peer->join_start_time);
                bool peer_properties_updated = peer_info_updated || memcmp(&membership_peer->properties, &baseline_peer->properties, sizeof(s_network_session_peer)) != 0;
                if (peer_info_updated || peer_properties_updated || membership_peer->version != baseline_peer->version)
                {
                    long next_available_update_index = message->peer_update_count;
                    s_network_message_membership_update_peer* update_data = &message->peers[next_available_update_index];
                    message->peer_update_count++;
                    update_data->peer_index = i;
                    update_data->peer_update_type = 1;
                    update_data->peer_connection_state = membership_peer->connection_state;
                    update_data->peer_info_updated = peer_info_updated;
                    if (peer_info_updated)
                    {
                        update_data->network_version_number = membership_peer->version;
                        update_data->peer_address = membership_peer->secure_address;
                        update_data->peer_party_nonce = membership_peer->party_nonce;
                        update_data->peer_join_nonce = membership_peer->join_nonce;
                        update_data->peer_creation_timestamp = membership_peer->join_start_time;
                        unknown1 |= unknown2;
                    }
                    update_data->peer_properties_updated = peer_properties_updated;
                    if (peer_properties_updated)
                    {
                        s_network_session_peer_properties* peer_properties = nullptr;
                        if (!peer_info_updated)
                            peer_properties = &baseline_peer->properties;
                        this->build_peer_properties_update(&membership_peer->properties, peer_properties, &update_data->peer_properties_update);
                    }
                }
            }
        }
        else
        {
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: removing peer #%d [%s]\n",
                i,
                transport_secure_address_get_string(&membership_peer->secure_address));
            long next_available_update_index = message->peer_update_count;
            message->peer_update_count++; // add 1 peer update
            message->peers[next_available_update_index].peer_index = i;
            message->peers[next_available_update_index].peer_update_type = 0;
            unknown3 |= unknown2;
        }
        unknown2 *= 2;
    }
    for (size_t i = 0; i < k_network_maximum_players_per_session; i++)
    {
        s_network_session_player* membership_player = &membership->players[i];
        s_network_session_player* baseline_player = nullptr;
        if (baseline != nullptr && ((baseline->player_valid_flags >> i) & 1)) // test player bit
            baseline_player = &baseline->players[i];
    
        long unknown4 = 1 << membership_player->peer_index;
        if (baseline_player == nullptr || ((membership->player_valid_flags >> i) & 1))
        {
            if (((membership->player_valid_flags >> i) & 1) && (baseline_player == nullptr || (unknown1 & unknown4) != 0 || memcmp(membership_player, baseline_player, sizeof(s_network_session_player)) != 0))
            {
                s_network_message_membership_update_player* update_player = &message->players[message->player_update_count];
                message->player_update_count++;
                update_player->player_index = i;
                update_player->update_type = 1;
                update_player->player_location_updated = (baseline_player == nullptr
                                                      || (unknown1 & unknown4) != 0
                                                      || csmemcmp(&membership_player->player_identifier, &baseline_player->player_identifier, sizeof(s_player_identifier)) != 0
                                                      || membership_player->peer_index != baseline_player->peer_index
                                                      || membership_player->player_sequence_number != baseline_player->player_sequence_number
                                                      || membership_player->player_occupies_a_public_slot != baseline_player->player_occupies_a_public_slot);
                if (update_player->player_location_updated)
                {
                    update_player->identifier = membership_player->player_identifier;
                    update_player->peer_index = (ushort)membership_player->peer_index;
                    update_player->player_addition_number = membership_player->player_sequence_number;
                    update_player->player_occupies_a_public_slot = membership_player->player_occupies_a_public_slot;
                }

                update_player->player_properties_updated = (update_player->player_location_updated == true
                                                         || (membership_player->desired_configuration_version != baseline_player->desired_configuration_version
                                                         || membership_player->controller_index != baseline_player->controller_index
                                                         || memcmp(&membership_player->configuration, &baseline_player->configuration, sizeof(s_player_configuration)) != 0
                                                         || membership_player->voice_settings != baseline_player->voice_settings));
                if (update_player->player_properties_updated)
                {
                    update_player->player_update_number = membership_player->desired_configuration_version;
                    update_player->controller_index = membership_player->controller_index;
                    memmove(&update_player->player_data, &membership_player->configuration, sizeof(s_player_configuration));
                    update_player->player_voice = membership_player->voice_settings;
                }
                printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: adding player [#%d]\n", i);
            }
        }
        else if ((unknown4 & unknown3) == 0 && (unknown4 & unknown1) == 0)
        {
            s_network_message_membership_update_player* player_update = &message->players[message->player_update_count];
            message->player_update_count++;
            printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: player removed [#%d]\n", i);
            player_update->player_index = i;
            player_update->update_type = 0;
        }
    }
    if (baseline == nullptr || membership->player_sequence_number != baseline->player_sequence_number)
    {
        message->player_addition_number_updated = true;
        message->player_addition_number = membership->player_sequence_number;
    }
    if (baseline == nullptr || membership->leader_peer_index != baseline->leader_peer_index)
    {
        message->leader_updated = true;
        message->leader_peer_index = membership->leader_peer_index;
    }
    if (baseline == nullptr || membership->host_peer_index != baseline->host_peer_index)
    {
        message->host_updated = true;
        message->host_peer_index = membership->host_peer_index;
    }
    if (baseline == nullptr
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

    message->checksum = fast_checksum(membership, sizeof(s_network_session_shared_membership), fast_checksum_new());
}

void c_network_session_membership::build_peer_properties_update(s_network_session_peer_properties* membership_properties, s_network_session_peer_properties* baseline_properties, s_network_message_membership_update_peer_properties* peer_properties_update)
{
    if (!baseline_properties
        || ustrncmp(membership_properties->peer_name, baseline_properties->peer_name, 16)
        || ustrncmp(membership_properties->peer_session_name, baseline_properties->peer_session_name, 32))
    {
        peer_properties_update->peer_name_updated = true;
        ustrnzcpy(peer_properties_update->peer_name, membership_properties->peer_name, 16);
        ustrnzcpy(peer_properties_update->peer_session_name, membership_properties->peer_session_name, 32);
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
        peer_properties_update->peer_game_instance_exists = true;
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
    auto peer = this->get_peer(peer_index);
    if (memcmp(&peer->secure_address, secure_address, sizeof(s_transport_secure_address)) != 0)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_address: [%s] secure address for peer #%d changed to [%s]\n",
            this->get_session()->get_id_string(),
            peer_index,
            transport_secure_address_get_string(secure_address));
        peer->secure_address = *secure_address;
        this->increment_update();
    }
}

void c_network_session_membership::set_peer_properties(long peer_index, s_network_session_peer_properties const* peer_properties)
{
    long(__fastcall* get_player_index_from_mask)(ulong* player_mask, long number_of_bits) = reinterpret_cast<decltype(get_player_index_from_mask)>(module_base + 0xC3C10);

    auto peer = this->get_peer(peer_index);
    auto id_string = "shadow";
    if (this->get_session())
        id_string = this->get_session()->get_id_string();
    if (memcmp(&peer->properties, peer_properties, sizeof(s_network_session_peer_properties)) != 0)
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties\n");
        memcpy(&peer->properties, peer_properties, sizeof(s_network_session_peer_properties));
        // observer quality_statistics_notify_established_connectivity removed as estimated bandwidth fields were removed from s_network_session_peer_properties

        // TODO - TEMPORARY ANVIL CODE (NON-ORIGINAL)
        // set the player name based on their peer name
        // we'll eventually reimplement network_session_check_properties to handle names, teams and player loadouts
        auto baseline = this->get_current_membership();
        auto peer_name = peer_properties->peer_name;
        long player_index = get_player_index_from_mask(&peer->player_mask, 16);
        if (player_index != -1)
        {
            auto player = &baseline->players[player_index];
            if (peer_name != L"" && (ustrncmp(player->configuration.host.player_name, peer_name, 16) != 0))
            {
                ustrnzcpy(player->configuration.host.player_name, peer_name, 16);
                ustrnzcpy(player->configuration.client.name, peer_name, 16);
            }
        }

        this->increment_update();
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
    memmove(&this->m_transmitted[peer_index], current_membership, sizeof(s_network_session_shared_membership));
    this->m_transmitted_checksums[peer_index] = fast_checksum(&this->m_transmitted[peer_index], sizeof(s_network_session_shared_membership), fast_checksum_new());
}

qword c_network_session_membership::get_join_nonce(long peer_index)
{
    return this->get_peer(peer_index)->join_nonce;
}

bool c_network_session_membership::is_player_in_player_add_queue(s_player_identifier const* player_identifier)
{
    assert(player_identifier);
    return find_player_in_player_add_queue(player_identifier) != -1;
}

long c_network_session_membership::find_player_in_player_add_queue(s_player_identifier const* player_identifier)
{
    long(__thiscall* find_player_in_player_add_queue)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(find_player_in_player_add_queue)>(module_base + 0x32D90);
    return find_player_in_player_add_queue(this, player_identifier);
}

void c_network_session_membership::remove_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    void(__thiscall* remove_player_from_player_add_queue)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(remove_player_from_player_add_queue)>(module_base + 0x32C00);
    return remove_player_from_player_add_queue(this, player_identifier);
}

void c_network_session_membership::commit_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    long(__fastcall* get_player_index_from_mask)(ulong* player_mask, long number_of_bits) = reinterpret_cast<decltype(get_player_index_from_mask)>(module_base + 0xC3C10);

    auto queue_entry = &this->m_player_add_queue[this->m_player_add_queue_current_index];
    bool sequence_number_not_zero = false;
    long mask_player_index = get_player_index_from_mask(&this->m_baseline.peers[queue_entry->player_index].player_mask, 16);
    if (mask_player_index != -1)
        sequence_number_not_zero = this->m_baseline.players[mask_player_index].player_occupies_a_public_slot != 0;
    long player_index = this->find_or_add_player(queue_entry->player_index, &queue_entry->player_identifier, sequence_number_not_zero);
    this->set_player_properties(player_index, queue_entry->player_update_number, queue_entry->controller_index, &queue_entry->client_configuration, queue_entry->voice_settings);
    this->m_player_add_queue_current_index = (this->m_player_add_queue_current_index + 1) % 4;
}

void c_network_session_membership::set_player_properties(long player_index, long player_update_number, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings)
{
    void(__thiscall* set_player_properties)(c_network_session_membership* thisptr, long player_index, long desired_configuration_version, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings) = reinterpret_cast<decltype(set_player_properties)>(module_base + 0x31C10);
    return set_player_properties(this, player_index, player_update_number, controller_index, player_data_from_client, voice_settings);
}

long c_network_session_membership::get_creation_timestamp(long peer_index)
{
    return this->get_peer(peer_index)->join_start_time;
}

s_player_add_queue_entry* c_network_session_membership::get_first_player_from_player_add_queue()
{
    if (this->m_player_add_queue_current_index == this->m_player_add_queue_count)
        return nullptr;
    else
        return &this->m_player_add_queue[this->m_player_add_queue_current_index];
}

void c_network_session_membership::remove_player(long player_index)
{
    auto player = this->get_player(player_index);
    if (this->get_peer_connection_state(player->peer_index) >= _network_session_peer_state_joining)
        managed_session_remove_players(this->get_session()->managed_session_index(), (qword*)&player->configuration.host.player_xuid, 1);
    this->remove_player_internal(player_index);
    this->increment_update();
}

void c_network_session_membership::remove_player_internal(long player_index)
{
    void(__thiscall* remove_player_internal)(c_network_session_membership* thisptr, long player_index) = reinterpret_cast<decltype(remove_player_internal)>(module_base + 0x31B80);
    remove_player_internal(this, player_index);
}

long c_network_session_membership::get_player_from_identifier(s_player_identifier const* player_identifier)
{
    long(__thiscall* get_player_from_identifier)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(get_player_from_identifier)>(module_base + 0x318B0); // funny how similar the address is to the call above
    return get_player_from_identifier(this, player_identifier);
}

bool c_network_session_membership::add_player_to_player_add_queue(s_player_identifier const* player_identifier, long peer_index, long peer_user_index, long controller_index, s_player_configuration_from_client* player_data_from_client, long voice_settings)
{
    bool(__thiscall* add_player_to_player_add_queue)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier, long peer_index, long peer_user_index, long controller_index, s_player_configuration_from_client* player_data_from_client, long voice_settings) = reinterpret_cast<decltype(add_player_to_player_add_queue)>(module_base + 0x32B40);
    return add_player_to_player_add_queue(this, player_identifier, peer_index, peer_user_index, controller_index, player_data_from_client, voice_settings);
}

long c_network_session_membership::local_peer_index()
{
    return this->m_local_peer_index;
}

long c_network_session_membership::get_peer_count()
{
    return this->get_current_membership()->peer_count;
}

void c_network_session_membership::set_peer_needs_reestablishment(long peer_index, bool needs_reestablishment)
{
    assert(is_peer_valid(peer_index));
    m_peer_channels[peer_index].needs_reestablishment = needs_reestablishment;
}

s_transport_secure_address* c_network_session_membership::get_peer_address(long peer_index)
{
    return &this->get_peer(peer_index)->secure_address;
}

long c_network_session_membership::get_peer_from_observer_channel(long channel_index)
{
    long(__thiscall* get_peer_from_observer_channel)(c_network_session_membership* thisptr, long channel_index) = reinterpret_cast<decltype(get_peer_from_observer_channel)>(module_base + 0x31180);
    return get_peer_from_observer_channel(this, channel_index);
}

bool c_network_session_membership::host_exists_at_incoming_address(s_transport_address const* incoming_address)
{
    bool(__thiscall* host_exists_at_incoming_address)(c_network_session_membership* thisptr, s_transport_address const* incoming_address) = reinterpret_cast<decltype(host_exists_at_incoming_address)>(module_base + 0x31370);
    return host_exists_at_incoming_address(this, incoming_address);
}

long c_network_session_membership::leader_peer_index()
{
    return this->get_current_membership()->leader_peer_index;
}

bool c_network_session_membership::handle_membership_update(s_network_message_membership_update const* message)
{
    typedef bool(__thiscall* handle_membership_update_ptr)(c_network_session_membership* thisptr, s_network_message_membership_update const* message);
    auto handle_membership_update = reinterpret_cast<handle_membership_update_ptr>(module_base + 0x31DD0);
    return handle_membership_update(this, message);
}

long c_network_session_membership::update_number()
{
    return this->get_current_membership()->update_number;
}

long c_network_session_membership::get_membership_update_number(long peer_index)
{
    return this->m_peer_channels[peer_index].expected_update_number;
}

bool c_network_session_membership::get_peer_needs_reestablishment(long peer_index)
{
    return this->m_peer_channels[peer_index].needs_reestablishment;
}

bool c_network_session_membership::has_membership()
{
    return this->update_number() != -1;
}