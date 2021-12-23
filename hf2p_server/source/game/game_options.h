#pragma once
#include <cstdint>
#include <windows.h>
#include "game_engine.h"
#include "players.h"
#include "..\networking\session\network_session_membership.h"
#include "..\scenario\scenario_map_variant.h"
#include "..\simulation\simulation.h"

struct s_game_machine_options
{
	uint32_t valid_machine_mask;
	s_machine_identifier machine_identifiers[k_network_maximum_machines_per_session];
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};

struct s_game_player_options
{
	bool valid;
	byte : 8;
	uint16_t input_user_index;
	uint16_t controller_index;
	s_machine_identifier machine_identifier;
	uint64_t player_identifier;
	s_player_configuration player_configuration;
};

struct s_game_options
{
	uint32_t game_mode;
	uint8_t game_simulation;
	uint8_t network_type;
	short tick_length;
	uint64_t game_id;
	int random_seed;
	int lanquage_id;
	int determinism_version;
	int campaign_id;
	int map_id;
	char scenario_path[260];
	short zoneset_group_index;
	uint8_t __unnknown12A[4];
	bool game_is_playtest;
	uint8_t __unnknown12F;
	uint16_t game_playback;
	bool record_saved_film;
	char : 8;
	int playback_start_in_ticks;
	int playback_length_in_ticks;
	uint16_t campaign_difficulty;
	uint16_t campaign_insertion_index;
	uint16_t campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool game_is_survival;
	short : 16;
	uint8_t campaign_arguments[120];
	short : 16;
	uint8_t game_progression[128];
	uint32_t campaign_skulls_primary;
	uint32_t campaign_skulls_secondary;
	char data248[136];
	char data2D0[92];
	c_game_variant game_variant;
	c_map_variant map_variant;
	s_game_machine_options machine_options;
	s_game_player_options player_options[k_network_maximum_players_per_session];
};