#include "network_message_queue.h"

bool c_network_message_queue::has_channel_been_used() const
{
	return m_second_fragment_sent;
}

void c_network_message_queue::send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload)
{
	INVOKE_CLASS_MEMBER(0x16BB0, c_network_message_queue, send_message, message_type, raw_message_size, raw_message_payload);
}
