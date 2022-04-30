#pragma once
#include <cstdint>
#include <windows.h>
#include "..\..\simulation\simulation.h"

struct c_network_session_parameter_base_vtbl;

class c_network_session;
class c_network_session_parameter_base
{
public:
	bool get_allowed();
	const char* get_session_description();
	bool set_allowed();

	c_network_session_parameter_base_vtbl* vftable;
	uint32_t m_type;
	const char* m_name;
	c_network_session* m_session;
	long m_update_priority;
	long m_writeable;
	byte m_peers_updated[k_network_maximum_machines_per_session];
	byte : 8;
	byte : 8;
	byte : 8;
	byte m_state_flags;
};
static_assert(sizeof(c_network_session_parameter_base) == 0x30);

struct c_network_session_parameter_base_vtbl
{
	void(__thiscall* notify_set_update_required)(c_network_session_parameter_base*);
	void(__thiscall* notify_set_change_request)(c_network_session_parameter_base*);
	void(__thiscall* notify_update_payload_sent_to_peer)(c_network_session_parameter_base*, long peer_index);
	void(__thiscall* notify_change_request_payload_sent)(c_network_session_parameter_base*);
	bool(__thiscall* can_set_peer_no_longer_requires_update)(c_network_session_parameter_base*, long peer_index);
	bool(__thiscall* can_clear_change_request_desired)(c_network_session_parameter_base*);
	bool(__thiscall* parameter_data_ready_for_consumption)(c_network_session_parameter_base*);
	void(__thiscall* get_update_payload)(c_network_session_parameter_base*, long, void*);
	long(__thiscall* get_update_payload_size)(c_network_session_parameter_base*);
	void(__thiscall* handle_update_payload)(c_network_session_parameter_base*, const void*);
	void(__thiscall* get_change_request_payload)(c_network_session_parameter_base*, long, long, void*);
	long(__thiscall* get_change_request_payload_size)(c_network_session_parameter_base*);
	void(__thiscall* handle_change_request_payload)(c_network_session_parameter_base*, const void*);
	void(__thiscall* notify_reset_peer_state)(c_network_session_parameter_base*, long);
	void(__thiscall* notify_clear)(c_network_session_parameter_base*);
	void(__thiscall* notify_transition_state_to_become_host)(c_network_session_parameter_base*);
};