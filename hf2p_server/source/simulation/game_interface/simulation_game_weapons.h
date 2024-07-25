#pragma once
#include <simulation\game_interface\simulation_game_items.h>

enum e_simulation_weapon_update_flag
{
	_simulation_weapon_update_weapon_flags = k_simulation_item_update_flag_count,
	_simulation_weapon_update_multiplayer_weapon_registration,
	_simulation_weapon_update_ammo,

	k_simulation_weapon_update_flag_count
};

void __cdecl simulation_action_weapon_state_update(datum_index weapon_index);