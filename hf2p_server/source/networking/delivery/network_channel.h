#pragma once
#include "network_connection.h"
#include "..\transport\transport_address.h"
#include "..\messages\network_message_queue.h"
#include "..\messages\network_message_type_collection.h"

constexpr long k_network_channel_maximum_clients = 6;
constexpr long k_network_channel_maximum_base_clients = 3;
constexpr long k_network_channel_maximum_simulation_clients = 4;

enum e_network_channel_client_flags
{


	k_network_channel_client_flags_count
};

enum e_network_channel_activity
{


	k_network_channel_activity_count = 6
};

enum e_network_channel_closure_reason : long
{
	_network_channel_reason_none,
	_network_channel_reason_link_destroyed,
	_network_channel_reason_link_refused_listen,
	_network_channel_reason_channel_deleted,
	_network_channel_reason_connect_timeout,
	_network_channel_reason_connect_refused,
	_network_channel_reason_connect_reinitiate,
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

	k_network_channel_reason_count = 19
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

struct s_channel_configuration
{
	s_transport_address address;
};

struct s_client_iterator
{
	long required_client_flags;
	long client_designator;
	long client_index;
	long : 32;
	long : 32;
};

class c_network_channel_owner
{
	void* vftable;
};

struct s_client_info
{
	long flags; // e_network_channel_client_flags
	c_network_channel_client* client;
};

class c_network_channel_simulation_interface
{
	bool m_initialized;
	void* m_simulation_context;
	void* m_simulation_closure_callback;
	long m_client_count;
	s_client_info m_clients[k_network_channel_maximum_base_clients];








	//bool m_is_authority;
};

class c_network_channel_simulation_gatekeeper : c_network_channel_client
{

};

struct s_network_channel_client_info
{
	unsigned int flags;
	c_network_channel_client* client;
};

class c_network_link;
class c_network_observer;
class c_network_message_handler;
class c_network_message_gateway;
class c_network_message_type_collection;
class c_network_channel
{
public:
	bool connected();
	char* get_name(); // m_short_name is followed by m_name
	e_network_channel_state get_state();
	//char* get_message_type_name(e_network_message_type message_type); // belongs to c_network_message_type_collection?
	bool get_remote_address(s_transport_address* remote_address);
	// allocated()
	// allocate()
	bool closed();
	void close(e_network_channel_closure_reason reason);
	// simulation_can_be_established()
	// is_voice_only()
	// is_receive_only()
	// is_transmit_only()
	bool established();
	long get_identifier();
	long get_remote_identifier();
	void open(s_transport_address const* remote_address, bool initial_connection, long channel_identifier);
	void send_connection_established(long remote_identifier);
	const char* get_closure_reason_string(e_network_channel_closure_reason reason);
	void send_message(e_network_message_type message_type, long message_size, s_network_message* message);

	c_network_link* m_link;
	c_network_observer* m_observer;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	s_channel_configuration* m_configuration; // g_network_configuration? s_channel_configuration?
	c_network_message_type_collection* m_type_collection;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	int32_t m_client_count;
	s_network_channel_client_info m_clients[k_network_channel_maximum_base_clients];
	c_network_channel_simulation_interface* m_simulation_interface;
	uint32_t m_flags;
	uint32_t m_local_identifier; // 0xA08
	uint32_t m_remote_identifier; // 0xA0C
	e_network_channel_state m_channel_state; // 0xA10
	e_network_channel_closure_reason m_closure_reason;
	s_transport_address m_local_address;
	s_transport_address m_remote_address;
	bool m_send_connect_packets;
	byte __alignA41[3];
	uint32_t m_connect_identifier;
	uint32_t m_connect_timestamp;
	int32_t m_connect_unknown;
	byte __dataA50[8];
	int32_t m_activity_times[k_network_channel_activity_count];
	byte __dataA70[4];
};
static_assert(sizeof(c_network_channel) == 0xA74);