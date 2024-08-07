#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_device_update_flag
{
	_simulation_device_update_position = k_simulation_object_update_flag_count,
	_simulation_device_update_15, // TODO
	_simulation_device_update_power,
	_simulation_device_update_17, // power group related

	k_simulation_device_update_flag_count
};

struct s_simulation_device_state_data
{
	s_simulation_object_state_data object;

	real position;
	real position_group_position;
	real power;
	real power_group_power;
};
static_assert(sizeof(s_simulation_device_state_data) == sizeof(s_simulation_object_state_data) + 0x10);