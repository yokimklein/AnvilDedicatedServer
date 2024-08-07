#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_address.h>
#include <networking\messages\network_message_queue.h>
#include <networking\network_statistics.h>

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

struct s_channel_configuration
{
	long unknown_time1;
	long connect_unknown; // time between sending connect requests?
	long connect_timeout; // attempt connect requests for x milliseconds before timing out
	long establishment_timeout;
};

struct s_client_iterator
{
	dword_flags required_client_flags;
	long client_designator;
	long client_index;
	long : 32;
	long : 32;
};

class c_network_channel_owner
{
public:
	virtual bool __thiscall desire_channel_heartbeat(long channel_index);
	virtual bool __thiscall channel_is_load_bearing(long channel_index);
	virtual bool __thiscall attempt_channel_reconnection(long channel_index, bool unknown);
	virtual void __thiscall notify_channel_connection(long channel_index, long unused, bool notify_host);
	virtual void __thiscall notify_channel_died(long channel_index);
};
static_assert(sizeof(c_network_channel_owner) == 0x4);

struct s_network_channel_client_info
{
	dword_flags flags; // e_network_channel_client_flags
	c_network_channel_client* client;
};

class c_network_channel_simulation_interface
{
	bool m_initialized;
	void* m_simulation_context;
	void(__cdecl* m_closure_callback)(void*);
	long m_client_count;
	s_network_channel_client_info m_clients[k_network_channel_maximum_simulation_clients];
	bool is_authority;
	bool established;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

class c_network_channel_simulation_gatekeeper : c_network_channel_client
{

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
	const char* get_name();
	const char* get_short_name();
	e_network_channel_state get_state();
	bool get_remote_address(s_transport_address* remote_address);
	bool allocated();
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
	s_channel_configuration* m_configuration; // g_network_configuration?
	c_network_message_type_collection* m_type_collection;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	long m_client_count;
	s_network_channel_client_info m_clients[k_network_channel_maximum_base_clients];
	// is there a struct split here at 0xA00? check c_network_observer::stream_check_stream_channel_connections
	c_network_channel_simulation_interface* m_simulation_interface;
	dword_flags m_flags;
	ulong m_local_identifier; // 0xA08 // 0xA14 in H3debug
	ulong m_remote_identifier; // 0xA0C
	e_network_channel_state m_channel_state; // 0xA10
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> m_closure_reason;
	s_transport_address m_local_address;
	s_transport_address m_remote_address;
	bool m_send_connect_packets;
	byte __alignA41[3];
	long m_connect_start_timestamp;
	long m_connect_request_timestamp;
	long m_connect_unknown;
	long m_established_timestamp; // establish start
	long m_connected_timestamp; // establish complete
	long m_activity_times[k_network_channel_activity_count];
	bool m_destination_unreachable;
	byte __alignA71[3];
};
static_assert(sizeof(c_network_channel) == 0xA74);
static_assert(0xA08 == OFFSETOF(c_network_channel, m_local_identifier));
static_assert(0xA0C == OFFSETOF(c_network_channel, m_remote_identifier));