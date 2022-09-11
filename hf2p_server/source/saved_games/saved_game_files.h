#pragma once
#include "..\cseries\cseries.h"
#include "..\objects\objects.h"

enum e_content_item
{
	_content_item_usermap,
	_content_item_personal,
	_content_item_ctf,
	_content_item_slayer,
	_content_item_oddball,
	_content_item_koth,
	_content_item_juggernaut,
	_content_item_territories,
	_content_item_assault,
	_content_item_vip,
	_content_item_infection,
	_content_item_film,
	_content_item_clip,
	_content_item_screenshot,

	k_content_item_count
};

struct s_saved_game_item_metadata
{
	qword unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	dword file_type;
	bool author_is_xuid_online;
	byte pad0[3];
	qword author_id;
	qword size_in_bytes;
	qword date;
	long length_seconds;
	long campaign_id;
	long map_id;
	long game_engine_type;
	long campaign_difficulty;
	byte campaign_insertion_point;
	bool campaign_survival_enabled;
	byte pad1[1];
	qword game_id;
};
static_assert(sizeof(s_saved_game_item_metadata) == 0xF8);

struct s_content_item_metadata
{
	qword m_id;
	wchar_t m_name[16];
	char m_description[128];
	char m_author[16];
	e_content_item m_content_type;
	bool m_user_is_online;
	qword m_user_id;
	qword m_size;
	qword m_timestamp;
	long : 32;
	long m_campaign_id;
	long m_map_id;
	long m_engine;
	ulong m_campaign_difficulty;
	byte m_campaign_insertion;
	bool m_survival;
	qword m_game_instance;
};
static_assert(sizeof(s_content_item_metadata) == 0xF8);

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

struct c_map_variant
{
	s_content_item_metadata base;
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
static_assert(sizeof(c_map_variant) == 0xE090);