#pragma once
#include <cstdint>
#include <windows.h>
#include "network_session_membership.h"
#include "network_session_parameters.h"
#include "..\delivery\network_channel.h"
#include "..\messages\network_message_handler.h"

enum e_network_session_class
{
	_network_session_class_offline,
	_network_session_class_online,
	_network_session_class_xbox_live,

	k_network_session_class_count
};

enum e_network_session_type
{
	_network_session_type_none,
	_network_session_type_squad,
	_network_session_type_group,

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
	char handle_join_request(s_transport_address const* address, s_network_message_join_request const* message);
	bool acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason);

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index;
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	e_network_session_state m_local_state;
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
	char data[64];

private:

};

