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

struct s_simulation_vehicle_state_data
{
    s_simulation_object_state_data object;

    bool auto_turret_enabled;
    bool auto_primary_trigger;
    bool auto_secondary_trigger;
    real_vector3d auto_aiming_vector;
    long emp_timer;
    bool active_camo_active;
    real active_camo_maximum;
    real active_camo;
    real active_camo_regrowth;
};
static_assert(sizeof(s_simulation_vehicle_state_data) == sizeof(s_simulation_object_state_data) + 0x24);