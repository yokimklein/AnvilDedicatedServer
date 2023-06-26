#pragma once
#include <game\game_options.h>
#include <game\game_progression.h>
#include <render\render_visibility_collection.h>

struct game_globals_storage
{
	bool initializing;
	bool map_active;
	byte : 8;
	byte : 8;

	dword active_structure_bsp_mask;
	dword active_designer_zone_mask;
	dword active_cinematic_zone_mask;

	game_options options;

	// ODST
	dword active_game_progression_level; // string id
	s_campaign_game_progression active_game_progression;

	bool game_in_progress;

	bool game_lost;
	bool game_revert;
	bool prepare_for_game_progression; // false if `active_game_progression_level` is `_invalid_string_id`
	long game_lost_wait_time;

	bool game_finished;
	byte : 8;
	byte : 8;
	byte : 8;
	long game_finished_wait_time;

	c_flags<e_campaign_skulls_primary, dword, k_campaign_skulls_primary_count> active_primary_skulls;
	c_flags<e_campaign_skulls_secondary, dword, k_campaign_skulls_secondary_count> active_secondary_skulls;

	byte : 8;

	// game_frame
	// game_had_an_update_tick_this_frame
	bool update_tick_this_frame;

	byte : 8;
	byte : 8;

	s_game_cluster_bit_vectors cluster_pvs;
	s_game_cluster_bit_vectors cluster_pvs_local;
	s_game_cluster_bit_vectors cluster_activation;

	// game_pvs_enable_scripted_camera_pvs
	// game_pvs_clear_scripted_camera_pvs
	bool scripted_camera_pvs;
	byte : 8;

	// game_pvs_scripted_clear
	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	word scripted;

	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	// scenario_group, scenario_block, scenario_cutscene_camera_point_block, camera_point_index
	datum_index scripted_object_index; // scenario_group.scenario_block.cutscene_camera_points[camera_point_index]

	long game_ragdoll_count;

	byte __data25204[4];
};
static_assert(sizeof(game_globals_storage) == 0x1A708);

struct s_globals_definition
{
	byte data[0x1B0];
	s_tag_reference multiplayer_globals;
	byte data2[0x45C];
};
static_assert(sizeof(s_globals_definition) == 0x61C);

game_globals_storage* game_globals_get();
float game_difficulty_get_team_value(long value_type, long team);

extern s_globals_definition*& g_globals_definition;