#include "network_message_gateway.h"
#include "..\delivery\network_link.h"

c_network_link* c_network_message_gateway::get_network_link()
{
	return this->m_link;
}