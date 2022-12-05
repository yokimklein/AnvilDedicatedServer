#include "network_channel.h"
#include "..\..\dllmain.h"

char const* k_network_channel_reason_names[k_network_channel_reason_count] =
{
	"no-reason-given",
	"link-destroyed",
	"link-refused-listen",
	"channel-deleted",
	"connect-timeout",
	"connect-refused",
	"connect-reinitiate",
	"establish-timeout",
	"address-change",
	"destination-unreachable",
	"remote-closure",
	"connection-overflow",
	"message-overflow",
	"security-lost",
	"observer-released",
	"observer-refused",
	"observer-timeout",
	"observer-reset",
	"observer-reset-security"
};

bool c_network_channel::connected()
{
	return this->get_state() == _network_channel_state_connected;
}

char* c_network_channel::get_name()
{
	return 0; // TODO - do release builds even contain time statistics and names anymore?
}

e_network_channel_state c_network_channel::get_state()
{
	return this->m_channel_state;
}

//char* c_network_channel::get_message_type_name(e_network_message_type message_type) // this belongs to c_network_message_type_collection?
//{
//	return 0;
//}

bool c_network_channel::get_remote_address(s_transport_address* remote_address) // untested
{
	if (this->get_state() == _network_channel_state_none || this->get_state() == _network_channel_state_empty)
		return false;
	*remote_address = this->m_remote_address;
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
	return this->m_local_identifier;
}

long c_network_channel::get_remote_identifier()
{
	return this->m_remote_identifier;
}

void c_network_channel::open(s_transport_address const* remote_address, bool initial_connection, long channel_identifier)
{
	void(__thiscall * open)(c_network_channel* thisptr, s_transport_address const* remote_address, bool initial_connection, long channel_identifier) = reinterpret_cast<decltype(open)>(module_base + 0xBE20);
	open(this, remote_address, initial_connection, channel_identifier);
}

void c_network_channel::send_connection_established(long remote_identifier)
{
	void(__thiscall * send_connection_established)(c_network_channel* thisptr, long remote_identifier) = reinterpret_cast<decltype(send_connection_established)>(module_base + 0xBF80);
	send_connection_established(this, remote_identifier);
}

// originally a switch-case statement in OG source
const char* c_network_channel::get_closure_reason_string(e_network_channel_closure_reason reason)
{
	if (reason <= k_network_channel_reason_count)
		return k_network_channel_reason_names[reason];
	else
		return "<unknown>";
}

void c_network_channel::send_message(e_network_message_type message_type, long message_size, s_network_message* message)
{
	this->m_message_queue.send_message(message_type, message_size, message);
}