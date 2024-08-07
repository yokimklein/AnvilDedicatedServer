#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_security.h>
#include <networking\transport\transport_address.h>
#include <game\players.h>
#include <simulation\simulation.h>
#include <networking\session\network_session_parameters_session.h>

struct s_network_session_join_request;

// POTENTIALLY INACCURATE! may produce invalid error logs - TODO: finish verifying enum entries
enum e_network_join_refuse_reason // based off mcc h3 - ms29's differs from mcc's slightly (check against mcc odst?)
{
	_network_join_refuse_reason_none, // verified in ms23
	_network_join_refuse_reason_tried_to_join_self,
	_network_join_refuse_reason_could_not_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_not_found,
	_network_join_refuse_reason_privacy_mode,
	_network_join_refuse_reason_not_joinable, // verified in ms23
	_network_join_refuse_reason_session_full, // verified in ms23
	_network_join_refuse_reason_alpha_split_screen,
	_network_join_refuse_reason_session_disband, // verified in ms23
	_network_join_refuse_reason_session_booted, // verified in ms23
	_network_join_refuse_reason_address_invalid,
	_network_join_refuse_reason_address_failed,
	_network_join_refuse_reason_too_many_observers,
	_network_join_refuse_reason_aborted,
	_network_join_refuse_reason_abort_ignored,
	_network_join_refuse_reason_wrong_payload_type,
	_network_join_refuse_reason_no_reservation,
	_network_join_refuse_reason_in_matchmaking, // verified in ms23
	_network_join_refuse_reason_player_count_zero,
	_network_join_refuse_reason_player_not_online_enabled,
	_network_join_refuse_reason_player_add_pending,
	_network_join_refuse_reason_player_add_failed, // verified in ms23 & ms29
	_network_join_refuse_reason_host_time_out, // verified in ms23 & ms29
	_network_join_refuse_reason_rejected_by_host,
	_network_join_refuse_reason_peer_version_too_low, // verified in ms23
	_network_join_refuse_reason_host_version_too_low, // verified in ms23
	_network_join_refuse_reason_holding_in_queue, // verified in ms23 & ms29
	_network_join_refuse_reason_film_in_progress, // verified in ms23
	_network_join_refuse_reason_campaign_in_progress, // verified in ms23 & ms29
	_network_join_refuse_reason_user_content_not_allowed,
	_network_join_refuse_reason_survival_in_progress, // verified in ms23
	_network_join_refuse_reason_executable_type_mismatch, // verified in ms23 & ms29? did this move? this was a lot further up in MCC

	k_network_join_refuse_reason_count
};

enum e_network_join_queue_mode
{
    _network_join_closed_to_all_joins = 0,
    _network_join_open_to_join_squad,
    _network_join_not_used_00, // Used to be _network_join_queue_joins_to_group in older builds
    _network_join_not_used_01, // Used to be _network_join_process_queued_group_joins in older builds
    _network_join_queue_closed_while_in_match,

    k_network_join_queue_mode_count
};

enum e_join_local_state
{
    _join_local_state_none = 0,
    _join_local_state_start_join_squad,
    _join_local_state_joining_squad,
    _join_local_state_leaving_old_squad,
    _join_local_state_processing_join_to_us,

    k_join_local_state_count
};

#pragma pack(push, 4)
struct s_joining_player
{
    struct s_global_stats
    {
        bool valid;
        ulong experience_base;
        ulong experience_penalty;
        ulong highest_skill;
    };
    struct s_hopper
    {
        bool stats_valid;
        ushort identifier;
        ulong mu;
        ulong sigma;
        ulong old_skill;
        ulong games_played;
        ulong games_completed;
        ulong games_won;
    };

    qword joining_peer_player_id;
    qword joining_peer_player_xuids;
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
    ulong joining_network_version_number;
    ulong user_player_index;
};
static_assert(sizeof(s_joining_peer) == 0x18);

struct s_network_session_join_request
{
    qword join_nonce;
    qword join_party_nonce;
    long joining_peer_count;
    s_joining_peer joining_peers[k_network_maximum_machines_per_session];
    long joining_player_count;
    s_player_identifier joining_players[k_network_maximum_players_per_session];
    bool join_from_recruiting;
    ulong payload_type;
};
static_assert(sizeof(s_network_session_join_request) == 0x238);

struct s_join_queue_entry
{
    s_transport_address address;
    qword join_nonce;
    s_network_session_join_request join_request;
    ulong times[3]; // 0 - queue entry time, 1 - last updated time, 2 - latency?
};
static_assert(sizeof(s_join_queue_entry) == 0x260);

struct s_networking_join_data
{
    ulong local_join_state; // 0x1039AFC
    ulong local_join_result;
    ulong time;
    ulong unknown1; // maybe bool created_session_for_remote_join
    ulong unknown2;
    s_network_session_remote_session_join_data join_data;
    c_enum<e_network_join_queue_mode, long, _network_join_closed_to_all_joins, k_network_join_queue_mode_count> join_queue_mode; // 0x1039B60
    long join_queue_entry_count; // 0x1039B64
    s_join_queue_entry join_queue[32]; // 32 - queue for channels?
};
static_assert(sizeof(s_networking_join_data) == 0x4C6C); // ?
#pragma pack(pop)

class c_network_session;
void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request);
bool network_join_process_joins_from_queue();
void __cdecl network_join_flush_join_queue();
void network_join_remove_join_from_queue(qword join_nonce);

extern s_networking_join_data& g_network_join_data;