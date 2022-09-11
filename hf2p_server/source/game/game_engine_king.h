#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_default.h"
#include "game_engine_koth_traits.h"
#include "game_engine_variant.h"

class c_game_engine_king_variant : c_game_engine_base_variant
{
public:
	c_game_engine_king_variant() :
		m_variant_flags(),
		m_score_to_win(),
		m_score_unknown(),
		m_moving_hill(),
		m_moving_hill_order(),
		m_uncontested_hill_bonus_points(),
		m_points_per_kill(),
		m_inside_hill_points(),
		m_outside_hill_points(),
		m_inside_hill_traits_name(),
		pad(),
		unused()
	{
	};

	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_king_team_scoring_settings, short, k_king_team_scoring_settings> m_team_scoring

	c_flags<e_king_variant_flags, dword_flags, k_king_variant_flags> m_variant_flags;

	// default: 100
	// maximum: 1000
	short m_score_to_win;

	// halo online specific
	// default: 90
	// maximum: 1000
	short m_score_unknown;

	c_enum<e_king_moving_hill_settings, char, k_king_moving_hill_settings> m_moving_hill;
	c_enum<e_king_moving_hill_order_settings, char, k_king_moving_hill_order_settings> m_moving_hill_order;

	// default: 0
	// maximum: 20
	char m_uncontested_hill_bonus_points;

	// default: 0
	// maximum: 20
	char m_points_per_kill;

	// default: 0
	// maximum: 20
	char m_inside_hill_points;

	// default: 0
	// maximum: 20
	char m_outside_hill_points;

	c_player_traits m_inside_hill_traits_name;

	byte pad[6];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_king_variant) == 0x260);

struct c_king_engine : c_game_engine
{
};