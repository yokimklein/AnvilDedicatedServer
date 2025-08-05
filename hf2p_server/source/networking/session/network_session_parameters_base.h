#pragma once
#include <cseries\cseries.h>
#include <simulation\simulation.h>

// TODO: these both may be inaccurate for ms29 - VERIFY!
const long k_network_session_parameters_update_payload_size = 0x20000;
const long k_network_session_parameters_request_payload_size = 0x20000;

// can only write if host and permission level matches requestee
enum e_parameter_change_request_access
{
	_change_request_access_default = 0,
	_change_request_access_leader,
	_change_request_access_peer,

	k_parameter_change_request_access_count,
	k_parameter_change_request_access_none = NONE
};

enum e_network_session_parameter_flags
{
	_network_session_parameter_valid_bit = 0,
	_network_session_parameter_desires_change_request_bit,

	k_network_session_parameter_flags_count
};

enum e_parameter_update_priority
{
	_parameter_update_priority_default = 0,
	_parameter_update_priority_high,
	k_parameter_update_priority_count,

	k_parameter_update_priority_none = NONE,
};

class c_network_session;
enum e_network_session_parameter_type;
class c_network_session_parameter_base
{
public:
	bool get_allowed() const;
	bool set_allowed() const;
	const char* get_set_denied_reason() const;
	long get_change_request_size() const;
	bool handle_change_request(void const* change_request, long change_request_size);

	// non original, however I don't see how else the accessibility could've worked here
	inline const char* get_parameter_type_description() const { return m_parameter_type_description; };

	// non original, original field was unused(?)
	// led to a bug where any client could edit any parameter and take over a m_session
	inline const e_parameter_change_request_access get_change_request_access() const { return m_change_request_access; };

protected:
	const char* get_session_description() const;
	void set_update_required();

	virtual void notify_set_update_required(void);
	virtual void notify_set_change_request(void);
	virtual void notify_update_payload_sent_to_peer(long);
	virtual void notify_change_request_payload_sent(void);
	virtual bool can_set_peer_no_longer_requires_update(long);
	virtual bool can_clear_change_request_desired(void);
	virtual bool parameter_data_ready_for_consumption(void) const;
	virtual void __thiscall get_update_payload(long unknown1, void* unknown2) const;
	virtual long __thiscall get_update_payload_size() const;
	virtual void __thiscall handle_update_payload(void const* unknown);
	virtual void __thiscall get_change_request_payload(long unknown1, long unknown2, void* unknown3) const;
	virtual long __thiscall get_change_request_payload_size() const;
	virtual bool __thiscall handle_change_request_payload(void const* change_request);
	virtual void __thiscall notify_reset_peer_state(long unknown);
	virtual void __thiscall notify_clear();
	virtual void __thiscall notify_transition_state_to_become_host();

private:
	//void set_peer_no_longer_requires_update(long peer_index);
	//void set_change_request_no_longer_desired(void);

protected:
	e_network_session_parameter_type m_parameter_type;
	char const* m_parameter_type_description;
	c_network_session* m_session;
	e_parameter_update_priority m_update_priority;
	e_parameter_change_request_access m_change_request_access;
	c_static_array<bool, k_network_maximum_machines_per_session> m_transmitted_peer_updates;
	dword_flags m_flags;
};
static_assert(sizeof(c_network_session_parameter_base) == 0x30);
