#include "network_session_membership.h"
#include "..\..\dllmain.h"
#include "..\network_globals.h"
#include <iostream>
#include "..\network_utilities.h"
#include "network_session.h"
#include "network_managed_session.h"
#include "..\messages\network_message_type_collection.h"

long c_network_session_membership::get_first_peer()
{
    typedef long(__fastcall* get_first_peer_ptr)(c_network_session_membership* session_membership);
    auto get_first_peer = reinterpret_cast<get_first_peer_ptr>(module_base + 0x313F0);
    return get_first_peer(this);
}

long c_network_session_membership::get_next_peer(long peer_index)
{
    long next_index = peer_index + 1;
    if (next_index >= k_network_maximum_machines_per_session)
        return -1;
    while (true)
    {
        if (_bittest((long*)&this->m_baseline.valid_peer_mask, next_index))
            break;
        if (++next_index >= k_network_maximum_machines_per_session)
            return -1;
    }
    return next_index;
}

long c_network_session_membership::get_peer_from_secure_address(s_transport_secure_address const* secure_address)
{
    typedef long(__thiscall* get_peer_from_secure_address_ptr)(c_network_session_membership* session_membership, s_transport_secure_address const* secure_address);
    auto get_peer_from_secure_address = reinterpret_cast<get_peer_from_secure_address_ptr>(module_base + 0x312B0);
    return get_peer_from_secure_address(this, secure_address);
}

bool c_network_session_membership::is_peer_valid(long peer_index)
{
    return (m_baseline.valid_peer_mask >> peer_index) & 1;
}

// TODO - test this
bool c_network_session_membership::is_player_valid(long player_index)
{
    return ((1 << (player_index & 0x1F)) & *(&this->get_current_membership()->valid_player_mask + (player_index >> 5)));
}

bool c_network_session_membership::add_peer(long peer_index, e_network_session_peer_state peer_state, uint32_t joining_network_version_number, s_transport_secure_address const* secure_address, uint64_t join_party_nonce, uint64_t join_nonce)
{
    typedef long(__thiscall* add_peer_ptr)(c_network_session_membership* session_membership, long peer_index, e_network_session_peer_state peer_state, uint32_t joining_network_version_number, s_transport_secure_address const* secure_address, uint64_t join_party_nonce, uint64_t join_nonce);
    auto add_peer_call = reinterpret_cast<add_peer_ptr>(module_base + 0x30DE0);
    return add_peer_call(this, peer_index, peer_state, joining_network_version_number, secure_address, join_party_nonce, join_nonce);
}

