#pragma once
#include <cseries\cseries.h>
#include <units\units.h>

class c_simulation_world;
struct c_simulation_actor
{
	datum_index simulation_actor_index;
	datum_index unit_index;
	c_simulation_world* m_world;
	long __unknownC;
	dword m_time;
	unit_control_data m_actor_control;
};
static_assert(sizeof(c_simulation_actor) == 0x94);