#pragma once
#include "..\transport\transport_security.h"
#include "..\session\network_session_membership.h"
#include "..\network_globals.h"

enum e_network_message_type : long
{
	_network_message_type_ping,
	_network_message_type_pong,
	//_network_message_type_broadcast_search, // removed in ms29
	//_network_message_type_broadcast_reply, // removed in ms29
	_network_message_type_connect_request,
	_network_message_type_connect_refuse,
	_network_message_type_connect_establish,
	_network_message_type_connect_closed,
	_network_message_type_join_request,
	_network_message_type_peer_connect,
	_network_message_type_join_abort,
	_network_message_type_join_refuse,
	_network_message_type_leave_session,
	_network_message_type_leave_acknowledge,
	_network_message_type_session_disband,
	_network_message_type_session_boot,
	_network_message_type_host_decline,
	_network_message_type_peer_establish,
	_network_message_type_time_synchronize,
	_network_message_type_membership_update,
	_network_message_type_peer_properties,
	_network_message_type_delegate_leadership,
	_network_message_type_boot_machine,
	_network_message_type_player_add,
	_network_message_type_player_refuse,
	_network_message_type_player_remove,
	_network_message_type_player_properties,
	_network_message_type_parameters_update,
	_network_message_type_parameters_request,
	_network_message_type_view_establishment,
	_network_message_type_player_acknowledge,
	_network_message_type_synchronous_update,
	_network_message_type_synchronous_playback_control,
	_network_message_type_synchronous_actions,
	_network_message_type_synchronous_acknowledge,
	_network_message_type_synchronous_gamestate,
	//_network_message_type_distributed_game_results, // removed in ms29
	_network_message_type_synchronous_client_ready,
	_network_message_type_game_results,
	_network_message_type_test,

	k_network_message_type_count
};

// TODO - update k_join_refuse_reason_strings whenever this is updated
// verified against ms23, not ms29 - INACCURATE! TODO will produce invalid error logs!
enum e_network_join_refuse_reason : long // based off mcc h3 - TODO, verify whether these are correct for ms29, ms29's differs from mcc's slightly (check mcc odst's when it releases?)
{
	_network_join_refuse_reason_none, // verified
	_network_join_refuse_reason_tried_to_join_self,
	_network_join_refuse_reason_could_not_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_not_found,
	_network_join_refuse_reason_privacy_mode,
	_network_join_refuse_reason_not_joinable, // verified
	_network_join_refuse_reason_session_full, // verified
	_network_join_refuse_reason_alpha_split_screen,
	_network_join_refuse_reason_session_disband, // verified
	_network_join_refuse_reason_session_booted, // verified
	_network_join_refuse_reason_address_invalid,
	_network_join_refuse_reason_address_failed,
	_network_join_refuse_reason_too_many_observers,
	_network_join_refuse_reason_aborted,
	_network_join_refuse_reason_abort_ignored,
	_network_join_refuse_reason_wrong_payload_type,
	_network_join_refuse_reason_no_reservation,
	_network_join_refuse_reason_in_matchmaking, // verified
	_network_join_refuse_reason_player_count_zero,
	_network_join_refuse_reason_player_not_online_enabled,
	_network_join_refuse_reason_player_add_pending,
	_network_join_refuse_reason_player_add_failed,
	_network_join_refuse_reason_host_time_out, // verified (verified in ms29 too)
	_network_join_refuse_reason_rejected_by_host,
	_network_join_refuse_reason_peer_version_too_low, // verified
	_network_join_refuse_reason_host_version_too_low, // verified
	_network_join_refuse_reason_holding_in_queue, // verified (in ms29 too)
	_network_join_refuse_reason_film_in_progress, // verified
	_network_join_refuse_reason_campaign_in_progress, // verified (verified in ms29 too)
	_network_join_refuse_reason_user_content_not_allowed,
	_network_join_refuse_reason_survival_in_progress, // verified
	_network_join_refuse_reason_executable_type_mismatch, // verified? did this move? TODO - verify

	k_network_join_refuse_reason_count
};

enum e_network_join_mode : long // unofficial name, i couldn't find the original
{
	_network_join_closed_to_all_joins, // these are official though
	_network_join_open_to_join_squad,
	_network_join_queue_joins_to_group,
	_network_join_process_queued_group_joins,
	_network_join_queue_closed_while_in_match,

	k_network_join_status_count
};

struct s_network_message
{
	// leftover notes
	// struct has no members
	
	//long channel_identifier; // transport_secure_identifier?
	//long next_update_number; // time sent? flags?
	//long chunk_size; // s_transport_secure_address?
	//long incremental_update_number; // enum? -1 == complete, else incremental
	//void* baseline_checksum;
	// protocol_version
	// payload
};

struct s_network_message_ping : s_network_message
{
	uint32_t id;
	uint32_t timestamp;
	uint32_t request_qos;
};

struct s_network_message_pong : s_network_message
{
	uint32_t id;
	uint32_t timestamp;
	uint32_t request_qos;
};
static_assert(sizeof(s_network_message_pong) == 0xC);

