#pragma once
#include <cstdint>
#include <windows.h>
#include "network_observer.h"
#include "network_session_manager.h"
#include "network_session_membership.h"
#include "network_session_parameters.h"
#include "..\messages\network_message_gateway.h"

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

struct c_network_channel_owner
{
	struct c_network_channel_owner_vtbl* __vftable /*VFT*/;
};

class c_network_session : c_network_channel_owner
{
public:
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index;
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	uint32_t m_local_state;
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

