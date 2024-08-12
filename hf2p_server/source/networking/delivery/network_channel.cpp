#include "network_channel.h"
#include "assert.h"
#include <networking\session\network_observer.h>
#include <networking\session\network_session.h>
#include <text\unicode.h>

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
const char* c_network_channel::get_name()
{
	static c_static_string<k_network_channel_name_size> channel_description{};
	channel_description.set(this->get_short_name());

	s_transport_secure_identifier secure_identifier;
	s_transport_secure_address secure_address;
	if (transport_secure_identifier_retrieve(&this->m_remote_address, _transport_platform_pc, &secure_identifier, &secure_address))
	{
		char secure_address_string[0x100]{};
		transport_secure_address_to_string(&secure_address, secure_address_string, 0x100, false, false);
		channel_description.append_print("-%s-%s", secure_address_string, transport_secure_identifier_get_string(&secure_identifier));
	}
	else
	{
		if (transport_address_valid(&this->m_remote_address))
		{
			char address_str[0x100];
			transport_address_to_string(&this->m_remote_address, nullptr, address_str, 0x100, false, false);
			channel_description.append_print("-%s", address_str);
		}
		else
		{
			channel_description.append_print("-%s", "invalid");
		}
		channel_description.append_print("-%s", "insecure");
	}
	return channel_description.get_string();
}

// m_remote_host_name doesn't exist in release builds, but we can reconstruct it
// its set by c_network_observer::observer_channel_set_remote_host_name > c_network_channel::set_remote_host_name when the peer's name is updated
const char* c_network_channel::get_short_name()
{
	static c_static_string<16> channel_remote_name{};
	channel_remote_name.clear();

	// find current channel's owner type
	for (long owner_type = 0; owner_type < k_network_observer_owner_count; owner_type++)
	{
		// find current channel's observer index
		long observer_index = m_observer->observer_channel_find_by_network_channel((e_network_observer_owner)owner_type, this);
		if (observer_index != -1)
		{
			// find peer paired to this channel
			c_network_session_membership* membership = m_observer->m_owners[owner_type].owner->get_session_membership();
			for (long peer_index = membership->get_first_peer(); peer_index != -1; peer_index = membership->get_next_peer(peer_index))
			{
				if (observer_index == membership->get_observer_channel_index(peer_index))
				{
					// set matching peer name as the channel's short name
					c_static_wchar_string<16>* peer_name = &membership->get_peer(peer_index)->properties.peer_name;
					if (!peer_name->is_empty())
						return channel_remote_name.print("%ls", peer_name->get_string());
					else
						return channel_remote_name.print("%s", "unnamed_peer");
				}
			}
		}
	}
	return channel_remote_name.print("%s", "unknown_peer");
}

e_network_channel_state c_network_channel::get_state()
{
	return this->m_channel_state;
}

bool c_network_channel::get_remote_address(s_transport_address* remote_address)
{
	if (this->get_state() == _network_channel_state_none || this->get_state() == _network_channel_state_empty)
		return false;
	*remote_address = this->m_remote_address;
	return true;
}

void c_network_channel::close(e_network_channel_closure_reason reason)
{
	// TODO: this sometimes throws an exception? when 2 clients timeout
	DECLFUNC(0xC310, void, __thiscall, c_network_channel*, e_network_channel_closure_reason)(this, reason);
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
	DECLFUNC(0xBE20, void, __thiscall, c_network_channel*, s_transport_address const*, bool, long)(this, remote_address, initial_connection, channel_identifier);
}

void c_network_channel::send_connection_established(long remote_identifier)
{
	DECLFUNC(0xBF80, void, __thiscall, c_network_channel*, long)(this, remote_identifier);
}

// originally a switch-case statement in OG source
const char* c_network_channel::get_closure_reason_string(e_network_channel_closure_reason reason)
{
	if (VALID_INDEX(reason, k_network_channel_reason_count))
		return k_network_channel_reason_names[reason];
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

bool c_network_channel_simulation_interface::established()
{
	assert(m_initialized);
	return m_established;
}