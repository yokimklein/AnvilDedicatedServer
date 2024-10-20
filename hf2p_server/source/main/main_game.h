#pragma once
#include <cseries\cseries.h>
#include <game\game_options.h>

enum e_game_loaded_status
{
	_game_loaded_status_none = 0,
	_game_loaded_status_map_loading,
	_game_loaded_status_map_loaded,
	_game_loaded_status_map_loaded_failure,
	_game_loaded_status_map_unloading,
	_game_loaded_status_map_reloading,
	_game_loaded_status_pregame,

	k_game_loaded_status_count
};

struct s_main_game_globals
{
	long map_memory_configuration;
	c_enum<e_game_loaded_status, long, _game_loaded_status_none, k_game_loaded_status_count> game_loaded_status;
	char game_loaded_scenario_path[260];

	bool reset_in_progress;
	bool request_level_advance;
	byte : 8;
	byte : 8;

	// ODST, game_progression_levels_block: "type" and block index
	c_enum<e_game_progression_level, long, _game_progression_level_none, k_game_progression_level_count> gp_level_advance_type;
	long gp_level_block_index;

	bool change_in_progress;
	bool game_load_pending;
	byte : 8;
	byte : 8;

	dword game_loaded_time;

	dword __unknown120;

	game_options game_loaded_options;
};

bool main_game_change_in_progress();
bool main_game_loaded_map();