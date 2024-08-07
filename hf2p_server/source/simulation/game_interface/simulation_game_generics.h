#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_generic_update_flag
{
	_simulation_generic_update_rigid_body = k_simulation_object_update_flag_count,

	k_simulation_generic_update_flag_count
};

struct s_simulation_generic_state_data
{
    s_simulation_object_state_data object;

    dword rigid_body_mask;
    real_matrix4x3 rigid_body_transforms[8];
};
static_assert(sizeof(s_simulation_generic_state_data) == sizeof(s_simulation_object_state_data) + 0x1A4);