long c_network_session_membership::find_or_add_player(long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting)
{
    // rewritten because the existing find_or_add_player method has peer index 0 baked into it and will ignore the actual peer index arg
    //typedef long(__thiscall* find_or_add_player_ptr)(c_network_session_membership* session_membership, long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting);
    //auto find_or_add_player_call = reinterpret_cast<find_or_add_player_ptr>(module_base + 0x31950);
    //return find_or_add_player_call(this, peer_index, player_identifier, join_from_recruiting);

    long(__fastcall * get_player_index_from_mask)(uint32_t * player_mask, long number_of_bits) = reinterpret_cast<decltype(get_player_index_from_mask)>(module_base + 0xC3C10);
    auto peer = this->get_peer(peer_index);
    long player_index = get_player_index_from_mask(&peer->player_mask, 16);
    if (player_index != -1)
    {
        if (this->get_player(player_index)->player_identifier.data == player_identifier->data)
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
    this->get_player(next_player_index)->configuration.client.multiplayer_team = -1;
    this->get_player(next_player_index)->configuration.client.active_armor_loadout = -1;
    this->get_player(next_player_index)->configuration.client.hopper_access_flags = 0;
    this->get_player(next_player_index)->configuration.host.player_team = -1;
    this->get_player(next_player_index)->configuration.host.player_assigned_team = -1;
    this->increment_update();
    return next_player_index;
}

s_network_session_player* c_network_session_membership::add_player_internal(long player_index, s_player_identifier const* player_identifier, long peer_index, long player_sequence_number, bool player_occupies_a_public_slot)
{
    long mask_unknown = 1 << (player_index & 0x1F);
    *(&this->get_current_membership()->valid_player_mask + (player_index >> 5)) |= mask_unknown;
    this->get_current_membership()->player_count++;
    auto player = &this->get_current_membership()->players[player_index];
    player->player_identifier.data = player_identifier->data;
    player->player_sequence_number = player_sequence_number;
    player->peer_index = peer_index;
    player->player_occupies_a_public_slot = player_occupies_a_public_slot;
    player->controller_index = -1;
    if (peer_index != -1)
        *(&this->get_peer(peer_index)->player_mask + (player_index >> 5)) |= mask_unknown;
    player->configuration.host.player_team = -1;
    player->configuration.host.player_assigned_team = -1;
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

// TEST THIS
void c_network_session_membership::set_peer_connection_state(long peer_index, e_network_session_peer_state state) // inlined in ms29
{
    s_network_session_peer* session_peer = this->get_peer(peer_index);
    session_peer->connection_state = state;
    uint32_t time_elapsed = network_get_time() - session_peer->join_start_time;
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_connection_state: [%s] peer #%d [%s] set to state %s (%d msec from start)\n",
        "00:00:00:00:00:00:00:00", // TODO - not sure how to include network_session.h to network_session_membership.h without causing errors to allow this method to run here
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
    // TODO - assert here w/ is_peer_valid
    return &this->m_baseline.peers[peer_index];
}

s_network_session_player* c_network_session_membership::get_player(long player_index)
{
    // TODO - asserts
    return &this->m_baseline.players[player_index];
}

const char* network_session_peer_state_get_string(e_network_session_peer_state state)
{
    return network_session_peer_states[state];
}

void c_network_session_membership::set_join_nonce(long peer_index, uint64_t join_nonce)
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
    void(__thiscall * idle)(c_network_session_membership* membership) = reinterpret_cast<decltype(idle)>(module_base + 0x328E0);
    idle(this);
}

bool c_network_session_membership::all_peers_established()
{
    bool(__thiscall * all_peers_established)(c_network_session_membership* membership) = reinterpret_cast<decltype(all_peers_established)>(module_base + 0x20E50);
    return all_peers_established(this);
}

long c_network_session_membership::get_observer_channel_index(long observer_channel_index)
{
    // TODO ASSERTS
    return this->m_peer_channels[observer_channel_index].channel_index;
}

long c_network_session_membership::get_host_observer_channel_index()
{
    // TODO ASSERTS
    long host_peer_index = this->host_peer_index();
    return this->get_observer_channel_index(host_peer_index);
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
    // char* peer_name_ascii; // TODO
    // wchar_string_to_ascii_string(raw_peer->properties.peer_name, peer_name_ascii, 256, 0); // TODO - convert from wchar to char
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::remove_peer: [%s] removing peer #%d [%s] name [%s]\n", // this log doesn't exist in ms23
        session->get_id_string(),
        peer_index,
        transport_secure_address_get_string(&raw_peer->secure_address),
        /*peer_name_ascii*/"unknown_peer");

    void(__thiscall * remove_peer)(c_network_session_membership* membership, long peer_index) = reinterpret_cast<decltype(remove_peer)>(module_base + 0x30E50);
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
    // TODO - is_peer_valid assert
    this->m_transmitted[peer_index].update_number = update_number;
}

