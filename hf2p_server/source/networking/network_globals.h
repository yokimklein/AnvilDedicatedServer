#pragma once
#include "..\dllmain.h"
#include "transport\transport_address.h"
#include "transport\transport_security.h"

enum e_network_session_type;
enum e_network_join_status;

enum e_join_remote_state : long
{

};

enum e_join_type : long
{

};

enum e_life_cycle_join_result : long
{

};

struct s_global_stats
{
    bool valid;
    char __align1[3];
    uint32_t experience_base;
    uint32_t experience_penalty;
    uint32_t highest_skill;
};
struct s_hopper
{
    bool stats_valid;
    char __align1;
    uint16_t identifier;
    uint32_t mu;
    uint32_t sigma;
    uint32_t old_skill;
    uint32_t games_played;
    uint32_t games_completed;
    uint32_t games_won;
};
struct s_joining_player
{
    int64_t joining_peer_player_id;
    int64_t joining_peer_player_xuids;
    bool joining_peer_is_silver_or_gold_live;
    bool joining_peer_is_online_enabled;
    bool joining_peer_is_user_created_content_allowed;
    bool joining_peer_is_friend_created_content_allowed;

    s_global_stats global_stats;
    s_hopper hopper;
};

struct s_joining_peer
{
    s_transport_secure_address joining_peer_address;
    uint32_t joining_network_version_number;
    uint32_t user_player_indices[4];
};
/*
struct s_matchmaking_session_preferences
{
    uint32_t search_preference;
    uint32_t search_min_skill;
    uint32_t search_max_skill;
};
struct s_matchmaking_desirability
{
    uint16_t hopper_identifier;
    byte hopper_ranked;
    char __align3;
    uint32_t player_count;
    uint32_t player_valid_flags;
    uint32_t player_peer[16][2]; // this is probably a struct with a size of 8 bytes
    uint32_t avg_skill;
    uint32_t max_skill;
    uint32_t min_skill;
    uint32_t avg_mu;
    uint32_t avg_rank;
    uint32_t party_needed;
    uint32_t party_join;
    uint32_t good_host_count;
    uint32_t party_nat_type;
    uint32_t gamer_zone;
    uint32_t gamer_region;
    uint32_t language;
    uint32_t nat_type;
    byte flags;
    char : 8; // align
    short : 16; // align
};
struct s_matchmaking_search_party_properties
{
    uint32_t search_seconds;
    uint32_t search_latency;
    s_matchmaking_session_preferences search_party_preferences;
    s_matchmaking_desirability matchmaking_desirability;
};
*/
struct s_network_session_join_request
{
    int64_t join_nonce;
    int64_t join_party_nonce;
    uint32_t joining_peer_count;
    s_joining_peer joining_peers[17];
    uint32_t joining_player_count;
    __int32 : 32;
    s_joining_player joining_players[16];
    bool join_from_recruiting;
    __int8 : 8;
    __int16 : 16;
    uint32_t payload_type;
    //s_matchmaking_search_party_properties search_party_properties; // cut from ms29
};

#pragma pack(push, 4)
struct s_join_queue_entry
{
    s_transport_address address;
    int64_t join_nonce;
    char __align1C_maybe[4];
    s_network_session_join_request join_request;
    uint32_t times[2];
    uint32_t latency_maybe;
    char __align704_maybe[4];
};
#pragma pack(pop)

struct s_network_session_remote_session_join_data
{
    e_join_remote_state join_state; // aka join_status
    e_join_type join_to;
    int64_t join_nonce;
    e_transport_platform platform;
    s_transport_secure_identifier session_id;
    s_transport_secure_key session_key;
    s_transport_secure_address host_secure_address;
    e_network_session_type session_class;
    e_life_cycle_join_result join_result;
    bool join_to_public_slots;
};

#pragma pack(push, 4)
struct s_networking_join_data
{
    uint32_t __unknown0; // 0x1039AFC
    uint32_t local_join_result;
    uint32_t time;
    char __unknownC;
    char __alignD[7];
    s_network_session_remote_session_join_data join_data;
    e_network_join_status join_queue_mode; // 0x1039B60
    long join_queue_entry_count;
    s_join_queue_entry join_queue[32];
};
#pragma pack(pop)

static s_networking_join_data* g_network_join_data = (s_networking_join_data*)(module_base + 0x1039AFC);