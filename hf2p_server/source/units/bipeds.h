#pragma once
#include <units\units.h>
#include <ai\sector.h>
#include <physics\character_physics.h>
#include <memory\ring_buffer.h>

enum e_biped_flag
{
	_biped_flag0,

	k_biped_flag_count
};

struct s_biped_data : s_unit_data
{
	c_flags<e_biped_flag, word, k_biped_flag_count> biped_flags; // TODO: just flags in biped, refactor object structs

	short pathfinding_structure_index;
	long pathfinding_time;
	long pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	long pathfinding__unknown10;
	long pathfinding_object_index;
	dword pathfinding_bsp_reference;
	real_point3d pathfinding_point;

	byte __data28[0x18];

	long current_bump_object_index;
	long bump_object_index;
	char bump_ticks;
	char jump_control_ticks;
	byte stun_ticks; // char?
	char last_known_speed_scale;
	real lean;
	real camera_offset_z;
	real camera_offset_y;
	real camera_height;
	long jump_time;
	long land_time;
	short current_gate_type;
	short current_gate_index;
	long current_gate_start_time;

	byte __data6C[0x28];

	c_character_physics_component physics;
	s_object_header_block_reference simulation_interpolation;
	s_object_header_block_reference last_node_matrices_storage;
	real_vector3d root_offset;
	real_vector3d ground_fit_normal;
	real pivot_on_foot_scale;
	real pivot_on_foot_scale_boost;
	real_point3d pivot_point;
	real_vector2d pivot_fixup;

	real_matrix4x3 __matrix150;
	real_matrix4x3 __matrix184;

	byte __data1B8[0xC];

	t_static_ring_buffer<real_matrix4x3, 3> root_matrix_history;
	real landing_recovery_offset;
	real pendulum_scale;
	real_vector3d pendulum_vector;
	real gravity_scale;

	// linked, armor related
	long customized_area; // previously a pair of 10 member long arrays, one long & one short

	// linked
	long __unknown2C0;
	short __unknown2C4;
	byte __data2C6[0x1];
	bool __unknown2C7;
};
static_assert(sizeof(s_biped_data) == 0x828);
static_assert(0x598 == OFFSETOF(s_biped_data, biped_flags));

bool __fastcall biped_calculate_melee_aiming(datum_index biped_index, real_vector3d* melee_aiming_vector);
bool __fastcall biped_update_melee_turning(datum_index biped_index);