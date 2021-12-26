#pragma once
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\delivery\network_channel.h"
#include "..\session\network_session_manager.h"

// verified against ms23, not ms29
enum e_network_join_refuse_reason : long // based off mcc h3 - TODO, verify whether these are correct for ms29, ms29's differs from mcc's slightly (check mcc odst's when it releases?)
{
	_network_join_refuse_reason_none, // verified
	_network_join_refuse_reason_tried_to_join_self,
	_network_join_refuse_reason_could_not_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_not_found,
	_network_join_refuse_reason_privacy_mode,
	_network_join_refuse_reason_not_joinable,
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
	_network_join_refuse_reason_holding_in_queue, // verified
	_network_join_refuse_reason_film_in_progress, // verified
	_network_join_refuse_reason_campaign_in_progress, // verified (verified in ms29 too)
	_network_join_refuse_reason_user_content_not_allowed,
	_network_join_refuse_reason_survival_in_progress, // verified
	_network_join_refuse_reason_executable_type_mismatch, // verified

	k_network_join_refuse_reason_count
};

class c_network_observer;
class c_network_message_handler
{
public:
	int handle_ping(s_transport_address const* outgoing_address, s_network_message_ping const* message);
	int handle_pong(s_transport_address const* outgoing_address, s_network_message_pong const* message);
	// broadcast_search (removed by saber)
	// broadcast_reply (removed by saber)
	int handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message);
	int handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message);
	int handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message);
	int handle_join_request(s_transport_address const* outgoing_address, s_network_message_join_request const* message);
	int handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message);
	int handle_join_abort(s_transport_address const* outgoing_address, s_network_message_join_abort const* message);
	int handle_join_refuse(s_transport_address const* outgoing_address, s_network_message_join_refuse const* message);
	int handle_leave_session(s_transport_address const* outgoing_address, s_network_message_leave_session const* message);
	int handle_leave_acknowledge(s_transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message);
	int handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message);
	int handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message);
	int handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message);
	int handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	int handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message);
	int handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message);
	int handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message);
	int handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message);
	int handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message);
	int handle_player_add(c_network_channel* channel, s_network_message_player_add const* message);
	int handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	int handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message);
	int handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	int handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message);
	int handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	int handle_view_establishment(c_network_channel* channel, s_network_message_view_establishment const* message);
	int handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message);
	
	// synchronous_update
	// synchronous_playback_control
	// synchronous_actions
	// synchronous_acknowledge
	// synchronous_gamestate
	// synchronous_client_ready
	// game_results
	// test
	void handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* message);
	void handle_out_of_band_message(s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message);

	c_network_message_gateway* get_message_gateway();

private:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_type_collection;
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
};

// hooks
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* message);
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unknown, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message);

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason reason);