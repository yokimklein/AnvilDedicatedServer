#pragma once
#include "..\cseries\cseries.h"
#include "game_engine_variant.h"
#include "game_engine_ctf_traits.h"
#include "game_engine_area_set.h"

class c_game_engine_ctf_variant : c_game_engine_base_variant
{
public:
	c_game_engine_ctf_variant() :
		m_variant_flags(),
		m_home_flag_waypoint(),
		m_game_type(),
		m_respawn(),
		m_touch_return_timeout(),
		m_sudden_death_time(),
		m_score_to_win(),
		m_score_unknown(),
		m_flag_reset_time(),
		m_carrier_traits(),
		pad(),
		unused()
	{
	};

	c_flags<e_ctf_variant_flags, char, k_ctf_variant_flags> m_variant_flags;
	c_enum<e_ctf_home_flag_waypoint_settings, char, k_ctf_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_game_type_settings, char, k_ctf_game_type_settings> m_game_type;
	c_enum<e_ctf_respawn_settings, char, k_ctf_respawn_settings> m_respawn;
	c_enum<e_ctf_touch_return_settings, short, k_ctf_touch_return_settings> m_touch_return_timeout;
	c_enum<e_ctf_sudden_death_time, short, k_ctf_sudden_death_times> m_sudden_death_time;

	// default: 5
	// maximum: 50
	short m_score_to_win;

	// halo online specific
	// default: 3
	// maximum: 50
	short m_score_unknown;

	// default: 30
	// maximum: 300
	short m_flag_reset_time; // seconds

	c_player_traits m_carrier_traits;

	byte pad[6];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_ctf_variant) == 0x260);

struct s_ctf_shared_globals
{
	c_static_array<c_area_set<c_area, 3>, 9> area_set0;
	c_static_array<c_area_set<c_area, 3>, 9> area_setB1C;
	byte __data1638[0x12];
	word_flags flags;
};
static_assert(sizeof(s_ctf_shared_globals) == 0x164C);

struct s_ctf_globals
{
	s_ctf_shared_globals shared_globals;
	c_static_array<dword, 9> __unknown164C;
	c_static_array<word, 9> __unknown1670;
	c_static_array<dword, 9> __unknown1684;
	c_static_array<byte, 9> __unknown16A8;
	c_static_array<word, 9> __unknown16B4;
	c_static_array<word, 9> __unknown16C4;
};
static_assert(sizeof(s_ctf_globals) == 0x16D8);