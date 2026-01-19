#include "simulation_game_weapons.h"
#include <game\game.h>
#include <items\weapons.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_action.h>

void __cdecl simulation_action_weapon_state_update(datum_index weapon_index)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}
	datum_index owner_unit_index = weapon_get_owner_unit_index(weapon_index);
	datum_index owner_unit_inventory_index = weapon_get_owner_unit_inventory_index(weapon_index);
	if (owner_unit_index != NONE && owner_unit_inventory_index < 4)
	{
		simulation_action_object_update(owner_unit_index, (e_simulation_unit_update_flag)(owner_unit_inventory_index + _simulation_unit_update_weapon1_state));
	}
}