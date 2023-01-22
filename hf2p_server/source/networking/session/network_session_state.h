#pragma once

enum e_network_session_state : long
{
	_network_session_state_none,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,
	// removed in ms29
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
	_dedicated_server_session_state_in_game,
	_dedicated_server_session_state_unknown,

	k_dedicated_server_session_state_count
};

enum e_network_rough_quality
{

};

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