struct s_network_message_connect_request : s_network_message
{
	uint32_t identifier;
	uint32_t flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse : s_network_message
{
	uint32_t remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish : s_network_message
{
	uint32_t identifier;
	uint32_t remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed : s_network_message
{
	uint32_t identifier;
	uint32_t remote_identifier;
	uint32_t closure_reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);

struct s_network_message_join_request : s_network_message
{
	uint16_t protocol_version;
	short executable_type;
	long executable_version;
	long compatible_version;
	s_transport_secure_identifier session_id;
	long unknown;
	s_network_session_join_request data;
};
static_assert(sizeof(s_network_message_join_request) == 0x258);

struct s_network_message_peer_connect : s_network_message
{
	uint16_t protocol_version;
	short : 16;
	s_transport_secure_identifier session_id;
	long : 32;
	uint64_t join_nonce;
};
static_assert(sizeof(s_network_message_peer_connect) == 0x20);

struct s_network_message_join_abort : s_network_message
{
	s_transport_secure_identifier session_id;
	uint64_t join_nonce;
};
static_assert(sizeof(s_network_message_join_abort) == 0x18);

struct s_network_message_join_refuse : s_network_message
{
	s_transport_secure_identifier session_id;
	e_network_join_refuse_reason reason;
};
static_assert(sizeof(s_network_message_join_refuse) == 0x14);

struct s_network_message_leave_session : s_network_message
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_session) == 0x10);

struct s_network_message_leave_acknowledge : s_network_message
{
	uint64_t session_id;
};

struct s_network_message_session_disband : s_network_message
{
	s_transport_secure_identifier session_id;
};

struct s_network_message_session_boot : s_network_message
{
	s_transport_secure_identifier session_id;
	enum e_network_session_boot_reason reason;
};
static_assert(sizeof(s_network_message_session_boot) == 0x14);

struct s_network_message_host_decline : s_network_message
{
	s_transport_secure_identifier session_id;
	bool session_exists;
	bool peer_exists;

	bool host_exists;
	char : 8;

	s_transport_secure_address host_address;
};

struct s_network_message_peer_establish : s_network_message
{

};

struct s_network_message_time_synchronize : s_network_message
{
	s_transport_secure_identifier session_id;
	uint32_t client_timestamp[2];
	uint32_t authority_timestamp[2];
	uint32_t synchronization_stage;
};
//static_assert(sizeof(s_network_message_time_synchronize) == 0x1C);

struct s_network_message_membership_update_player
{
	uint32_t player_index;
	uint32_t update_type;
	bool player_location_updated;
	uint64_t identifier;
	uint16_t peer_index;
	uint16_t peer_user_index;
	bool player_occupies_a_public_slot;
	bool player_properties_updated;
	uint32_t player_update_number;
	uint32_t controller_index;
	s_player_configuration configuration;
	uint32_t player_voice;
};
static_assert(sizeof(s_network_message_membership_update_player) == 0xBA0);

struct s_network_message_membership_update_peer_properties
{
	bool peer_name_updated;
	wchar_t peer_name[16]; // offset 60? code uses 58
	wchar_t peer_session_name[32]; // offset 92? code uses 90
	uint32_t game_start_error;
	bool peer_map_id_updated;
	uint32_t peer_map_id;
	bool peer_map_updated;
	uint32_t peer_map_status; // 0-4
	uint32_t peer_map_progress_percentage; // 0-100
	bool peer_game_instance_exists;
	uint64_t peer_game_instance;
	bool available_multiplayer_map_mask_updated;
	uint32_t available_multiplayer_map_mask;
	bool peer_connection_updated;
	uint32_t connectivity_badness_rating;
	uint32_t host_badness_rating;
	uint32_t client_badness_rating;
	uint32_t language; // or nat_type? probably not
	uint16_t peer_connectivity_mask;
	uint16_t peer_probe_mask;
	uint32_t peer_latency_min;
	uint32_t peer_latency_est;
	uint32_t peer_latency_max;
	bool versions_updated;
	uint32_t determinism_version;
	uint32_t determinism_compatible_version;
	bool flags_updated;
	uint32_t flags;
};
static_assert(sizeof(s_network_message_membership_update_peer_properties) == 0xC8);

struct s_network_message_membership_update_peer
{
	uint32_t peer_index;
	uint32_t peer_update_type;
	uint32_t peer_connection_state;
	bool peer_info_updated;
	s_transport_secure_address peer_address;
	uint64_t peer_party_nonce;
	uint64_t peer_join_nonce;
	uint32_t network_version_number;
	uint32_t peer_creation_timestamp;
	bool peer_properties_updated;
	uint32_t unknown; // weird gap in struct here, what is this?
	s_network_message_membership_update_peer_properties peer_properties_update;
};
static_assert(sizeof(s_network_message_membership_update_peer) == 0x108);

struct s_network_message_membership_update : s_network_message // this is actually a class?
{
	s_transport_secure_identifier session_id;
	uint32_t update_number;
	uint32_t incremental_update_number;
	uint32_t baseline_checksum;
	uint16_t peer_count; // max 34?
	uint16_t player_count; // max 32?
	s_network_message_membership_update_peer peer_update[k_network_maximum_machines_per_session];
	s_network_message_membership_update_player player_update[k_network_maximum_players_per_session];
	bool player_addition_number_updated;
	uint32_t player_addition_number;
	bool leader_updated;
	uint32_t leader_peer_index;
	bool host_updated;
	uint32_t host_peer_index;
	bool slot_counts_updated;
	uint32_t private_slot_count;
	uint32_t public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	uint32_t checksum;
};
static_assert(sizeof(s_network_message_membership_update) == 0xCBD8);

struct s_network_message_peer_properties : s_network_message
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address secure_address;

