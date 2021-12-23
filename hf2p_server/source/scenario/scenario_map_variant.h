#pragma once
#include <windows.h>
#include <cstdint>
#include "..\math\real_math.h"
#include "..\objects\objects.h"

struct s_content_item_metadata
{
	uint64_t id;
	wchar_t name[16];
	char description[128];
	char author[16];
	uint32_t content_type;
	bool user_is_online;
	uint64_t user_id;
	uint64_t content_size;
	uint64_t timestamp;
	long : 32;
	int campaign_id;
	int map_id;
	int game_engine_type;
	uint32_t campaign_difficulty;
	uint8_t campaign_insertion_point;
	bool is_survival;
	uint64_t map_checksum;
};

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
	__int16 : 16;
	int object_index;
	int editor_object_index;
	int budget_index;
	real_point3d position;
	real_vector3d right;
	real_vector3d up;
	c_object_identifier parent_object_identifier;
	s_map_variant_placement_properties properties;
};

struct s_map_variant_budget_entry
{
	int tag_index;
	uint8_t runtime_minimum;
	uint8_t runtime_maximum;
	uint8_t runtime_current;
	uint8_t design_time_maximum;
	real cost;
};

struct c_map_variant
{
	s_content_item_metadata base;
	__int16 : 16;
	uint16_t total_placements_count;
	uint16_t used_placements_count;
	uint16_t budget_entry_count;
	int map_id;
	real_bounds world_x_bounds;
	real_bounds world_y_bounds;
	real_bounds world_z_bounds;
	int content_type;
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