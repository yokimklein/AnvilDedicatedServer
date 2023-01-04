#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_default.h"
#include "game_engine_juggernaut_traits.h"
#include "game_engine_variant.h"

class c_game_engine_juggernaut_variant : c_game_engine_base_variant
{
public:
	c_game_engine_juggernaut_variant() :
		m_score_to_win_round(),
		m_score_unknown(),
		m_unknown(),
		m_initial_juggernaut(),
		m_next_juggernaut(),
		m_variant_flags(),
		m_zone_movement(),
		m_zone_order(),
		m_kill_points(),
		m_juggeraut_kill_points(),
		m_kill_as_juggernaut_points(),
		m_destination_arrival_points(),
		m_suicide_points(),
		m_betrayal_points(),
		m_juggernaut_delay(),
		m_juggernaut_traits(),
		pad(),
		unused()
	{
	};

	// default: 15
	// maximum: 500
	short m_score_to_win_round;

	// default: 13
	// maximum: 500
	short m_score_unknown;

	// always set to '0'
	short m_unknown;

	c_enum<e_juggernaut_initial_juggernaut_settings, char, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, char, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_variant_flags, byte_flags, k_juggernaut_variant_flags> m_variant_flags;
	c_enum<e_juggernaut_zone_movement_settings, char, k_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, char, k_juggernaut_zone_order_settings> m_zone_order;

	// default: 0
	// maximum: 20
	char m_kill_points;

	// default: 1
	// maximum: 20
	char m_juggeraut_kill_points;

	// default: 1
	// maximum: 20
	char m_kill_as_juggernaut_points;

	// default: 1
	// maximum: 20
	char m_destination_arrival_points;

	// default: 1
	// maximum: 20
	char m_suicide_points;

	// default: 1
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 10
	char m_juggernaut_delay;

	c_player_traits m_juggernaut_traits;

	byte pad[2];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_juggernaut_variant) == 0x260);

struct c_juggernaut_engine : c_game_engine
{
};

struct s_juggernaut_globals
{
	dword __unknown0;
	word __unknown4;
	byte __unknown6;
	dword __unknown8;
	c_area_set<c_area, 12> area_set;
	c_static_array<long, 16> __unknown4CC;
};
static_assert(sizeof(s_juggernaut_globals) == 0x50C);