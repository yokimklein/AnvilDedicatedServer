#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\transport\transport_security.h>
#include <networking\network_time.h>

// reach replaced this with get_network_protocol_version()
constexpr long k_network_protocol_version = 9; // H3 is 7, ODST is 8

struct s_network_message_join_request
{
	ushort protocol;
	short executable_type;
	long minimum_network_version;
	long current_network_version;
	s_transport_secure_identifier session_id;
	long : 32;
	s_network_session_join_request join_request;
	long : 32;
};
static_assert(sizeof(s_network_message_join_request) == 0x258);

struct s_network_message_peer_connect
{
	ushort protocol;
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_peer_connect) == 0x20);

struct s_network_message_join_abort
{
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_join_abort) == 0x18);

struct s_network_message_join_refuse
{
	s_transport_secure_identifier session_id;
	e_network_join_refuse_reason reason;
};
static_assert(sizeof(s_network_message_join_refuse) == 0x14);

struct s_network_message_leave_session
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_session) == 0x10);

struct s_network_message_leave_acknowledge
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_acknowledge) == 0x10);

struct s_network_message_session_disband
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_session_disband) == 0x10);

struct s_network_message_session_boot
{
	s_transport_secure_identifier session_id;
	enum e_network_session_boot_reason reason;
};
static_assert(sizeof(s_network_message_session_boot) == 0x14);

struct s_network_message_host_decline
{
	s_transport_secure_identifier session_id;
	bool session_exists;
	bool peer_exists;
	bool host_exists;
	char : 8;
	s_transport_secure_address host_address;
};
static_assert(sizeof(s_network_message_host_decline) == 0x24);

struct s_network_message_peer_establish
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_peer_establish) == 0x10);

struct s_network_message_time_synchronize
{
	s_transport_secure_identifier session_id;
	ulong client_timestamp[2];
	ulong authority_timestamp[2];
	e_network_time_synchronize_stage synchronization_stage;
};
static_assert(sizeof(s_network_message_time_synchronize) == 0x24);