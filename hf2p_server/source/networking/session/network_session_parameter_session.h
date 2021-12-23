#pragma once
#include "network_session_parameters_base.h"
#include "..\transport\transport_address.h"
#include "..\transport\transport_security.h"

enum e_network_session_mode
{
	_network_session_mode_none,
	_network_session_mode_idle,
	_network_session_mode_setup,
	_network_session_mode_in_game,
	_network_session_mode_end_game,
	_network_session_mode_post_game,
	// matchmaking enums may not exist in ms29
	_network_session_mode_matchmaking_start,
	_network_session_mode_matchmaking_searching,
	_network_session_mode_matchmaking_gathering,
	_network_session_mode_matchmaking_slave,
	_network_session_mode_matchmaking_disbanding,
	_network_session_mode_matchmaking_arbitrating,
	_network_session_mode_matchmaking_choosing_game,

	k_network_session_mode_count
};

struct s_network_session_parameter_session_mode
{
	int set_counter;
	e_network_session_mode mode;
	int last_set_time;
};

class c_network_session_parameter_session_mode : c_network_session_parameter_base
{
	s_network_session_parameter_session_mode m_data;
	s_network_session_parameter_session_mode m_requested_data;
};

struct s_network_session_parameter_session_size
{
	int max_peer_count;
	int max_player_count;
};

struct c_network_session_parameter_session_size : c_network_session_parameter_base
{
	s_network_session_parameter_session_size m_data;
	s_network_session_parameter_session_size m_requested_data;
};

struct c_generic_network_session_parameter_qword : c_network_session_parameter_base
{
	uint64_t m_data;
	uint64_t m_requested_data;
};

struct c_generic_network_session_parameter_ulong : c_network_session_parameter_base
{
	uint32_t m_data;
	uint32_t m_requested_data;
};

struct c_generic_network_session_parameter_long : c_network_session_parameter_base
{
	int m_data;
	int m_requested_data;
};

struct c_generic_network_session_parameter_short : c_network_session_parameter_base
{
	short m_data;
	short m_requested_data;
};

struct c_generic_network_session_parameter_uchar : c_network_session_parameter_base
{
	uint8_t m_data;
	uint8_t m_requested_data;
};

struct c_generic_network_session_parameter_bool : c_network_session_parameter_base
{
	bool m_data;
	bool m_requested_data;
};

struct s_network_session_remote_session_join_data
{
	int unknown0;
	int unknown4;
	unsigned __int64 join_nonce;
	int transport_platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key key;
	s_transport_secure_address host_address;
	int session_class;
	int status;
	char unknown_char;
};

struct c_network_session_parameter_requested_remote_join_data : c_network_session_parameter_base
{
	s_network_session_remote_session_join_data m_data;
	s_network_session_remote_session_join_data m_requested_data;
};

struct c_network_session_parameter_remote_join_data : c_network_session_parameter_base
{
	s_network_session_remote_session_join_data m_data;
	s_network_session_remote_session_join_data m_requested_data;
};

struct s_network_session_parameter_leader_properties
{
	char data[64];
};

struct c_network_session_parameter_leader_properties : c_network_session_parameter_base
{
	s_network_session_parameter_leader_properties m_data;
	s_network_session_parameter_leader_properties m_requested_data;
};