#pragma once
#include "..\saved_games\saved_game_files.h"
#include "..\game\game_engine.h"
#include "..\dllmain.h"

struct s_map_variant_placement_properties
{
	word_flags game_engine_flags;
	byte_flags object_flags;
	char team_affiliation;
	byte shared_storage;
	byte respawn_time;
	char object_type;
	char boundry_shape;
	float boundry_width;
	float boundry_length;
	float boundry_top;
	float boundry_bottom;
};

struct s_map_variant_placement
{
	word_flags flags;
	short : 16;
	long object_index;
	long editor_object_index;
	long budget_index;
	real_point3d position;
	real_vector3d right;
	real_vector3d up;
	c_object_identifier parent_object_identifier;
	s_map_variant_placement_properties properties;
};

struct s_map_variant_budget_entry
{
	long tag_index;
	byte runtime_minimum;
	byte runtime_maximum;
	byte runtime_current;
	byte design_time_maximum;
	real cost;
};

class c_map_variant
{
public:
	c_map_variant();
	void create_default(e_map_id map_id);

	s_saved_game_item_metadata base;
	short : 16;
	ushort total_placements_count;
	ushort used_placements_count;
	ushort budget_entry_count;
	long map_id;
	real_bounds world_x_bounds;
	real_bounds world_y_bounds;
	real_bounds world_z_bounds;
	long content_type;
	real maximum_budget;
	real current_budget;
	long : 32;
	long : 32;
	s_map_variant_placement placements[640];
	ushort scenario_indices[16];
	s_map_variant_budget_entry budget_entries[256];
	char unused[320];
};
static_assert(sizeof(c_map_variant) == 0xE090); // is this actually 0xE108 now?