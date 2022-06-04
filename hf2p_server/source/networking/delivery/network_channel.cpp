#include "network_channel.h"
#include "..\..\dllmain.h"
#include "..\messages\network_message_type_collection.h"
#include "..\transport\transport_address.h"

bool c_network_channel::connected()
{
	return this->get_state() == _network_channel_state_connected;
}

char* c_network_channel::get_name()
{
	return 0; // TODO once the field is mapped out
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

void c_network_channel::close(e_network_channel_closure_reason reason)
{
	typedef void(__thiscall* network_channel__close_ptr)(c_network_channel* channel, e_network_channel_closure_reason reason);
	auto network_channel__close = reinterpret_cast<network_channel__close_ptr>(module_base + 0xC310);
	network_channel__close(this, reason);
}

bool c_network_channel::closed()
{
	return this->get_state() <= _network_channel_state_closed;
}

bool c_network_channel::established()
{
	return this->get_state() >= _network_channel_state_established;
}

long c_network_channel::get_identifier()
{
	return this->m_identifier;
}

long c_network_channel::get_remote_identifier()
{
	return this->m_remote_identifier;
}

void c_network_channel::open(s_transport_address const* remote_address, bool unknown, long channel_identifier)
{
	void(__thiscall * open)(c_network_channel* thisptr, s_transport_address const* remote_address, bool unknown, long channel_identifier) = reinterpret_cast<decltype(open)>(module_base + 0xBE20);
	open(this, remote_address, unknown, channel_identifier);
}

void c_network_channel::send_connection_established(long remote_identifier)
{
	void(__thiscall * send_connection_established)(c_network_channel* thisptr, long remote_identifier) = reinterpret_cast<decltype(send_connection_established)>(module_base + 0xBF80);
	send_connection_established(this, remote_identifier);
}