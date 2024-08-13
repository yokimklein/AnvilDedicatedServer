#pragma once
#include <simulation\game_interface\simulation_game_objects.h>
#include <game\materials.h>

enum e_simulation_projectile_update_flag
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,
	// surprisingly nothing was added for projectile latching

	k_simulation_projectile_update_flag_count
};

enum e_simulation_projectile_effect_flag
{
	_simulation_projectile_effect_flag0,
	_simulation_projectile_effect_flag1,
	_simulation_projectile_effect_flag2,
	_simulation_projectile_effect_flag3,
	_simulation_projectile_effect_flag4,
	_simulation_projectile_effect_flag5,

	k_simulation_projectile_effect_flag_count
};

// What is this for? Unused and unfinished?
struct s_simulation_projectile_state_data
{
	s_simulation_object_state_data object;
	bool at_rest;
};
static_assert(sizeof(s_simulation_projectile_state_data) == sizeof(s_simulation_object_state_data) + 0x4);

struct s_simulation_projectile_attached_event_data
{
	bool attached_to_object;
	long parent_node_index;
	long position_encoding_type;
	real_point3d node_position;
	s_location location;
	byte __data1A[0x2];
};
static_assert(sizeof(s_simulation_projectile_attached_event_data) == 0x1C);

struct s_simulation_projectile_detonate_event_data
{
	long projectile_definition_index;
	real_point3d position;
	real_vector3d forward;
	real damage_scale;
	byte projectile_effect_flag;
	bool collided_with_invalid_material;
	long material_index;

	bool location_valid;
	byte : 8;
	s_location location;

	real_vector3d hit_normal;
	long player_index0; // from?
	long player_index1; // to?
};
static_assert(sizeof(s_simulation_projectile_detonate_event_data) == 0x40);

void simulation_action_projectile_attached(datum_index projectile_index, datum_index object_index, short node_index, real_point3d const* position, s_location const* location);
void simulation_action_projectile_detonate(long projectile_definition_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* hit_normal, float damage_scale, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> projectile_effect_flag, c_global_material_type material_type, s_location const* location, long player_index0, long player_index1, bool projectile_gamestate_is_valid, bool valid_material_type, bool collided_with_invalid_material);