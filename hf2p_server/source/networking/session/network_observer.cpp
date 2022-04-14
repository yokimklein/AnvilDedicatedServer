#include "network_observer.h"
#include "..\..\dllmain.h"

char c_network_observer::handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message)
{
	typedef char(__thiscall* c_network_observer__handle_connect_request_ptr)(c_network_observer* observer, s_transport_address const* address, s_network_message_connect_request const* message);
	static auto c_network_observer__handle_connect_request = reinterpret_cast<c_network_observer__handle_connect_request_ptr>(module_base + 0x10E30);
	return c_network_observer__handle_connect_request(this, address, message);
}

// TEST THIS
void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner observer_owner, int observer_channel_index)
{
	typedef void(__thiscall* observer_channel_initiate_connection_call_ptr)(c_network_observer* observer, int observer_owner, int observer_channel_index);
	static auto observer_channel_initiate_connection_call = reinterpret_cast<observer_channel_initiate_connection_call_ptr>(module_base + 0xF970);
	return observer_channel_initiate_connection_call(this, observer_owner, observer_channel_index);
}