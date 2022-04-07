#pragma once
#include <windows.h>
#include <cstdint>
#include "..\math\real_math.h"
#include "..\objects\objects.h"
#include "..\game\game_engine.h"

struct s_map_variant_placement_properties
{
	uint16_t game_engine_flags;
	uint8_t object_flags;
	char team_affiliation;
	uint8_t shared_storage;
	uint8_t respawn_time;
	char object_type;
	char boundry_shape;
	float boundry_width;
	float boundry_length;
	float boundry_top;
	float boundry_bottom;
};

struct s_map_variant_placement
{
	uint16_t flags;
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
	uint8_t runtime_minimum;
	uint8_t runtime_maximum;
	uint8_t runtime_current;
	uint8_t design_time_maximum;
	real cost;
};

struct c_map_variant
{
	s_content_item_metadata base;
	short : 16;
	uint16_t total_placements_count;
	uint16_t used_placements_count;
	uint16_t budget_entry_count;
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
	uint16_t scenario_indices[16];
	s_map_variant_budget_entry budget_entries[256];
	char unused[320];
};
static_assert(sizeof(c_map_variant) == 0xE090);