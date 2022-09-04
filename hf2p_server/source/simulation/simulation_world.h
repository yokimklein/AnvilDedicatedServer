#pragma once
#include "simulation_view.h"
#include "simulation.h"

class c_simulation_actor
{
	byte unknown_data[0x94];
};
static_assert(sizeof(c_simulation_actor) == 0x94);

class c_simulation_player
{
	byte unknown_data[0xB0];
};
static_assert(sizeof(c_simulation_player) == 0xB0);

class c_simulation_world
{
public:
	void update_establishing_view(c_simulation_view* simulation_view);

	byte unknown1[0x4C];
	long m_establishment_identifier;
	byte unknown2[0x14];
	c_simulation_view* m_views[k_network_maximum_players_per_session];
	byte unknown3[0x4];
	c_simulation_player m_players[k_network_maximum_players_per_session];
	c_simulation_actor m_actors[k_network_maximum_players_per_session];
	byte unknown4[0x58];
};
static_assert(sizeof(c_simulation_world) == 0x1540);