	s_network_session_peer_properties peer_properties;
};

struct s_network_message_delegate_leadership : s_network_message
{

};

struct s_network_message_boot_machine : s_network_message
{

};

struct s_network_message_player_add : s_network_message
{

};

struct s_network_message_player_refuse : s_network_message
{
	s_transport_secure_identifier session_id;
	s_transport_secure_identifier player_identifier;
	uint32_t user_index;
	uint32_t refuse_reason;
};

struct s_network_message_player_remove : s_network_message
{

};

struct s_network_message_player_properties : s_network_message
{

};

struct s_network_message_parameters_update : s_network_message
{
	s_transport_secure_identifier session_id;
	bool initial_update;

	char : 8;
	short : 16;
	long : 32;

	uint64_t cleared_parameters;
	uint64_t updated_parameters;

	char parameters[6144];
};

struct s_network_message_parameters_request : s_network_message
{

};

struct s_network_message_view_establishment : s_network_message
{

};

struct s_network_message_player_acknowledge : s_network_message
{

};

struct s_network_message_synchronous_update : s_network_message
{

};

struct s_network_message_synchronous_playback_control : s_network_message
{

};

struct s_network_message_synchronous_actions : s_network_message
{

};

struct s_network_message_synchronous_acknowledge : s_network_message
{

};

struct s_network_message_synchronous_gamestate : s_network_message
{

};

struct s_network_message_synchronous_client_ready : s_network_message
{

};

struct s_network_message_game_results : s_network_message
{

};

struct c_network_message_type
{
	bool __unknown0;
	const char* message_type_name;
	uint32_t flags;
	uint32_t message_size;
	uint32_t message_size_maximum;
	void* encode_function;
	void* decode_function;
	void* compare_function;
	void* dispose_function;
};
static_assert(sizeof(c_network_message_type) == 0x24);

class c_network_message_type_collection
{
public:
	// char handle_ping(c_network_message_handler* message_handler, s_transport_address outgoing_address, s_network_message_ping* message_ping);
	const char* get_message_type_name(e_network_message_type message_type);
private:
	c_network_message_type message_types[k_network_message_type_count];
	//
	//
	//
	//
	//
	//
	//
	//
	// type names?

};

/*
static const char* k_message_type_names[k_network_message_type_count] = { // update this whenever the enum updates
	"ping",
	"pong",
	"connect_request",
	"connect_refuse",
	"connect_establish",
	"connect_closed",
	"join_request",
	"peer_connect",
	"join_abort",
	"join_refuse",
	"leave_session",
	"leave_acknowledge",
	"session_disband",
	"session_boot",
	"host_decline",
	"peer_establish",
	"time_synchronize",
	"membership_update",
	"peer_properties",
	"delegate_leadership",
	"boot_machine",
	"player_add",
	"player_refuse",
	"player_remove",
	"player_properties",
	"parameters_update",
	"parameters_request",
	"view_establishment",
	"player_acknowledge",
	"synchronous_update",
	"synchronous_playback_control",
	"synchronous_actions",
	"synchronous_acknowledge",
	"synchronous_gamestate",
	"synchronous_client_ready",
	"game_results",
	"test"
};
*/

static const char* k_join_refuse_reason_strings[k_network_join_refuse_reason_count] = { // TODO - update this whenever the enum updates
	"no-reason-given",
	"tried-to-join-self",
	"could-not-connect",
	"join-timed-out",
	"not-found",
	"privacy-mode",
	"not-joinable",
	"session-full",
	"alpha-split-screen",
	"session-disbanded",
	"session-booted",
	"address-invalid",
	"address-failed",
	"too-many-observers",
	"aborted",
	"abort-ignored",
	"wrong-payload-type",
	"no-reservation",
	"in-matchmaking",
	"player-count-zero",
	"player-not-online-enabled",
	"player-add-pending",
	"player-add-failed",
	"host-time-out",
	"rejected-by-host",
	"peer-version-too-low",
	"host-version-too-low",
	"holding-in-queue",
	"film-in-progress",
	"campaign-in-progress",
	"user-content-not-allowed",
	"survival-in-progress", // interestingly enough this refuse reason is in h3 too
	"executable-type-mismatch"
};

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason);