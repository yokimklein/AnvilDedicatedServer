#pragma once
#include "..\cseries\cseries.h"
#include "network_globals.h"

enum e_life_cycle_session_type
{

};

enum e_life_cycle_state
{
	_life_cycle_state_none = 0,
	_life_cycle_state_pre_game,
	_life_cycle_state_start_game,
	_life_cycle_state_in_game,
	_life_cycle_state_end_game_write_stats,
	_life_cycle_state_leaving,
	_life_cycle_state_joining,
	_life_cycle_state_matchmaking_start,
	_life_cycle_state_matchmaking_find_match_client,
	_life_cycle_state_matchmaking_find_match,
	_life_cycle_state_matchmaking_find_and_assemble_match,
	_life_cycle_state_matchmaking_assemble_match,
	_life_cycle_state_matchmaking_select_host,
	_life_cycle_state_matchmaking_arbitration,
	_life_cycle_state_matchmaking_prepare_map,
	_life_cycle_state_in_match,
	_life_cycle_state_end_match_write_stats,
	_life_cycle_state_post_match,

	k_life_cycle_state_count,
};

c_network_session_manager* network_get_session_manager();
void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version);
ulong network_get_time();
bool network_initialized();