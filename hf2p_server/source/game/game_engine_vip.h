#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_default.h"
#include "game_engine_variant.h"
#include "game_engine_vip_traits.h"

class c_game_engine_vip_variant : c_game_engine_base_variant
{
public:
	c_game_engine_vip_variant() :
		m_score_to_win_round(),
		m_score_unknown(),
		m_variant_flags(),
		m_kill_points(),
		m_takedown_points(),
		m_kill_as_vip_points(),
		m_vip_death_points(),
		m_destination_arrival_points(),
		m_suicide_points(),
		m_betrayal_points(),
		m_vip_suicide_points(),
		m_vip_selection(),
		m_zone_movement(),
		m_zone_order(),
		pad(),
		m_influence_radius(),
		m_vip_team_traits(),
		m_vip_influence_traits(),
		m_vip_traits(),
		unused()
	{
	};

	// default: 15
	// maximum: 500
	short m_score_to_win_round;

	// halo online specific
	// default: 10
	// maximum: 500
	short m_score_unknown;

	c_flags<e_vip_variant_flags, word_flags, k_vip_variant_flags> m_variant_flags;

	// default: 0
	// maximum: 20
	char m_kill_points;

	// default: 0
	// maximum: 20
	char m_takedown_points;

	// default: 0
	// maximum: 20
	char m_kill_as_vip_points;

	// default: 0
	// maximum: 20
	char m_vip_death_points;

	// default: 0
	// maximum: 20
	char m_destination_arrival_points;

	// default: 0
	// maximum: 20
	char m_suicide_points;

	// default: 0
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 20
	char m_vip_suicide_points;

	c_enum<e_vip_selection_settings, char, k_vip_selection_settings> m_vip_selection;
	c_enum<e_vip_zone_movement_settings, char, k_vip_zone_movement_settings> m_zone_movement;
	c_enum<e_vip_zone_order_settings, char, k_vip_zone_order_settings> m_zone_order;

	byte pad[1];

	// default: 0
	// maximum: 50
	short m_influence_radius;

	c_player_traits m_vip_team_traits;
	c_player_traits m_vip_influence_traits;
	c_player_traits m_vip_traits;

	byte unused[0x28];
};
static_assert(sizeof(c_game_engine_vip_variant) == 0x260);

struct c_vip_engine : c_game_engine
{
};

struct c_destination_zone : c_area
{
	byte_flags m_team_designator_flags;
};
static_assert(sizeof(c_destination_zone) == 0x68);

struct s_vip_globals
{
	c_static_array<long, 8> __unknown0;
	dword __unknown20;
	c_static_array<s_multiplayer_object_boundary_geometry_data, 8> geometry_datas;
	c_area_set<c_destination_zone, 12> destination_zones;
};
static_assert(sizeof(s_vip_globals) == 0x794);