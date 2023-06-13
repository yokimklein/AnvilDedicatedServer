#include "network_channel.h"
#include "assert.h"
#include "..\session\network_observer.h"
#include "..\session\network_session.h"
#include "..\..\text\unicode.h"

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

// m_channel_description doesn't exist in release builds, which this normally returns, but we can recreate it based on c_network_channel::rebuild_channel_description
// name = short_name + "-<secure_address>-<secure_identifier>"
// name = short_name + "-<secure_address>-insecure"
// name = short_name + "-invalid-insecure"
// name = <secure-address> / "unknown"
char* c_network_channel::get_name()
{
	static char channel_description[k_network_channel_name_size] = "unknown";

	if (this->established())
	{
		// this->get_short_name() crashes? TODO
		long short_name_length = strlen(this->get_short_name());
		strncpy_debug(channel_description, k_network_channel_name_size, this->get_short_name(), short_name_length);
		channel_description[short_name_length] = '-';
		short_name_length++;

		s_transport_secure_identifier secure_identifier;
		s_transport_secure_address secure_address;
		if (transport_secure_identifier_retrieve(&this->m_remote_address, _transport_platform_pc, &secure_identifier, &secure_address))
		{
			transport_secure_address_to_string(&secure_address, &channel_description[short_name_length], k_network_channel_name_size - short_name_length, false, false);
			csnzappendf(channel_description, k_network_channel_name_size, "-%s", transport_secure_identifier_get_string(&secure_identifier));
		}
		else
		{
			if (transport_address_valid(&this->m_remote_address))
				transport_address_to_string(&this->m_remote_address, nullptr, &channel_description[short_name_length], (short)(k_network_channel_name_size - short_name_length), false, false);
			else
				strncpy_debug(&channel_description[short_name_length], k_network_channel_name_size - short_name_length, "invalid", k_network_channel_name_size - short_name_length);
			csnzappendf(channel_description, k_network_channel_name_size, "-insecure");
		}
		assert(strlen(channel_description) < k_network_channel_name_size);
	}
	else
	{
		auto observer = this->m_observer->find_observer_by_channel(this);
		if (observer != nullptr)
			transport_secure_address_to_string(&observer->secure_address, channel_description, k_network_channel_name_size, false, false);
	}
	return channel_description;
}

// TODO TEST THIS
// m_remote_host_name doesn't exist in release builds, but like the above we can reconstruct it
// its set by c_network_observer::observer_channel_set_remote_host_name > c_network_channel::set_remote_host_name when the peer's name is updated
char* c_network_channel::get_short_name()
{
	static char channel_remote_name[16]{};

	e_network_observer_owner observer_owner = m_observer->m_session->observer_owner();
	long observer_index = m_observer->observer_channel_find_by_network_channel(observer_owner, this);
	if (observer_index != -1)
	{
		long peer_index = m_observer->m_session->get_session_membership()->get_peer_from_observer_channel(observer_index);
		if (peer_index != -1)
		{
			auto peer = m_observer->m_session->get_session_membership()->get_peer(peer_index);
			wchar_string_to_ascii_string(peer->properties.peer_name, channel_remote_name, 16, 0);
			return channel_remote_name;
		}
	}
	
	return 0;
}

e_network_channel_state c_network_channel::get_state()
{
	return this->m_channel_state;
}

//char* c_network_channel::get_message_type_name(e_network_message_type message_type) // this belongs to c_network_message_type_collection?
//{
//	return 0;
//}

bool c_network_channel::get_remote_address(s_transport_address* remote_address)
{
	if (this->get_state() == _network_channel_state_none || this->get_state() == _network_channel_state_empty)
		return false;
	*remote_address = this->m_remote_address;
	return true;
}

void c_network_channel::close(e_network_channel_closure_reason reason)
{
	void(__thiscall* close)(c_network_channel* channel, e_network_channel_closure_reason reason) = reinterpret_cast<decltype(close)>(base_address(0xC310));
	close(this, reason); // this sometimes throws an exception? when 2 clients timeout
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
	void(__thiscall* open)(c_network_channel* thisptr, s_transport_address const* remote_address, bool initial_connection, long channel_identifier) = reinterpret_cast<decltype(open)>(base_address(0xBE20));
	open(this, remote_address, initial_connection, channel_identifier);
}

void c_network_channel::send_connection_established(long remote_identifier)
{
	void(__thiscall* send_connection_established)(c_network_channel* thisptr, long remote_identifier) = reinterpret_cast<decltype(send_connection_established)>(base_address(0xBF80));
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

bool c_network_channel::allocated()
{
	return this->get_state() != _network_channel_state_none;
}