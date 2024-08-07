#pragma once
#include <simulation\game_interface\simulation_game_items.h>

enum e_simulation_weapon_update_flag
{
	_simulation_weapon_update_weapon_flags = k_simulation_item_update_flag_count,
	_simulation_weapon_update_multiplayer_weapon_registration,
	_simulation_weapon_update_ammo,

	k_simulation_weapon_update_flag_count
};

struct s_simulation_weapon_state_data
{
    s_simulation_item_state_data item;

    byte_flags multiplayer_state_flags;
    byte __pad1[0x3];
    short multiplayer_weapon_identifier;
    short rounds_loaded;
    short rounds_inventory;
    real age;
};
static_assert(sizeof(s_simulation_weapon_state_data) == sizeof(s_simulation_item_state_data) + 0x10);

void __cdecl simulation_action_weapon_state_update(datum_index weapon_index);