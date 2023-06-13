#pragma once

#include <cseries\cseries.h>
#include <game\game_engine_variant.h>
#include <game\game_engine_infection_traits.h>

class c_game_engine_infection_variant : c_game_engine_base_variant
{
public:
	c_game_engine_infection_variant() :
		m_variant_flags(),
		m_safe_havens(),
		m_next_zombie(),
		m_initial_zombie_count(),
		m_safe_haven_movement_time(),
		m_zombie_kill_points(),
		m_infection_points(),
		m_safe_haven_arrival_points(),
		m_suicide_points(),
		m_betrayal_points(),
		m_last_man_bonus_points(),
		m_zombie_traits(),
		m_first_zombie_traits(),
		m_safe_haven_defender_traits(),
		m_last_human_traits(),
		pad(),
		unused()
	{
	};

	c_flags<e_infection_variant_flags, byte_flags, k_infection_variant_flags> m_variant_flags;
	c_enum<e_infection_safe_havens_settings, char, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, char, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, char, k_infection_initial_zombie_count_settings> m_initial_zombie_count;

	// default: 30
	// maximum: 120
	short m_safe_haven_movement_time; // 0 is no movement

	// default: 1
	// maximum: 20
	char m_zombie_kill_points;

	// default: 0
	// maximum: 20
	char m_infection_points;

	// default: 0
	// maximum: 20
	char m_safe_haven_arrival_points;

	// default: -1
	// maximum: 20
	char m_suicide_points;

	// default: -1
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 20
	char m_last_man_bonus_points;

	c_player_traits m_zombie_traits;
	c_player_traits m_first_zombie_traits;
	c_player_traits m_safe_haven_defender_traits;
	c_player_traits m_last_human_traits;

	byte pad[4];
	byte unused[0x10];
};
static_assert(sizeof(c_game_engine_infection_variant) == 0x260);

struct s_infection_globals
{
	word __unknown0;
	word __unknown2;
	word __unknown4;
	byte __unknown6;
	word __unknown8;
	word __unknown10;
	c_area_set<c_area, 12> area_set;
	c_static_array<long, 16> __unknown44C;
	c_static_array<long, 16> __unknown50C;
	byte __unknown54C;
};
static_assert(sizeof(s_infection_globals) == 0x550);