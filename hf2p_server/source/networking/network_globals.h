#pragma once
#include "..\dllmain.h"
#include "transport\transport_address.h"
#include "transport\transport_security.h"
#include "..\simulation\simulation.h"
#include "..\game\players.h"

enum e_network_session_type;
enum e_network_join_mode;

enum e_join_remote_state : long
{

};

enum e_join_type : long
{

};

enum e_life_cycle_join_result : long
{

};

#pragma pack(push, 4)
struct s_joining_player
{
    struct s_global_stats
    {
        bool valid;
        uint32_t experience_base;
        uint32_t experience_penalty;
        uint32_t highest_skill;
    };
    struct s_hopper
    {
        bool stats_valid;
        uint16_t identifier;
        uint32_t mu;
        uint32_t sigma;
        uint32_t old_skill;
        uint32_t games_played;
        uint32_t games_completed;
        uint32_t games_won;
    };

    int64_t joining_peer_player_id;
    int64_t joining_peer_player_xuids;
    bool joining_peer_is_silver_or_gold_live;
    bool joining_peer_is_online_enabled;
    bool joining_peer_is_user_created_content_allowed;
    bool joining_peer_is_friend_created_content_allowed;

    s_joining_player::s_global_stats global_stats;
    s_joining_player::s_hopper hopper;

};
static_assert(sizeof(s_joining_player) == 0x40);

struct s_joining_peer
{
    s_transport_secure_address joining_peer_address;
    uint32_t joining_network_version_number;
    uint32_t user_player_index;
};
static_assert(sizeof(s_joining_peer) == 0x18);

struct s_network_session_join_request
{
    uint64_t join_nonce;
    uint64_t join_party_nonce;
    int32_t joining_peer_count;
    s_joining_peer joining_peers[k_network_maximum_machines_per_session];
    int32_t joining_player_count;
    s_player_identifier joining_players[k_network_maximum_players_per_session];
    bool join_from_recruiting;
    uint32_t payload_type;
};
static_assert(sizeof(s_network_session_join_request) == 0x238);

struct s_join_queue_entry
{
    s_transport_address address;
    uint64_t join_nonce;
    s_network_session_join_request join_request;
    uint32_t times[3]; // 0 - queue entry time, 1 - last updated time, 2 - latency?
};
static_assert(sizeof(s_join_queue_entry) == 0x260);

struct s_network_session_remote_session_join_data
{
    e_join_remote_state join_state; // aka join_status
    e_join_type join_to;
    uint64_t join_nonce;
    e_transport_platform platform;
    s_transport_secure_identifier session_id;
    s_transport_secure_key session_key;
    s_transport_secure_address host_secure_address;
    e_network_session_type session_class;
    e_life_cycle_join_result join_result;
    bool join_to_public_slots;
};
static_assert(sizeof(s_network_session_remote_session_join_data) == 0x50);

struct s_networking_join_data
{
    uint32_t local_join_state; // 0x1039AFC
    uint32_t local_join_result;
    uint32_t time;
    uint32_t unknown1; // maybe bool created_session_for_remote_join
    uint32_t unknown2;
    s_network_session_remote_session_join_data join_data;
    e_network_join_mode join_queue_mode; // 0x1039B60
    long join_queue_entry_count; // 0x1039B64
    s_join_queue_entry join_queue[32]; // 32 - queue for channels?
};
static_assert(sizeof(s_networking_join_data) == 0x4C6C); // ?
#pragma pack(pop)

static s_networking_join_data* g_network_join_data = (s_networking_join_data*)(module_base + 0x1039AFC);

// belongs in network_time.h
static bool* network_time_locked = (bool*)(module_base + 0x1038344);
static uint32_t* g_network_locked_time = (uint32_t*)(module_base + 0x1038348);