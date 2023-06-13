#include "network_message_gateway.h"
#include "..\delivery\network_link.h"

c_network_link* c_network_message_gateway::get_network_link()
{
	return this->m_link;
}

bool c_network_message_gateway::send_message_directed(s_transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
	typedef bool(__thiscall* c_network_message_gateway__send_message_directed_ptr)(c_network_message_gateway* network_message_gateway, s_transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message const* message);
	static auto c_network_message_gateway__send_message_directed = reinterpret_cast<c_network_message_gateway__send_message_directed_ptr>(base_address(0x232C0));
	return c_network_message_gateway__send_message_directed(this, outgoing_address, message_type, message_storage_size, message);
}