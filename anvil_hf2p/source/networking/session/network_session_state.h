#pragma once
#include <networking\logic\network_join.h>

enum e_network_session_state
{
	_network_session_state_none,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,
	// removed since h3/ms23
	//_network_session_state_host_handoff,
	//_network_session_state_host_reestablish,
	//_network_session_state_election,

	k_network_session_state_count
};

enum e_dedicated_server_session_state
{
	_dedicated_server_session_state_matchmaking_session,
	_dedicated_server_session_state_waiting_for_players,
	_dedicated_server_session_state_waiting_for_voting_set,
	_dedicated_server_session_state_voting,
	_dedicated_server_session_state_game_start_countdown,
	_dedicated_server_session_state_game_start_countdown2, // unknown difference
	_dedicated_server_session_state_in_game, // unknown difference
	_dedicated_server_session_state_in_game2, // unknown difference
	_dedicated_server_session_state_in_game3, // unknown difference
	_dedicated_server_session_state_in_game4, // unknown difference

	k_dedicated_server_session_state_count
};

struct s_local_state_data_peer_creating
{
	e_transport_platform secure_key_platform;
	bool connect_not_join;
	qword join_nonce;
	s_network_session_join_request join_request;
};
static_assert(sizeof(s_local_state_data_peer_creating) == 0x248);

struct s_local_state_data_peer_joining
{
	long observer_channel_index;
	s_transport_secure_address join_remote_address;
	transport_address join_usable_address;
	bool connect_not_join;
	qword join_nonce;
	s_network_session_join_request join_request;
	long : 32;
	dword join_initiated_timestamp;
	dword join_ping_from_host_timestamp;
	dword join_secure_connection_timestamp;
	long join_attempt_count;
	dword last_join_attempt_timestamp;
};
static_assert(sizeof(s_local_state_data_peer_joining) == 0x288);

struct s_local_state_data_peer_join_abort
{
	s_transport_secure_address join_remote_address;
	transport_address join_usable_address;
	s_transport_secure_address joining_local_address;
	qword join_nonce;
	dword join_initiated_timestamp;
	dword join_abort_initiated_timestamp;
	dword last_join_abort_timestamp;
};
static_assert(sizeof(s_local_state_data_peer_join_abort) == 0x50);

struct s_local_state_data_peer_established
{
	dword peer_reestablishment_state; // e_peer_reestablish_state
	dword established_timestamp;
};
static_assert(sizeof(s_local_state_data_peer_established) == 0x8);

struct s_local_state_data_peer_leaving
{
	long peer_reestablishment_state; // e_peer_reestablish_state
	dword leave_initiated_timestamp;
	dword last_leave_attempt_timestamp;
};
static_assert(sizeof(s_local_state_data_peer_leaving) == 0xC);

struct s_local_state_data
{
	union
	{
		s_local_state_data_peer_creating peer_creating;
		s_local_state_data_peer_joining peer_joining;
		s_local_state_data_peer_join_abort peer_join_abort;
		s_local_state_data_peer_established peer_established;
		s_local_state_data_peer_leaving peer_leaving;
		//s_local_state_data_host_handoff host_handoff;
		//s_local_state_data_host_reestablish host_reestablish;
		//s_local_state_data_election election;
	};
};