#pragma once
#include "..\transport\transport_address.h"

enum e_network_message_type : long
{
	_network_message_type_ping,
	_network_message_type_pong,
	//_network_message_type_broadcast_search, // saber removed most of the matchmaking related sections of the networking
	//_network_message_type_broadcast_reply,
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
	//_network_message_type_distributed_game_results, // not sure if this one still exists in ms29
	_network_message_type_synchronous_client_ready,
	_network_message_type_game_results,
	_network_message_type_test,

	k_network_message_type_count
};

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
	_network_join_refuse_reason_executable_type_mismatch, // verified

	k_network_join_refuse_reason_count
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
	unsigned short id;
	short : 16;

	unsigned long timestamp;

	bool request_qos;
	char : 8;
	short : 16;
};

struct s_network_message_pong : s_network_message
{
	unsigned short id;
	short : 16;

	unsigned long timestamp;
	unsigned long request_qos;
};
/*
struct s_network_message_broadcast_search : s_network_message
{

};

struct s_network_message_broadcast_reply : s_network_message
{

};
*/
struct s_network_message_connect_request : s_network_message
{

};

struct s_network_message_connect_refuse : s_network_message
{

};

struct s_network_message_connect_establish : s_network_message
{

};

struct s_network_message_connect_closed : s_network_message
{

};

struct s_network_message_join_request : s_network_message
{
	unsigned short protocol_version;
	short : 16;

	long : 32;
	long : 32;
	s_transport_secure_identifier session_id;
};

struct s_network_message_peer_connect : s_network_message
{

};

struct s_network_message_join_abort : s_network_message
{

};

struct s_network_message_join_refuse : s_network_message
{
	s_transport_secure_identifier session_id;
	e_network_join_refuse_reason reason;
};

struct s_network_message_leave_session : s_network_message
{

};

struct s_network_message_leave_acknowledge : s_network_message
{
	s_transport_secure_identifier session_id;
};

struct s_network_message_session_disband : s_network_message
{

};

struct s_network_message_session_boot : s_network_message
{

};

struct s_network_message_host_decline : s_network_message
{

};

struct s_network_message_peer_establish : s_network_message
{

};

struct s_network_message_time_synchronize : s_network_message
{

};

struct s_network_message_membership_update : s_network_message
{

};

struct s_network_message_peer_properties : s_network_message
{

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

};

struct s_network_message_player_remove : s_network_message
{

};

struct s_network_message_player_properties : s_network_message
{

};

struct s_network_message_parameters_update : s_network_message
{

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

struct c_network_message_type
{
	bool __unknown0;
	const char* message_type_name;
	unsigned int flags;
	unsigned int message_size;
	unsigned int message_size_maximum;
	void* encode_function;
	void* decode_function;
	void* compare_function;
	void* dispose_function;
};

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