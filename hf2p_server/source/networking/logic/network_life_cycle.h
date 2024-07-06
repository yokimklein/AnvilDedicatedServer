#pragma once
#include <networking\session\network_session_parameters_session.h>
#include <networking\session\network_observer.h>
#include <networking\logic\life_cycle\life_cycle_manager.h>
#include <networking\logic\life_cycle\life_cycle_handler_none.h>
#include <networking\logic\life_cycle\life_cycle_handler_pre_game.h>
#include <networking\logic\life_cycle\life_cycle_handler_start_game.h>
#include <networking\logic\life_cycle\life_cycle_handler_in_game.h>
#include <networking\logic\life_cycle\life_cycle_handler_end_game_write_stats.h>
#include <networking\logic\life_cycle\life_cycle_handler_leaving.h>
#include <networking\logic\life_cycle\life_cycle_handler_joining.h>

struct s_network_life_cycle_globals
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;

	// Removed since h3/ms23
	//byte : 8;
	//byte : 8;
	//byte : 8;
	//byte : 8;

	c_life_cycle_state_manager state_manager;
	c_life_cycle_state_handler_none handler_none;
	c_life_cycle_state_handler_pre_game handler_pre_game;
	c_life_cycle_state_handler_start_game handler_start_game;
	c_life_cycle_state_handler_in_game handler_in_game;
	c_life_cycle_state_handler_end_game_write_stats handler_end_game_write_stats;
	c_life_cycle_state_handler_leaving handler_leaving;
	c_life_cycle_state_handler_joining handler_joining;

	// Removed since h3/ms23
	//c_life_cycle_state_handler_matchmaking_start handler_matchmaking_start;
	//c_life_cycle_state_handler_matchmaking_find_match_client handler_matchmaking_find_match_client;
	//c_life_cycle_state_handler_matchmaking_find_match handler_matchmaking_find_match;
	//c_life_cycle_state_handler_matchmaking_find_and_assemble_match handler_matchmaking_find_and_assemble_match;
	//c_life_cycle_state_handler_matchmaking_assemble_match handler_matchmaking_assemble_match;
	//c_life_cycle_state_handler_matchmaking_select_host handler_matchmaking_select_host;
	//c_life_cycle_state_handler_matchmaking_arbitration handler_matchmaking_arbitration;
	//c_life_cycle_state_handler_matchmaking_prepare_map handler_matchmaking_prepare_map;
	//c_life_cycle_state_handler_in_match handler_in_match;
	//c_life_cycle_state_handler_end_match_write_stats handler_end_match_write_stats;
	//c_life_cycle_state_handler_post_match handler_post_match;
};
static_assert(sizeof(s_network_life_cycle_globals) == 0x118);
static_assert(0x4 == OFFSETOF(s_network_life_cycle_globals, state_manager));
static_assert(0x68 == OFFSETOF(s_network_life_cycle_globals, handler_none));
static_assert(0x78 == OFFSETOF(s_network_life_cycle_globals, handler_pre_game));
static_assert(0x94 == OFFSETOF(s_network_life_cycle_globals, handler_start_game));
static_assert(0xA8 == OFFSETOF(s_network_life_cycle_globals, handler_in_game));
static_assert(0xD0 == OFFSETOF(s_network_life_cycle_globals, handler_end_game_write_stats));
static_assert(0xE8 == OFFSETOF(s_network_life_cycle_globals, handler_leaving));
static_assert(0xFC == OFFSETOF(s_network_life_cycle_globals, handler_joining));

extern s_network_life_cycle_globals& life_cycle_globals;

void __fastcall network_life_cycle_end();
bool __fastcall network_life_cycle_create_local_squad(enum e_network_session_class session_class);
bool network_life_cycle_get_observer(c_network_observer** out_observer);
c_network_session* network_life_cycle_get_squad_session_for_join();
bool network_life_cycle_in_squad_session(c_network_session** out_session);