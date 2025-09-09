#include "network_channel.h"
#include <networking\session\network_observer.h>
#include <networking\session\network_session.h>
#include <networking\messages\network_messages_connect.h>
#include <networking\messages\network_message_gateway.h>
#include <text\unicode.h>
#include <cseries\cseries_events.h>

char const* k_network_channel_reason_names[] =
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
	"observer-reset-security",
	"test"
};
static_assert(NUMBEROF(k_network_channel_reason_names) == k_network_channel_reason_count);

bool c_network_channel::connected() const
{
	return get_state() == _network_channel_state_connected;
}

// m_channel_description doesn't exist in release builds, which this normally returns, but we can recreate it based on c_network_channel::rebuild_channel_description
// name = short_name + "-<secure_address>-<secure_identifier>"
// name = short_name + "-<secure_address>-insecure"
// name = short_name + "-invalid-insecure"
const char* c_network_channel::get_name() const
{
	static c_static_string<k_network_channel_name_size> channel_description{};
	channel_description.set(get_short_name());

	s_transport_secure_identifier secure_identifier;
	s_transport_secure_address secure_address;
	if (transport_secure_identifier_retrieve(&m_remote_address, _transport_platform_windows, &secure_identifier, &secure_address))
	{
		char secure_address_string[0x100]{};
		transport_secure_address_to_string(&secure_address, secure_address_string, 0x100, false, false);
		channel_description.append_print("-%s-%s", secure_address_string, transport_secure_identifier_get_string(&secure_identifier));
	}
	else
	{
		if (transport_address_valid(&m_remote_address))
		{
			char address_str[0x100];
			transport_address_to_string(&m_remote_address, nullptr, address_str, 0x100, false, false);
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
const char* c_network_channel::get_short_name() const
{
	static c_static_string<16> channel_remote_name{};
	channel_remote_name.clear();

	// find current channel's owner type
	for (long owner_type = 0; owner_type < k_network_observer_owner_count; owner_type++)
	{
		// find current channel's observer index
		long observer_index = m_observer->observer_channel_find_by_network_channel((e_network_observer_owner)owner_type, this);

		if (observer_index == NONE)
		{
			continue;
		}

		// find peer paired to this channel
		const c_network_session_membership* membership = m_observer->m_owners[owner_type].owner->get_session_membership();
		for (long peer_index = membership->get_first_peer(); peer_index != NONE; peer_index = membership->get_next_peer(peer_index))
		{
			if (observer_index != membership->get_observer_channel_index(peer_index))
				continue;

			// set matching peer name as the channel's short name
			const s_network_session_peer_properties* peer_properties = &membership->get_peer(peer_index)->properties;

			if (peer_properties->peer_name.is_empty())
			{
				channel_remote_name.set("unnamed_peer");
			}
			else
			{
				// $TODO: set_wchar is causing linker errors, need to fix this
				//channel_remote_name.set_wchar(peer_properties->peer_name.get_string());
				wchar_string_to_ascii_string(peer_properties->peer_name.get_string(), channel_remote_name.get_buffer(), channel_remote_name.element_count, NULL);
			}
			return channel_remote_name.get_string();
		}
	}
	channel_remote_name.set("unknown_peer");
	return channel_remote_name.get_string();
}

e_network_channel_state c_network_channel::get_state() const
{
	return m_channel_state;
}

bool c_network_channel::get_remote_address(transport_address* address) const
{
	ASSERT(address);
	if (get_state() > _network_channel_state_empty)
	{
		*address = m_remote_address;
		return true;
	}
	return false;
}

void c_network_channel::close(e_network_channel_closure_reason closure_reason)
{
	ASSERT(closure_reason > _network_channel_reason_none && closure_reason < k_network_channel_reason_count);
	ASSERT(allocated());
	ASSERT(!closed());

	event(_event_message, "networking:channel: Closing channel. Reason %s.", get_closure_reason_string(closure_reason));

	if (connected() && closure_reason != _network_channel_reason_connect_reinitiated)
	{
		s_network_message_connect_closed connect_closed =
		{
			.remote_channel_identifier = m_remote_channel_identifier,
			.channel_identifier = m_channel_identifier,
			.closure_reason = closure_reason
		};

		m_message_gateway->send_message_directed(&m_remote_address, _network_message_connect_closed, sizeof(s_network_message_connect_closed), &connect_closed);
	}

	if (m_simulation_interface)
	{
		m_simulation_interface->notify_closed();
	}

	ASSERT(m_simulation_interface == NULL);

	m_channel_state = _network_channel_state_closed;
	m_closure_reason = closure_reason;
	m_channel_closure_address = m_remote_address;
	m_channel_identifier = NONE;
	m_remote_channel_identifier = NONE;
}

bool c_network_channel::closed() const
{
	return get_state() <= _network_channel_state_closed;
}

bool c_network_channel::established() const
{
	return get_state() >= _network_channel_state_established;
}

ulong c_network_channel::get_identifier() const
{
	return m_channel_identifier;
}

ulong c_network_channel::get_remote_identifier() const
{
	return m_remote_channel_identifier;
}

void c_network_channel::open(transport_address const* remote_address, bool send_connect_packets, long channel_identifier)
{
	DECLFUNC(0xBE20, void, __thiscall, c_network_channel*, transport_address const*, bool, long)(this, remote_address, send_connect_packets, channel_identifier);
}

void c_network_channel::send_connection_established(long remote_identifier)
{
	DECLFUNC(0xBF80, void, __thiscall, c_network_channel*, long)(this, remote_identifier);
}

// originally a switch-case statement in OG source
char const* c_network_channel::get_closure_reason_string(e_network_channel_closure_reason reason)
{
	if (VALID_INDEX(reason, k_network_channel_reason_count))
		return k_network_channel_reason_names[reason];

	return "<unknown>";
}

void c_network_channel::send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload)
{
	m_message_queue.send_message(message_type, raw_message_size, raw_message_payload);
}

bool c_network_channel::allocated() const
{
	return get_state() != _network_channel_state_none;
}

bool c_network_channel_simulation_interface::established() const
{
	ASSERT(m_initialized);
	return m_established;
}