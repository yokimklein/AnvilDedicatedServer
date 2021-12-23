#include "network_message_gateway.h"
#include "..\delivery\network_link.h"

c_network_link* c_network_message_gateway::get_network_link()
{
	return this->m_link;
}

bool c_network_message_gateway::send_message_directed(c_network_message_gateway* network_message_gateway, s_transport_address outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message* message)
{
	typedef int(__thiscall* c_network_message_gateway__send_message_directed_ptr)(c_network_message_gateway* network_message_gateway, s_transport_address outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message* message);
	static auto c_network_message_gateway__send_message_directed = reinterpret_cast<c_network_message_gateway__send_message_directed_ptr>(module_base + 0x232C0);
	c_network_message_gateway__send_message_directed(network_message_gateway, outgoing_address, message_type, message_storage_size, message);

	//temp
	return true;
}