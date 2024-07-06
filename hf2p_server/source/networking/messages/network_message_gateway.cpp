#include "network_message_gateway.h"
#include <networking\delivery\network_link.h>

c_network_link* c_network_message_gateway::get_network_link()
{
	return this->m_link;
}

bool c_network_message_gateway::send_message_directed(s_transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
	return DECLFUNC(0x232C0, bool, __thiscall, c_network_message_gateway*, s_transport_address const*, e_network_message_type, long, s_network_message const*)(this, outgoing_address, message_type, message_storage_size, message);
}