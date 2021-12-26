#include "network_observer.h"
#include "..\..\dllmain.h"

char c_network_observer::handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message)
{
	typedef char(__thiscall* c_network_observer__handle_connect_request_ptr)(c_network_observer* observer, s_transport_address const* address, s_network_message_connect_request const* message);
	static auto c_network_observer__handle_connect_request = reinterpret_cast<c_network_observer__handle_connect_request_ptr>(module_base + 0x10E30);
	return c_network_observer__handle_connect_request(this, address, message);
}