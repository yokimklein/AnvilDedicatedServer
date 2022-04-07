#include "network_channel.h"
#include "..\..\dllmain.h"
#include "..\messages\network_message_type_collection.h"
#include "..\transport\transport_address.h"

bool c_network_channel::connected() // TODO - needs testing
{
	return this->get_state() == _network_channel_state_connected;
}

char* c_network_channel::get_name() // TODO - non crucial, used for logging only
{
	return 0;
}

e_network_channel_state c_network_channel::get_state()
{
	return this->m_state;
}

//char* c_network_channel::get_message_type_name(e_network_message_type message_type) // this belongs to c_network_message_type_collection?
//{
//	return 0;
//}

bool c_network_channel::get_remote_address(s_transport_address* remote_address) // untested
{
	if (this->get_state() != _network_channel_state_none || this->get_state() == _network_channel_state_empty)
		return false;
	remote_address = &this->m_remote_address;
	return true;
}

long c_network_channel::close(e_network_channel_closure_reason reason)
{
	typedef long(__fastcall* network_channel__close_ptr)(c_network_channel* channel, e_network_channel_closure_reason reason);
	auto network_channel__close = reinterpret_cast<network_channel__close_ptr>(module_base + 0xC310);
	return network_channel__close(this, reason);
}