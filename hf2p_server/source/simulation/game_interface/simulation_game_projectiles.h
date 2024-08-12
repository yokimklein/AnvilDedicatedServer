#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_projectile_update_flag
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,
	// surprisingly nothing was added for projectile latching

	k_simulation_projectile_update_flag_count
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

void simulation_action_projectile_attached(datum_index projectile_index, datum_index object_index, short node_index, real_point3d const* position, s_location const* location);