#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_security.h>
#include <networking\transport\transport_address.h>
#include <game\players.h>
#include <simulation\simulation.h>
#include <networking\session\network_session_parameters_session.h>

enum e_network_join_refuse_reason
{
	_network_join_refuse_reason_none,
	_network_join_refuse_reason_tried_to_join_self,
    _network_join_refuse_reason_unable_to_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_not_found,
    _network_join_refuse_reason_session_privacy,
	_network_join_refuse_reason_not_joinable,
	_network_join_refuse_reason_session_full,
    _network_join_refuse_reason_no_split_screen_on_alpha,
    _network_join_refuse_reason_session_disbanded,
	_network_join_refuse_reason_session_boot,
	_network_join_refuse_reason_address_invalid,
    _network_join_refuse_reason_address_decode_failed,
	_network_join_refuse_reason_too_many_observers,
    _network_join_refuse_reason_abort_successful,
	_network_join_refuse_reason_abort_ignored,
	_network_join_refuse_reason_wrong_payload_type,
	_network_join_refuse_reason_no_reservation,
	_network_join_refuse_reason_in_matchmaking,
	_network_join_refuse_reason_player_count_zero,
	_network_join_refuse_reason_player_not_online_enabled,
	_network_join_refuse_reason_player_add_pending,
	_network_join_refuse_reason_player_add_failed,
	_network_join_refuse_reason_host_time_out,
	_network_join_refuse_reason_rejected_by_host,
	_network_join_refuse_reason_peer_version_too_low,
	_network_join_refuse_reason_host_version_too_low,
	_network_join_refuse_reason_holding_in_queue,
	_network_join_refuse_reason_film_in_progress,
	_network_join_refuse_reason_campaign_in_progress,
	_network_join_refuse_reason_user_content_not_allowed,
	_network_join_refuse_reason_survival_in_progress,
	_network_join_refuse_reason_executable_type_mismatch,

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

struct s_network_session_join_request_player
{
    s_player_identifier joining_peer_player_id;
};
static_assert(sizeof(s_network_session_join_request_player) == sizeof(s_player_identifier));

struct s_network_session_join_request_peer
{
    s_transport_secure_address joining_peer_address;
    long joining_network_version_number;
    long user_player_index;
};
static_assert(sizeof(s_network_session_join_request_peer) == 0x18);

//struct s_group_session_join_request_payload
//{
//    long payload_type;
//    union
//    {
//        s_matchmaking_gather_party_properties gather_party_properties;
//        s_matchmaking_search_party_properties search_party_properties;
//    };
//};
//static_assert(sizeof(s_group_session_join_request_payload) == 0x4);

struct s_network_session_join_request
{
    qword join_nonce;
    qword party_nonce;
    long joining_peer_count;
    s_network_session_join_request_peer joining_peers[k_network_maximum_machines_per_session];
    long joining_player_count;
    s_network_session_join_request_player joining_players[k_network_maximum_players_per_session];
    bool join_to_public_slots;
    // alignment leaves 7 bytes of padding here
    //s_group_session_join_request_payload payload; // likely removed since ms23
};
static_assert(sizeof(s_network_session_join_request) == 0x238);
static_assert(OFFSETOF(s_network_session_join_request, join_nonce) == 0x00);
static_assert(OFFSETOF(s_network_session_join_request, party_nonce) == 0x08);
static_assert(OFFSETOF(s_network_session_join_request, joining_peer_count) == 0x10);
static_assert(OFFSETOF(s_network_session_join_request, joining_peers) == 0x14);
static_assert(OFFSETOF(s_network_session_join_request, join_to_public_slots) == 0x230); // $TODO: confirm this

struct s_networking_join_queue_entry
{
    transport_address address;
    s_network_session_join_request join_request; // need offs
    ulong time_inserted_into_the_queue;
    ulong time_client_was_last_notified;
    long desirability; // entirely unused now
};
static_assert(sizeof(s_networking_join_queue_entry) == 0x260);
static_assert(OFFSETOF(s_networking_join_queue_entry, address) == 0x00);
//static_assert(OFFSETOF(s_networking_join_queue_entry, join_request) == 0x18); // not referenced in code in ms29, but it's offset would be sizeof(transport_address) aligned to 8 bytes
static_assert(OFFSETOF(s_networking_join_queue_entry, time_inserted_into_the_queue) == 0x250);
static_assert(OFFSETOF(s_networking_join_queue_entry, time_client_was_last_notified) == 0x254);

struct s_networking_join_data
{
    bool disable_joins;

    e_join_local_state local_join_state;
    e_life_cycle_join_result local_join_result;
    ulong timeout_start_time;
    bool client_already_joining;
    s_network_session_remote_session_join_data queued_squad_join;

    //bool request_join_squad_to_group;
    //bool request_join_squad_to_target_group;
    //bool request_join_group_to_group;
    //s_network_session_remote_session_join_data requested_group_join;
    //e_life_cycle_join_result group_join_result;
    //s_group_session_join_request_payload requested_payload;
    //bool request_leave_group;
    //e_networking_join_destination_squad requested_group_to_leave;

    e_network_join_queue_mode join_queue_mode;
    long join_queue_entry_count;
    //long number_of_peers_expected_in_membership_at_last_desiribility_calculation;
    s_networking_join_queue_entry join_queue[32];
};
static_assert(sizeof(s_networking_join_data) == 0x4C70);
static_assert(OFFSETOF(s_networking_join_data, disable_joins) == 0x00);
static_assert(OFFSETOF(s_networking_join_data, local_join_state) == 0x04);
static_assert(OFFSETOF(s_networking_join_data, local_join_result) == 0x08);
static_assert(OFFSETOF(s_networking_join_data, timeout_start_time) == 0x0C);
static_assert(OFFSETOF(s_networking_join_data, client_already_joining) == 0x10);
static_assert(OFFSETOF(s_networking_join_data, queued_squad_join) == 0x18);
static_assert(OFFSETOF(s_networking_join_data, join_queue_mode) == 0x68);
static_assert(OFFSETOF(s_networking_join_data, join_queue_entry_count) == 0x6C);
static_assert(OFFSETOF(s_networking_join_data, join_queue) == 0x70);

class c_network_session;
void network_join_add_join_to_queue(c_network_session* session, transport_address const* address, s_network_session_join_request const* join_request);
bool network_join_process_joins_from_queue();
void __cdecl network_join_flush_join_queue();
void network_join_remove_join_from_queue(qword join_nonce);

extern s_networking_join_data& g_network_join_data;