#pragma once
#include "network_message_type_collection.h"

class c_network_channel;
class c_network_link;
class c_network_message_type_collection;
class c_network_message_gateway;
class c_network_observer;
class c_network_session_manager;

//struct s_network_message_ping;
//struct s_network_message_pong;
//struct s_network_message_connect_refuse;
//struct s_network_message_connect_establish;
//struct s_network_message_connect_closed;
//struct s_network_message_join_request;
//struct s_network_message_peer_connect;
//struct s_network_message_join_abort;
//struct s_network_message_join_refuse;
//struct s_network_message_leave_session;
//struct s_network_message_leave_acknowledge;
//struct s_network_message_session_disband;
//struct s_network_message_session_boot;
//struct s_network_message_host_decline;
//struct s_network_message_peer_establish;
//struct s_network_message_time_synchronize;
//struct s_network_message_membership_update;
//struct s_network_message_peer_properties;
//struct s_network_message_delegate_leadership;
//struct s_network_message_boot_machine;
//struct s_network_message_player_add;
//struct s_network_message_player_refuse;
//struct s_network_message_player_remove;
//struct s_network_message_player_properties;
//struct s_network_message_parameters_update;
//struct s_network_message_parameters_request;
//struct s_network_message_view_establishment;
//struct s_network_message_player_acknowledge;

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