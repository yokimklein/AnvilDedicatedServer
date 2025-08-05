#pragma once

#include <cseries\cseries.h>

struct __declspec(align(4)) s_simulation_view_player_telemetry_data
{
	long absolute_player_index;
	long controlled_entity_index;
	real_point3d position;
	real_vector3d desired_aiming_vector;
	char desired_zoom_level;
};
static_assert(sizeof(s_simulation_view_player_telemetry_data) == 0x24);

struct __declspec(align(8)) s_simulation_view_telemetry_data
{
	const c_simulation_view_telemetry_provider* provider;
	bool joining;
	ulong player_acknowledged_mask;
	long number_of_players;
	s_simulation_view_player_telemetry_data players[4];
};
static_assert(sizeof(s_simulation_view_telemetry_data) == 0xA0);

class c_simulation_view;
class c_simulation_view_telemetry_provider
{
public:
	c_simulation_view* m_view;
	s_simulation_view_telemetry_data m_telemetry_data;
};
static_assert(sizeof(c_simulation_view_telemetry_provider) == 0xA8);