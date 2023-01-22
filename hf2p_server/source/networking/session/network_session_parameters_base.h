#pragma once
#include "..\..\cseries\cseries.h"
#include "..\..\simulation\simulation.h"

// these both may be inaccurate for ms29, TODO - VERIFY!
const long k_network_session_parameters_update_payload_size = 0x20000;
const long k_network_session_parameters_request_payload_size = 0x20000;

class c_network_session;

class c_network_session_parameter_base
{
public:
	virtual void __thiscall notify_set_update_required();
	virtual void __thiscall notify_set_change_request();
	virtual void __thiscall notify_update_payload_sent_to_peer(long peer_index);
	virtual void __thiscall notify_change_request_payload_sent();
	virtual bool __thiscall can_set_peer_no_longer_requires_update(long peer_index);
	virtual bool __thiscall can_clear_change_request_desired();
	virtual bool __thiscall parameter_data_ready_for_consumption();
	virtual void __thiscall get_update_payload(long unknown1, void* unknown2);
	virtual long __thiscall get_update_payload_size();
	virtual void __thiscall handle_update_payload(const void* unknown);
	virtual void __thiscall get_change_request_payload(long unknown1, long unknown2, void* unknown3);
	virtual long __thiscall get_change_request_payload_size();
	virtual bool __thiscall handle_change_request_payload(const void* change_request);
	virtual void __thiscall notify_reset_peer_state(long unknown);
	virtual void __thiscall notify_clear();
	virtual void __thiscall notify_transition_state_to_become_host();

	bool get_allowed();
	const char* get_session_description();
	bool set_allowed();
	long get_change_request_size();
	bool handle_change_request(void const* change_request, long change_request_size);
	void set_update_required();
	const char* get_set_denied_reason();

	ulong m_type;
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