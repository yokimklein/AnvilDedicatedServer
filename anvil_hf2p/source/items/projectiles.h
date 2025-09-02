#pragma once
#include <items\items.h>
#include <effects\effects.h>
#include <game\aim_target.h>
#include <simulation\game_interface\simulation_game_projectiles.h>

enum e_projectile_flags
{
	_projectile_flags0,
	_projectile_flags1,
	_projectile_flags2,
	_projectile_flags3,
	_projectile_flags4,
	_projectile_flags5,
	_projectile_flags6,
	_projectile_flags7,
	_projectile_flags8,
	_projectile_flags9,
	_projectile_flags10,
	_projectile_flags11,
	_projectile_flags12,
	_projectile_flags13,
	_projectile_flags14,
	_projectile_flags15,
	_projectile_flags16,
	_projectile_flags17,
	_projectile_flags18,
	_projectile_flags19,
	_projectile_flags20,
	_projectile_flags21,

	k_projectile_flags_count
};

struct _projectile_datum
{
	c_flags<e_projectile_flags, long, k_projectile_flags_count> flags;
	char action;
	word hit_global_material_type;
	real_vector3d hit_normal;
	long ignore_object_index_a;
	long ignore_object_index_b;
	c_aim_target_object targeting_data;
	long weapon_index;
	long simulation_collision_object_index;
	long simulation_collision_object_node_index;
	real_point3d simulation_collision_object_position;
	real detonation_timer;
	real detonation_timer_delta;
	real super_detonation_check_timer;
	real super_detonation_check_timer_delta;
	real arming_timer;
	real arming_timer_delta;
	real odometer;
	real bounce_odometer;
	real velocity_scale;
	real maximum_damage_distance;
	real_vector3d rotation_axis;
	real rotation_sine;
	real rotation_cosine;
	real damage_scale;
	real guided_angular_velocity;
	real last_thrust_speed;
	real thrust_odometer;
	real_point3d start_point;
	word shot_id;
	word time_of_flight_ticks;
};
static_assert(sizeof(_projectile_datum) == 0x9C);

struct projectile_datum
{
	long definition_index;
	_object_datum object;
	_projectile_datum projectile;
};
static_assert(sizeof(projectile_datum) == 0x218);

// last 2 args were compiled out to consts in ms29, leaving these out for the hook
void __fastcall projectile_detonate_effects_and_damage(datum_index projectile_index, real_point3d const* position, real_vector3d const* forward, s_effect_vector const* effect_vector, float damage_scale, datum_index object_index, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> effect_flags, c_global_material_type material_type, real_vector3d const* hit_normal, s_location const* location, bool valid_material_type, bool collided_with_invalid_material);
void __fastcall projectile_detonate_effects_and_damage_shared(long definition_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* hit_vector, s_effect_vector const* effect_vector, float damage_scale, s_damage_owner const* damage_owner, datum_index object_index, s_location const* location, short shot_id, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> effect_flags, c_global_material_type material_type, bool valid_material_type, bool collided_with_invalid_material, bool valid_parent_index, datum_index object_index2, enum e_predictability predictability);