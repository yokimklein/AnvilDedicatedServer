#pragma once
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\session\network_session_manager.h"
#include "..\session\network_observer.h"

class c_network_message_handler
{
public:
	int handle_ping(s_transport_address outgoing_address, s_network_message_ping* message);
	int handle_pong(s_transport_address outgoing_address, s_network_message_pong* message);
	//int handle_broadcast_search(s_transport_address outgoing_address, s_network_message_broadcast_search* message);
	//int handle_broadcast_reply(s_transport_address outgoing_address, s_network_message_broadcast_reply* message);
	int handle_connect_request(s_transport_address outgoing_address, s_network_message_connect_request* message);
	int handle_connect_refuse(c_network_channel* outgoing_address, s_network_message_connect_refuse* message);
	int handle_connect_establish(c_network_channel* outgoing_address, s_network_message_connect_establish* message);
	int handle_connect_closed(c_network_channel* outgoing_address, s_network_message_connect_closed* message);
	int handle_join_request(s_transport_address outgoing_address, s_network_message_join_request* message);
	int handle_peer_connect(s_transport_address outgoing_address, s_network_message_peer_connect* message);
	int handle_join_abort(s_transport_address outgoing_address, s_network_message_join_abort* message);
	int handle_join_refuse(s_transport_address outgoing_address, s_network_message_join_refuse* message);
	int handle_leave_session(s_transport_address outgoing_address, s_network_message_leave_session* message);
	int handle_leave_acknowledge(s_transport_address outgoing_address, s_network_message_leave_acknowledge* message);
	int handle_session_disband(s_transport_address outgoing_address, s_network_message_session_disband* message);
	int handle_session_boot(s_transport_address outgoing_address, s_network_message_session_boot* message);
	int handle_time_synchronize(s_transport_address outgoing_address, s_network_message_time_synchronize* message);
	void handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message_connect_establish* message);
	int handle_out_of_band_message(s_transport_address transport_address, e_network_message_type message_type, long message_storage_size, s_network_message* message);

	c_network_message_gateway* get_message_gateway();

private:
	bool m_initialized;
	long : 32;
	c_network_message_type_collection* m_message_type_collection;
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
};