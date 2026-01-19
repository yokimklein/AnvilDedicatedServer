#include "game_engine_area_set.h"
#include <objects\objects.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <simulation\game_interface\simulation_game_objects.h>

void c_area::set_selected(bool selected)
{
	if (m_object_index == NONE)
	{
		return;
	}

	s_multiplayer_object_properties* multiplayer = object_try_and_get_multiplayer(m_object_index);
	if (!multiplayer)
	{
		return;
	}

	multiplayer->flags.set(_multiplayer_object_data_unknown8, selected);
	simulation_action_object_update(m_object_index, _simulation_object_update_multiplayer_properties);
}
