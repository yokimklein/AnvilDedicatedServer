#include "network_message_queue.h"

bool c_network_message_queue::has_channel_been_used()
{
	return this->m_channel_has_been_used;
}

void c_network_message_queue::send_message(e_network_message_type message_type, long message_size, s_network_message* message)
{
	void(__thiscall* send_message)(c_network_message_queue* thisptr, e_network_message_type message_type, long message_size, s_network_message* message) = reinterpret_cast<decltype(send_message)>(base_address(0x16BB0));
	send_message(this, message_type, message_size, message);
}