// TODO - test this, especially with the null pointer stuff going on, make sure to confirm that
void c_network_session_membership::build_membership_update(long peer_index, s_network_session_shared_membership* membership, s_network_session_shared_membership* baseline, s_network_message_membership_update* message)
{
    uint32_t unknown1 = 0;
    uint32_t unknown2 = 1;
    uint32_t unknown3 = 0;
    
    printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: [%s] building new update\n",
        this->get_session()->get_id_string());
    memset(message, 0, sizeof(s_network_message_membership_update));
    managed_session_get_id(this->get_session()->managed_session_index(), &message->session_id);
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
        if (baseline != nullptr && ((baseline->valid_peer_mask >> i) & 1)) // test peer bit
            baseline_peer = &baseline->peers[i];
    
        if (baseline_peer == nullptr || ((membership->valid_peer_mask >> i) & 1))
        {
            if ((membership->valid_peer_mask >> i) & 1)
            {
                bool peer_info_updated = (baseline_peer == nullptr
                    || membership_peer->party_nonce != baseline_peer->party_nonce
                    || membership_peer->join_nonce != baseline_peer->join_nonce
                    || membership_peer->join_start_time != baseline_peer->join_start_time);
                bool peer_properties_updated = peer_info_updated || memcmp(&membership_peer->properties, &baseline_peer->properties, sizeof(s_network_session_peer));
                if (peer_info_updated || peer_properties_updated || membership_peer->version != baseline_peer->version)
                {
                    s_network_message_membership_update_peer* update_data = &message->peer_updates[i];
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
            long last_peer_update_index = message->peer_count;
            message->peer_count++; // add 1 peer update
            message->peer_updates[last_peer_update_index].peer_index = i;
            message->peer_updates[last_peer_update_index].peer_update_type = 0;
            unknown3 |= unknown2;
        }
        unknown2 *= 2;
    }
    for (size_t i = 0; i < k_network_maximum_players_per_session; i++)
    {
        s_network_session_player* membership_player = &membership->players[i];
        s_network_session_player* baseline_player = nullptr;
        if (baseline != nullptr && ((baseline->valid_player_mask >> i) & 1)) // test player bit
            baseline_player = &baseline->players[i];
    
        long unknown4 = 1 << membership_player->peer_index;
        if (baseline_player == nullptr || ((membership->valid_player_mask >> i) & 1))
        {
            // blam uses csmemcmp here, which asserts if one of the pointers is null, we're not so we're getting a warning
            if (((membership->valid_player_mask >> i) & 1) && (baseline_player == nullptr || (unknown1 & unknown4) != 0 || memcmp(membership_player, baseline_player, sizeof(s_network_session_player))))
            {
                s_network_message_membership_update_player* update_player = &message->player_updates[message->player_count];
                message->player_count++;
                update_player->player_index = i;
                update_player->update_type = 1;
                update_player->player_location_updated = (baseline_player == nullptr
                                                      || (unknown1 & unknown4) != 0
                                                      || membership_player->player_identifier.data != baseline_player->player_identifier.data
                                                      || membership_player->peer_index != baseline_player->peer_index
                                                      || membership_player->player_sequence_number != baseline_player->player_sequence_number
                                                      || membership_player->player_occupies_a_public_slot != baseline_player->player_occupies_a_public_slot);
                if (update_player->player_location_updated)
                {
                    update_player->identifier.data = membership_player->player_identifier.data;
                    update_player->peer_index = membership_player->peer_index;
                    update_player->player_occupies_a_public_slot = membership_player->player_occupies_a_public_slot;
                }

                update_player->player_properties_updated = (update_player->player_location_updated
                                                         || membership_player->desired_configuration_version != baseline_player->desired_configuration_version
                                                         || membership_player->controller_index != baseline_player->controller_index
                                                         || memcmp(&membership_player->configuration, &baseline_player->configuration, sizeof(s_player_configuration))
                                                         || membership_player->voice_settings != baseline_player->voice_settings);
                if (update_player->player_properties_updated)
                {
                    update_player->player_update_number = membership_player->desired_configuration_version;
                    update_player->controller_index = membership_player->controller_index;
                    memmove(&update_player->configuration, &membership_player->configuration, sizeof(s_player_configuration));
                    update_player->player_voice = membership_player->voice_settings;
                }
                printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: adding player [#%d]\n", i);
            }
            else if ((unknown4 & unknown3) == 0 && (unknown4 & unknown1) == 0)
            {
                s_network_message_membership_update_player* player_update = &message->player_updates[message->player_count];
                message->player_count++;
                printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::build_membership_update: player removed [#%d]\n", i);
                player_update->player_index = i;
                player_update->update_type = 0;
            }
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
    
    auto fast_checksum = fast_checksum_new();
    message->checksum = fast_checksum_s_network_session_shared_membership(fast_checksum, membership);
}

void c_network_session_membership::build_peer_properties_update(s_network_session_peer_properties* membership_properties, s_network_session_peer_properties* baseline_properties, s_network_message_membership_update_peer_properties* peer_properties_update)
{
    if (!baseline_properties
        || wcsncmp(membership_properties->peer_name, baseline_properties->peer_name, 16)
        || wcsncmp(membership_properties->peer_session_name, baseline_properties->peer_session_name, 32))
    {
        peer_properties_update->peer_name_updated = true;
        memcpy(peer_properties_update->peer_name, membership_properties->peer_name, 16);
        memcpy(peer_properties_update->peer_session_name, membership_properties->peer_session_name, 32);
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
        || membership_properties->client_badness_rating != baseline_properties->client_badness_rating)
    {
        peer_properties_update->peer_connection_updated = true;
        peer_properties_update->connectivity_badness_rating = membership_properties->connectivity_badness_rating;
        peer_properties_update->host_badness_rating = membership_properties->host_badness_rating;
        peer_properties_update->client_badness_rating = membership_properties->client_badness_rating;
        peer_properties_update->peer_connectivity_mask = membership_properties->peer_connectivity_mask;
        peer_properties_update->peer_latency_est = membership_properties->peer_latency_est;
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
    if (memcmp(&peer->secure_address, secure_address, sizeof(s_transport_secure_address)))
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_address: [%s] secure address for peer #%d changed to [%s]\n",
            this->get_session()->get_id_string(),
            peer_index,
            transport_secure_address_get_string(secure_address));
        memcpy(&peer->secure_address, secure_address, sizeof(s_transport_secure_address));
        this->increment_update();
    }
}

void c_network_session_membership::set_peer_properties(long peer_index, s_network_session_peer_properties const* peer_properties)
{
    auto peer = this->get_peer(peer_index);
    auto id_string = "shadow";
    if (this->get_session())
        id_string = this->get_session()->get_id_string();
    if (memcmp(&peer->properties, peer_properties, sizeof(s_network_session_peer_properties)))
    {
        printf("MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties\n");
        memcpy(&peer->properties, peer_properties, sizeof(s_network_session_peer_properties));
        // observer quality_statistics_notify_established_connectivity removed as estimated bandwidth fields were removed from s_network_session_peer_properties
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
    auto fast_checksum = fast_checksum_new();
    this->m_transmitted_checksums[peer_index] = fast_checksum_s_network_session_shared_membership(fast_checksum, &this->m_transmitted[peer_index]);
}

long fast_checksum_new()
{
    return -1;
}

// TODO FUNCTION
long fast_checksum_s_network_session_shared_membership(long fast_checksum, s_network_session_shared_membership* shared_membership) // fast_checksum<s_network_session_shared_membership>
{
    //long unknown1 = fast_checksum -
    
    return -1;
}

s_network_session_shared_membership::s_network_session_shared_membership()
{
    for (size_t j = 0; j < k_network_maximum_machines_per_session; j++)
        peers[j].properties.flags = 0;
    for (size_t j = 0; j < k_network_maximum_players_per_session; j++)
    {
        memset(&players[j].configuration.client, 0, sizeof(s_player_configuration_from_client));
        memset(&players[j].configuration.host.player_appearance.unknown_struct, 0, sizeof(s_player_appearance_unknown1));
        memset(&players[j].configuration.host, 0, sizeof(s_player_configuration_from_host));
        players[j].configuration.host.player_assigned_team = -1;
        players[j].configuration.host.player_team = -1;
    }
};

uint64_t c_network_session_membership::get_join_nonce(long peer_index)
{
    return this->get_peer(peer_index)->join_nonce;
}

bool c_network_session_membership::is_player_in_player_add_queue(s_player_identifier const* player_identifier)
{
    // TODO - asserts here
    return this->find_player_in_player_add_queue(player_identifier) != -1;
}

long c_network_session_membership::find_player_in_player_add_queue(s_player_identifier const* player_identifier)
{
    long(__thiscall * find_player_in_player_add_queue)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(find_player_in_player_add_queue)>(module_base + 0x32D90);
    return find_player_in_player_add_queue(this, player_identifier);
}

void c_network_session_membership::remove_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    void(__thiscall * remove_player_from_player_add_queue)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(remove_player_from_player_add_queue)>(module_base + 0x32C00);
    return remove_player_from_player_add_queue(this, player_identifier);
}

void c_network_session_membership::commit_player_from_player_add_queue(s_player_identifier const* player_identifier)
{
    long(__fastcall* get_player_index_from_mask)(uint32_t* player_mask, long number_of_bits) = reinterpret_cast<decltype(get_player_index_from_mask)>(module_base + 0xC3C10);

    auto queue_entry = &this->m_player_add_queue[this->m_player_add_queue_current_index];
    bool sequence_number_not_zero = false;
    long mask_player_index = get_player_index_from_mask(&this->m_baseline.peers[queue_entry->player_index].player_mask, 16);
    if (mask_player_index != -1)
        sequence_number_not_zero = this->m_baseline.players[mask_player_index].player_sequence_number != 0;
    long player_index = this->find_or_add_player(queue_entry->player_index, &queue_entry->player_identifier, sequence_number_not_zero);
    this->set_player_properties(player_index, queue_entry->output_user_index, queue_entry->controller_index, &queue_entry->client_configuration, queue_entry->voice_settings);
    this->m_player_add_queue_current_index = (this->m_player_add_queue_current_index + 1) % 4;
}

void c_network_session_membership::set_player_properties(long player_index, long desired_configuration_version, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings)
{
    void(__thiscall * set_player_properties)(c_network_session_membership* thisptr, long player_index, long desired_configuration_version, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings) = reinterpret_cast<decltype(set_player_properties)>(module_base + 0x31C10);
    return set_player_properties(this, player_index, desired_configuration_version, controller_index, player_data_from_client, voice_settings);
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
        managed_session_remove_players(this->get_session()->managed_session_index(), (uint64_t*)&player->configuration.host.player_xuid.data, 1);
    this->remove_player_internal(player_index);
    this->increment_update();
}

void c_network_session_membership::remove_player_internal(long player_index)
{
    void(__thiscall * remove_player_internal)(c_network_session_membership* thisptr, long player_index) = reinterpret_cast<decltype(remove_player_internal)>(module_base + 0x31B80);
    remove_player_internal(this, player_index);
}

long c_network_session_membership::get_player_from_identifier(s_player_identifier const* player_identifier)
{
    long(__thiscall * get_player_from_identifier)(c_network_session_membership* thisptr, s_player_identifier const* player_identifier) = reinterpret_cast<decltype(get_player_from_identifier)>(module_base + 0x318B0); // funny how similar the address is to the call above
    return get_player_from_identifier(this, player_identifier);
}