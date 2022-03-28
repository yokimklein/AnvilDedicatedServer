#pragma once
#include <cstdint>
#include <windows.h>
#include "network_session_membership.h"
#include "network_session_parameters.h"
#include "..\messages\network_message_type_collection.h"
#include "..\delivery\network_channel.h"
#include "..\..\dllmain.h"

enum e_network_session_type
{
	_network_session_type_none,
	_network_session_type_group,
	_network_session_type_squad,

	k_network_session_type_count
};

enum e_network_session_state : uint32_t
{
	_network_session_state_none,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,
	_network_session_state_host_handoff,
	_network_session_state_host_reestablish,
	_network_session_state_election,

	k_network_session_state_count
};

class c_network_message_gateway;
class c_network_observer;
class c_network_session_manager;
class c_network_session : c_network_channel_owner
{
public:
	bool handle_join_request(s_transport_address const* address, s_network_message_join_request const* message);
	bool acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason);
	bool handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message);
	bool handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message);
	bool handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message);
	bool handle_host_decline (c_network_channel* channel, s_network_message_host_decline const* message);
	bool handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message);
	bool channel_is_authoritative(c_network_channel* channel);
	bool handle_membership_update(s_network_message_membership_update const* message);
	bool handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	bool handle_parameters_update(s_network_message_parameters_update const* message);

	e_network_join_refuse_reason get_closure_reason();

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index;
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	e_network_session_state m_local_state; // this should be + 0x1ABDA8
	long : 32;
	char m_local_state_data[648];
	uint32_t m_connection_identifier;
	uint32_t m_time_synchronization_end_time;
	uint32_t m_time_synchronization_start_time;
	bool m_time_exists;
	uint32_t m_time;
	long : 32;
	int m_managed_session_index;
	int m_join_refuse_reason;
	uint64_t m_host_join_nonce;
	long : 32;
	uint32_t m_disconnection_policy;
	s_player_identifier m_player_add_single_player_identifier;
	s_transport_secure_address m_player_add_secure_address;
	long m_player_add_peer_index;
	s_player_identifier m_player_add_player_identifier;
	uint32_t m_player_add_join_refuse_reason;
	uint32_t m_player_add_time;

private:

};
//static_assert(sizeof(c_network_session_membership) == 0x1AC098);

void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request);
bool network_join_process_joins_from_queue();