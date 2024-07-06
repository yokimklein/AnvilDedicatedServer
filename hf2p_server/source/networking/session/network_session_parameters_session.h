#pragma once
#include <cseries\cseries.h>
#include <networking\session\network_session_parameters_base.h>
#include <networking\session\network_session_parameters_generic.h>
#include <networking\transport\transport_address.h>
#include <networking\transport\transport_security.h>

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

enum e_network_session_class
{
	_network_session_class_offline,
	_network_session_class_online,
	_network_session_class_xbox_live,

	k_network_session_class_count
};

enum e_life_cycle_join_result
{
	_life_cycle_join_result_none = 0,
	_life_cycle_join_result_join_in_progress,
	_life_cycle_join_result_join_failed_generic_error,
	_life_cycle_join_result_join_failed_to_find_session,
	_life_cycle_join_result_join_failed_not_enough_space,
	_life_cycle_join_result_join_failed_game_not_open,
	_life_cycle_join_result_join_failed_target_is_matchmaking,
	_life_cycle_join_result_join_failed_host_timed_out,
	_life_cycle_join_result_join_failed_peer_version_too_low,
	_life_cycle_join_result_join_failed_host_version_too_low,
	_life_cycle_join_result_join_failed_unable_to_connect_open_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_moderate_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_closed_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_open_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_moderate_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_strict_nat,
	_life_cycle_join_result_join_failed_party_member_not_online_enabled,
	_life_cycle_join_result_join_failed_film_in_progress,
	_life_cycle_join_result_join_failed_campaign_in_progress,
	_life_cycle_join_result_join_failed_user_content_not_permitted,
	_life_cycle_join_result_join_failed_survival_in_progress,
	_life_cycle_join_result_join_failed_invalid_executable_type,

	k_life_cycle_join_result_count
};

enum e_join_remote_state
{
	_join_remote_state_none = 0,
	_join_remote_state_party_start_join,
	_join_remote_state_party_join_host,
	_join_remote_state_party_join_clients,
	_join_remote_state_party_leave_old_squad,
	_join_remote_state_party_join_complete,
	_join_remote_state_not_used, // _join_remote_state_leave_group in older builds

	k_join_remote_state_count
};

enum e_join_type
{
	_join_type_squad = 0,
	_join_type_group,

	k_join_type_count
};

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

struct s_network_session_remote_session_join_data
{
	c_enum<e_join_remote_state, long, k_join_remote_state_count> join_state;
	c_enum<e_join_type, long, k_join_type_count> join_to;
	qword join_nonce;
	c_enum<e_transport_platform, long, k_transport_platform_count> platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key session_key;
	s_transport_secure_address host_secure_address;
	c_enum<e_network_session_class, long, k_network_session_class_count> session_class;
	c_enum<e_life_cycle_join_result, long, k_life_cycle_join_result_count> join_result;
	bool join_to_public_slots;
};
static_assert(sizeof(s_network_session_remote_session_join_data) == 0x50);

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
	byte winning_vote_index;
	byte : 8;
};
static_assert(sizeof(s_network_session_parameter_lobby_vote_set) == 0x8);

struct c_network_session_parameter_lobby_vote_set : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_lobby_vote_set>
{
public:
	bool set(s_network_session_parameter_lobby_vote_set* vote_set);
	void get(s_network_session_parameter_lobby_vote_set* output);
};
static_assert(sizeof(c_network_session_parameter_lobby_vote_set) == 0x40);