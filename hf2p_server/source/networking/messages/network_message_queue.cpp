#include "network_message_queue.h"

bool c_network_message_queue::has_channel_been_used()
{
	return this->m_channel_has_been_used;
}

void c_network_message_queue::send_message(e_network_message_type message_type, long message_size, s_network_message* message)
{
	DECLFUNC(0x16BB0, void, __thiscall, c_network_message_queue*, e_network_message_type, long, s_network_message*)(this, message_type, message_size, message);
}