#pragma once
#include <cstring>
#include <cseries\cseries.h>
#include <math\real_math.h>
#include <ai\ai_actions.h>
#include <ai\sector.h>

enum e_special_movement_flags
{
	_special_movement_inclines,
	_special_movement_jump,
	_special_movement_climb,
	_special_movement_vault,
	_special_movement_mount,
	_special_movement_hoist,
	_special_movement_walljump,
	_special_movement_na,
	_special_movement_rail,
	_special_movement_seam,
	_special_movement_door,

	k_special_movement_flags_count
};

enum e_hint_type_flags
{
	_hint_type_vault_step,
	_hint_type_vault_crouch,
	_hint_type_unused0,
	_hint_type_unused1,
	_hint_type_unused2,
	_hint_type_mount_step,
	_hint_type_mount_crouch,
	_hint_type_mount_stand,
	_hint_type_unused3,
	_hint_type_unused4,
	_hint_type_unused5,
	_hint_type_hoist_crouch,
	_hint_type_hoist_stand,
	_hint_type_unused6,
	_hint_type_unused7,
	_hint_type_unused8,

	k_hint_type_flags_count
};

struct path_input
{
	real pathfinding_radius;
	long ignore_source_object_index;
	long ignore_target_object_index;
	c_ai_point3d start_point;
	c_sector_ref start_sector_ref;
	real_point3d attractor_point;
	long attractor_object_index;
	real attractor_radius;
	real attractor_obstacle_radius;
	real attractor_weight;
	real search_maximum_distance;
	real hint_link_penalty;
	bool start_valid;
	bool ignore_broken_surfaces;
	bool search_bounded;
	bool obstacle_avoid_attractor;
	bool attractor_valid;
	bool giant;
	bool vehicle;
	byte __data[245];
};
static_assert(sizeof(path_input) == 0x140);
static_assert(0x0 == OFFSETOF(path_input, pathfinding_radius));
static_assert(0x4 == OFFSETOF(path_input, ignore_source_object_index));
static_assert(0x8 == OFFSETOF(path_input, ignore_target_object_index));
static_assert(0xC == OFFSETOF(path_input, start_point));
static_assert(0x1C == OFFSETOF(path_input, start_sector_ref));

struct c_path_destination
{
	c_ai_point3d m_point;
	real_vector3d m_alignment;
	c_sector_ref m_sector_ref;
	real m_target_radius;
};
static_assert(sizeof(c_path_destination) == 0x24);

struct path_step_source
{
	short type;
	short structure_index;
	union
	{
		long sector_link_index;
		long hint_index;
	};
};
static_assert(sizeof(path_step_source) == 0x8);

struct path_error_info
{
	short error_stage;
	short num_thresholds;
	path_step_source failed_threshold[3];
};
static_assert(sizeof(path_error_info) == 0x1C);

struct c_recent_obstacle
{
	long m_object_index;
	short m_ticks;
	word m_flags;
};
static_assert(sizeof(c_recent_obstacle) == 0x8);

struct special_movement
{
	c_flags<e_special_movement_flags, long, k_special_movement_flags_count> flags;
	c_flags<e_hint_type_flags, long, k_hint_type_flags_count> hint_type_flags;
	long jump_height_flags;
	bool jump_infinite;
	short ignore_size;
	short swipable_size;
	short leapable_size_min;
	short leapable_size_max;
	c_recent_obstacle* recent_obstacles;
};
static_assert(sizeof(special_movement) == 0x1C);

struct s_hint_penalty
{
	short hint_index;
	char structure_index;
	char usage_count;
};
static_assert(sizeof(s_hint_penalty) == 0x4);

struct s_hint_penalty_cache
{
	s_hint_penalty penalties[4];
	short count;
};
static_assert(sizeof(s_hint_penalty_cache) == 0x12);

struct c_hint_ref
{
	short m_structure_index;
	short m_hint_index;
};
static_assert(sizeof(c_hint_ref) == 0x4);

struct path_step
{
	short type;
	short flags;
	c_sector_ref sector_ref;
	c_hint_ref hint_ref;
	c_ai_point3d point;
};
static_assert(sizeof(path_step) == 0x1C);

struct path_result
{
	bool valid;
	bool steps_finish_path;
	bool obstacle_on_goal;
	c_ai_point3d start_point;
	c_path_destination endpoint;
	long ignorable_object_index;
	short ignorable_object_flags;
	char step_count;
	char step_index;
	path_step steps[4];
	path_error_info error_info;
};
static_assert(sizeof(path_result) == 0xCC);

struct path_state
{
	byte __opaque[0x28320];
};
// ai_scratch_allocate hands back a fixed 0x64000-byte buffer regardless of what's
// asked for (see the comment below) - this just documents that path_state actually
// fits in one, rather than silently overrunning it.
static_assert(sizeof(path_state) <= 0x64000);

void* __fastcall ai_scratch_allocate();
void __fastcall ai_scratch_free(void* memory);
bool ai_point3d_new(const real_point3d* position, datum_index pathfinding_object_index, dword pathfinding_bsp_reference, c_ai_point3d* out_ai_point);

void path_state_new(const path_input* input, path_state* state, const path_error_info* error_info, void* debug, const special_movement* movement, const s_hint_penalty_cache* penalty_cache);
void __fastcall path_state_destination(path_state* state, const c_path_destination* destination);
bool __fastcall path_state_find(path_state* state);
bool path_state_build_path(datum_index actor_index, path_state* state, path_result* result);

c_sector_ref __fastcall collision_surface_get_sector(long structure_bsp_index, long surface_index, long instanced_geometry_instance_index, datum_index object_index, dword bsp_reference, const real_point3d* point);

void __fastcall object_get_pathfinding_location(datum_index object_index, c_sector_ref* sector_ref_out, c_ai_point3d* position_out);

inline void path_input_new(path_input* input, real pathfinding_radius, bool ignore_broken_surfaces, datum_index source_object_index, real hint_link_penalty)
{
	memset(input, 0, sizeof(path_input));
	input->pathfinding_radius = pathfinding_radius;
	input->ignore_broken_surfaces = ignore_broken_surfaces;
	input->ignore_source_object_index = source_object_index;
	input->ignore_target_object_index = NONE;
	input->hint_link_penalty = hint_link_penalty;
}

inline void path_input_set_start(path_input* input, const c_ai_point3d* start_point, c_sector_ref start_sector_ref)
{
	input->start_valid = true;
	input->start_point = *start_point;
	input->start_sector_ref = start_sector_ref;
}
