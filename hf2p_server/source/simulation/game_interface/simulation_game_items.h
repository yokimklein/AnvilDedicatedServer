#pragma once
#include <simulation\game_interface\simulation_game_objects.h>

enum e_simulation_item_update_flag
{
	_simulation_item_update_set_at_rest = k_simulation_object_update_flag_count,
	// equipment only
	_simulation_item_update_equipment_begin_animation_state,
	_simulation_item_update_equipment_creation_time,
	_simulation_item_update_equipment_owner,

	k_simulation_item_update_flag_count
};