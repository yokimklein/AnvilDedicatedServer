#pragma once
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\delivery\network_channel.h"
#include "..\session\network_session_manager.h"

class c_network_observer;
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
	int handle_host_decline(c_network_channel* channel, s_network_message_host_decline* message);
	int handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish* message);
	int handle_membership_update(c_network_channel* channel, s_network_message_membership_update* message);
	int handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties* message);
	int handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership* message);
	int handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine* message);
	int handle_player_add(c_network_channel* channel, s_network_message_player_add* message);
	int handle_time_synchronize(s_transport_address outgoing_address, s_network_message_time_synchronize* message);

	void handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message* message);
	int handle_out_of_band_message(s_transport_address transport_address, e_network_message_type message_type, long message_storage_size, s_network_message* message);

	c_network_message_gateway* get_message_gateway();

private:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_type_collection;
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
};