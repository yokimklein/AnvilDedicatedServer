#pragma once
#include "network_session_parameters_base.h"
#include "..\transport\transport_address.h"
#include "..\transport\transport_security.h"

enum e_network_session_class
{
	_network_session_class_offline,
	_network_session_class_online,
	_network_session_class_xbox_live,

	k_network_session_class_count
};

enum e_network_session_mode
{
	_network_session_mode_none,
	_network_session_mode_idle,
	_network_session_mode_setup,
	_network_session_mode_in_game,
	_network_session_mode_end_game,
	_network_session_mode_post_game,
	// matchmaking enums likely don't exist in ms29
	//_network_session_mode_matchmaking_start,
	//_network_session_mode_matchmaking_searching,
	//_network_session_mode_matchmaking_gathering,
	//_network_session_mode_matchmaking_slave,
	//_network_session_mode_matchmaking_disbanding,
	//_network_session_mode_matchmaking_arbitrating,
	//_network_session_mode_matchmaking_choosing_game,

	k_network_session_mode_count
};

struct s_network_session_parameter_session_mode
{
	long session_mode_sequence;
	e_network_session_mode mode;
	long session_mode_timestamp;
};
static_assert(sizeof(s_network_session_parameter_session_mode) == 0xC);

class c_network_session_parameter_session_mode : public c_network_session_parameter_base
{
	s_network_session_parameter_session_mode m_data;
	s_network_session_parameter_session_mode m_requested_data;
};

struct s_network_session_parameter_session_size
{
	long maximum_peer_count;
	long maximum_player_count;
};
static_assert(sizeof(s_network_session_parameter_session_size) == 0x8);

class c_network_session_parameter_session_size : public c_network_session_parameter_base
{
public:
	long get_max_player_count();
	bool set_max_player_count(long player_count);

	s_network_session_parameter_session_size m_data;
	s_network_session_parameter_session_size m_requested_data;
};

class c_generic_network_session_parameter_qword : public c_network_session_parameter_base
{
	uint64_t m_data;
	uint64_t m_requested_data;
};

class c_generic_network_session_parameter_ulong : public c_network_session_parameter_base
{
public:
	uint32_t m_data;
	uint32_t m_requested_data;
};

class c_generic_network_session_parameter_long : public c_network_session_parameter_base
{
	long m_data;
	long m_requested_data;
};

class c_generic_network_session_parameter_short : public c_network_session_parameter_base
{
	short m_data;
	short m_requested_data;
};

class c_generic_network_session_parameter_uchar : public c_network_session_parameter_base
{
	uint8_t m_data;
	uint8_t m_requested_data;
};

class c_generic_network_session_parameter_bool : public c_network_session_parameter_base
{
	bool m_data;
	bool m_requested_data;
};

struct s_remote_session_join_data
{
	long join_state;
	long join_to;
	uint64_t join_nonce;
	long transport_platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key key;
	s_transport_secure_address host_address;
	e_network_session_class session_class;
	long join_result;
	char join_to_public_slots;
};
static_assert(sizeof(s_remote_session_join_data) == 0x50);

class c_network_session_parameter_requested_remote_join_data : public c_network_session_parameter_base
{
	s_remote_session_join_data m_data;
	s_remote_session_join_data m_requested_data;
};

class c_network_session_parameter_remote_join_data : public c_network_session_parameter_base
{
	s_remote_session_join_data m_data;
	s_remote_session_join_data m_requested_data;
};

struct s_network_session_parameter_leader_properties
{
	char override_hopper_directory[64];
};
static_assert(sizeof(s_network_session_parameter_leader_properties) == 0x40);

class c_network_session_parameter_leader_properties : public c_network_session_parameter_base
{
	s_network_session_parameter_leader_properties m_data;
	s_network_session_parameter_leader_properties m_requested_data;
};