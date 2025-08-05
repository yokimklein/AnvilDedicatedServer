#pragma once
#include <cseries\cseries.h>
#include <units\units.h>

class c_simulation_world;
struct c_simulation_actor
{
	datum_index m_simulation_actor_index;
	datum_index m_unit_index;
	c_simulation_world* m_world;
	bool m_exists;
	ulong m_current_control_time;
	unit_control_data m_current_control;
};
static_assert(sizeof(c_simulation_actor) == 0x94);