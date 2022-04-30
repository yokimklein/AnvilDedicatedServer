#include "network_observer.h"
#include "..\..\dllmain.h"

void c_network_observer::handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message)
{
	typedef void(__thiscall* c_network_observer__handle_connect_request_ptr)(c_network_observer* observer, s_transport_address const* address, s_network_message_connect_request const* message);
	static auto c_network_observer__handle_connect_request = reinterpret_cast<c_network_observer__handle_connect_request_ptr>(module_base + 0x10E30);
	c_network_observer__handle_connect_request(this, address, message);
}

void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner observer_owner, int observer_channel_index)
{
	typedef void(__thiscall* observer_channel_initiate_connection_call_ptr)(c_network_observer* observer, int observer_owner, int observer_channel_index);
	static auto observer_channel_initiate_connection_call = reinterpret_cast<observer_channel_initiate_connection_call_ptr>(module_base + 0xF970);
	return observer_channel_initiate_connection_call(this, observer_owner, observer_channel_index);
}

// FUNC TODO
const char* c_network_observer::get_name(long observer_index)
{
	// get channel from observer index
	// call c_network_channel::get_name
	const char* name = "<channel-name>";
	return name;
}

bool c_network_observer::observer_channel_dead(e_network_observer_owner owner_type, long observer_index)
{
	return this->get_observer(owner_type, observer_index)->state == _observer_state_dead;
}

c_network_observer::s_channel_observer* c_network_observer::get_observer(e_network_observer_owner owner_type, long observer_index)
{
	// TODO ASSERTS
	return &this->m_channel_observers[observer_index];
}