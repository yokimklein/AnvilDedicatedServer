#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_projectile_update_flag
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,
	// surprisingly nothing was added for projectile latching

	k_simulation_projectile_update_flag_count
};

struct s_simulation_projectile_state_data
{
	s_simulation_object_state_data object;

	bool at_rest;
};
static_assert(sizeof(s_simulation_projectile_state_data) == sizeof(s_simulation_object_state_data) + 0x4);