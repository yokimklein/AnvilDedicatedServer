#pragma once
#include "..\cseries\cseries.h"
#include "..\simulation\simulation.h"
#include "..\shell\shell.h"
#include "game_progression.h"
#include "game_variant.h"
#include "..\tools\network_webstats.h"
#include "..\scenario\scenario_map_variant.h"
#include "players.h"
#include "player_configuration.h"

#pragma pack(push, 1)
struct game_machine_options
{
	dword valid_machine_mask;
	s_machine_identifier machine_identifiers[k_network_maximum_machines_per_session];
	bool local_machine_exists;
	s_machine_identifier local_machine;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(game_machine_options) == 0x128);
#pragma pack(pop)

struct game_player_options
{
	bool valid;
	byte : 8;
	ushort input_user_index;
	ushort controller_index;
	s_machine_identifier machine_identifier;
	s_player_identifier player_identifier;
	s_player_configuration player_configuration;
};
static_assert(sizeof(game_player_options) == 0xB90);

struct game_options
{
	c_enum<e_game_mode, long, k_game_mode_count> game_mode;
	c_enum<e_game_simulation_type, char, k_game_simulation_count> game_simulation;
	byte game_network_type;
	short game_tick_rate;
	qword game_instance;
	dword random_seed;
	c_enum<e_language, long, k_language_count> language;
	long determinism_version;
	long campaign_id;
	long map_id;
	char scenario_path[260];
	short zoneset_group_index;
	bool load_level_only;
	bool dump_machine_index;
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	byte : 8;
	c_enum<e_game_playback_type, short, k_game_playback_count> game_playback;
	bool record_saved_film;
	byte : 8;
	long playback_start_in_ticks;
	long playback_length_in_ticks;
	c_enum<e_campaign_difficulty_level, short, k_campaign_difficulty_levels_count> campaign_difficulty;
	short campaign_insertion_index;
	short campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool survival_enabled;
	byte : 8; // halo3_tag_test: campaign_allow_persistent_storage
	byte : 8; // halo3_tag_test: campaign_customization_enabled
	s_campaign_armaments campaign_armaments;
	byte : 8;
	byte : 8;
	s_campaign_game_progression campaign_game_progression;
	c_flags<e_campaign_skulls_primary, long, k_campaign_skulls_primary_count> skulls_primary;
	c_flags<e_campaign_skulls_secondary, long, k_campaign_skulls_secondary_count> skulls_secondary;
	s_hub_progression hub_progression;
	bool matchmade_game;
	byte __align2C9[7];
	s_game_matchmaking_options matchmaking_options;
	c_game_variant game_variant;
	c_map_variant map_variant;
	game_machine_options machine_options;
	c_static_array<game_player_options, k_network_maximum_players_per_session> players;
};
static_assert(sizeof(game_options) == 0x1A048);