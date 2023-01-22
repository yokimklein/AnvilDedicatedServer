#pragma once
#include "..\..\cseries\cseries.h"
#include "network_session_parameters_base.h"
#include "network_session_parameters_generic.h"
#include "..\logic\network_join.h"
#include "network_session_state.h"

struct s_network_session_parameter_session_mode
{
	long session_mode_sequence;
	c_enum<e_network_session_mode, long, k_network_session_mode_count> session_mode;
	dword session_mode_timestamp;
};
static_assert(sizeof(s_network_session_parameter_session_mode) == 0xC);

class c_network_session_parameter_session_mode : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_session_mode>
{
public:
	bool set(e_network_session_mode session_mode);
};
static_assert(sizeof(c_network_session_parameter_session_mode) == 0x48);

struct s_network_session_parameter_session_size
{
	long maximum_peer_count;
	long maximum_player_count;
};
static_assert(sizeof(s_network_session_parameter_session_size) == 0x8);

class c_network_session_parameter_session_size : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_session_size>
{
public:
	long get_max_player_count();
	bool set_max_player_count(long player_count);
};
static_assert(sizeof(c_network_session_parameter_session_size) == 0x40);

class c_network_session_parameter_requested_remote_join_data : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
static_assert(sizeof(c_network_session_parameter_requested_remote_join_data) == 0xD0);

class c_network_session_parameter_remote_join_data : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
static_assert(sizeof(c_network_session_parameter_remote_join_data) == 0xD0);

struct s_network_session_parameter_leader_properties
{
	c_static_string<64> override_hopper_directory;
};
static_assert(sizeof(s_network_session_parameter_leader_properties) == 0x40);

struct s_network_session_parameter_lobby_vote_set
{
	struct s_lobby_vote
	{
		byte gamemode; // ditto below, for modes
		byte map; // ID of the map info title instance on the API, these are assigned in the order that they're sent by the API
		byte number_of_votes;
	};

	c_static_array<s_lobby_vote, 2> vote_options;
	byte __unknown6; // winning vote? countdown time?
	byte __unknown7; // padding?
};
static_assert(sizeof(s_network_session_parameter_lobby_vote_set) == 0x8);

struct c_network_session_parameter_lobby_vote_set : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_lobby_vote_set>
{
};
static_assert(sizeof(c_network_session_parameter_lobby_vote_set) == 0x40);