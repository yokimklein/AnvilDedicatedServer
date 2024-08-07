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

struct s_simulation_item_state_data
{
	s_simulation_object_state_data object;

	bool at_rest;
	bool equipment_deployed;
	bool equipment_activated;
	short equipment_creator;
};
static_assert(sizeof(s_simulation_item_state_data) == sizeof(s_simulation_object_state_data) + 0x8);