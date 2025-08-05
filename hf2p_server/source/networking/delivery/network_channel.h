#pragma once
#include <cseries\cseries.h>
#include <networking\delivery\network_connection.h>
#include <networking\messages\network_message_queue.h>
#include <networking\transport\transport_address.h>

constexpr long k_network_channel_maximum_clients = 6;
constexpr long k_network_channel_maximum_base_clients = 3;
constexpr long k_network_channel_maximum_simulation_clients = 4;
constexpr long k_network_channel_name_size = 0x100;

enum e_network_channel_client_flags
{
	k_network_channel_client_flags_count
};

enum e_network_channel_activity
{
	// activity_types

	k_network_channel_activity_count = 6
};

enum e_network_channel_closure_reason
{
	_network_channel_reason_none,
	_network_channel_reason_link_destroyed,
	_network_channel_reason_link_refused_listen,
	_network_channel_reason_channel_deleted,
	_network_channel_reason_connect_timeout,
	_network_channel_reason_connect_refused,
	_network_channel_reason_connect_reinitiated,
	_network_channel_reason_establish_timeout,
	_network_channel_reason_address_change,
	_network_channel_reason_destination_unreachable,
	_network_channel_reason_remote_closure,
	_network_channel_reason_connection_overflow,
	_network_channel_reason_message_overflow,
	_network_channel_reason_security_lost,
	_network_channel_reason_observer_released,
	_network_channel_reason_observer_refused,
	_network_channel_reason_observer_timeout,
	_network_channel_reason_observer_reset,
	_network_channel_reason_observer_reset_security,
	_network_channel_reason_test,

	k_network_channel_reason_count
};

enum e_network_channel_state
{
	_network_channel_state_none,
	_network_channel_state_empty,
	_network_channel_state_closed,
	_network_channel_state_connecting,
	_network_channel_state_established,
	_network_channel_state_connected,

	k_network_channel_state_count
};

enum e_observer_state
{
	_observer_state_none = 0,
	_observer_state_dead,
	_observer_state_idle,
	_observer_state_securing,
	_observer_state_waiting,
	_observer_state_ready,
	_observer_state_connecting,
	_observer_state_connected,
	_observer_state_reconnecting,
	_observer_state_disconnected,

	k_observer_state_count
};

struct s_client_iterator
{
	dword_flags required_client_flags;
	long client_designator;
	long client_index;
	long : 32;
	long : 32;
};

struct s_network_channel_client_info
{
	dword_flags flags; // e_network_channel_client_flags
	c_network_channel_client* client;
};

class c_network_channel_simulation_interface
{
public:
	bool established() const;

	void __cdecl notify_closed() const
	{
		ASSERT(m_initialized);
		ASSERT(m_simulation_context != NULL);
		ASSERT(m_simulation_closure_callback != NULL);

		m_simulation_closure_callback(m_simulation_context);
	}

protected:
	bool m_initialized;
	void* m_simulation_context;
	void(__cdecl* m_simulation_closure_callback)(void*);
	long m_client_count;
	s_network_channel_client_info m_clients[4];
	bool m_simulation_is_authority;
	bool m_established;
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

struct c_network_channel_simulation_gatekeeper : public c_network_channel_client
{
	bool m_write_simulation_data_available;
	bool m_read_simulation_data_expected;
};
static_assert(sizeof(c_network_channel_simulation_gatekeeper) == 0x8);

class c_network_link;
class c_network_observer;
class c_network_message_handler;
class c_network_message_gateway;
class c_network_message_type_collection;
struct s_channel_configuration;
class c_network_channel
{
public:
	struct s_activity_timer
	{
		dword timestamp;
	};
	static_assert(sizeof(s_activity_timer) == 0x4);

	bool connected()  const;
	const char* get_name() const;
	const char* get_short_name() const;
	e_network_channel_state get_state() const;
	bool get_remote_address(transport_address* address) const;
	bool allocated() const;
	// allocate()
	bool closed() const;
	void close(e_network_channel_closure_reason closure_reason);
	// simulation_can_be_established()
	// is_voice_only()
	// is_receive_only()
	// is_transmit_only()
	bool established() const;
	ulong get_identifier() const;
	ulong get_remote_identifier() const;
	void open(transport_address const* remote_address, bool send_connect_packets, long channel_identifier);
	void send_connection_established(long remote_identifier);
	static char const* get_closure_reason_string(e_network_channel_closure_reason reason);
	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);

//protected:
	c_network_link* m_link;
	c_network_observer* m_observer;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	s_channel_configuration* m_configuration; // g_network_configuration?
	c_network_message_type_collection* m_type_collection;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	long m_client_count;
	s_network_channel_client_info m_clients[k_network_channel_maximum_base_clients];
	c_network_channel_simulation_interface* m_simulation_interface;
	dword_flags m_channel_flags;
	ulong m_channel_identifier;
	ulong m_remote_channel_identifier;
	c_enum<e_network_channel_state, long, _network_channel_state_none, k_network_channel_state_count> m_channel_state;
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> m_closure_reason;
	transport_address m_channel_closure_address;
	transport_address m_remote_address;
	bool m_send_connect_packets;
	byte : 8;
	byte : 8;
	byte : 8;
	dword m_first_connect_packet_timestamp;
	dword m_next_connect_packet_timestamp;
	long m_sent_connect_packet_count;
	dword m_established_timestamp;
	dword m_connected_timestamp;
	s_activity_timer m_activity_times[k_network_channel_activity_count];
	bool m_destination_unreachable;

};
static_assert(sizeof(c_network_channel) == 0xA74);
static_assert(0xA08 == OFFSETOF(c_network_channel, m_channel_identifier));
static_assert(0xA0C == OFFSETOF(c_network_channel, m_remote_channel_identifier));