#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_vehicle_update_flag
{
	_simulation_vehicle_update_auto_turret = k_simulation_object_update_flag_count, // auto turret initialisation?
	_simulation_vehicle_update_auto_turret_tracking, // auto turret tracking?
	_simulation_vehicle_update_seat_power,
	_simulation_vehicle_update_active_camo,

	k_simulation_vehicle_update_flag_count
};