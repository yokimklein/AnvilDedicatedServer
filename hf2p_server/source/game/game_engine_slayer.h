#pragma once

#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_slayer_traits.h>

class c_game_engine_slayer_variant : c_game_engine_base_variant
{
public:
	c_game_engine_slayer_variant() :
		m_score_to_win(),
		m_score_unknown(),
		m_kill_points(),
		m_assist_points(),
		m_death_points(),
		m_suicide_points(),
		m_betrayal_points(),
		m_leader_killed_points(),
		m_elimination_points(),
		m_assassination_points(),
		m_headshot_points(),
		m_melee_points(),
		m_sticky_points(),
		m_splatter_points(),
		m_killing_spree_points(),
		m_leader_traits(),
		pad(),
		unused()
	{
	};

	// `c_game_engine_base_variant::m_team_scoring_method` override
	// c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings> m_team_scoring

	// default: 25
	// maximum: 251
	short m_score_to_win;

	// halo online specific
	// default: 20
	// maximum: 250
	short m_score_unknown;

	// default: 1
	// maximum: 20
	short m_kill_points;

	// default: 0
	// maximum: 20
	char m_assist_points;

	// default: 0
	// maximum: 20
	char m_death_points;

	// default: -1
	// maximum: 20
	char m_suicide_points;

	// default: -1
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 20
	char m_leader_killed_points;

	// default: 0
	// maximum: 20
	char m_elimination_points;

	// default: 0
	// maximum: 20
	char m_assassination_points;

	// default: 0
	// maximum: 20
	char m_headshot_points;

	// default: 0
	// maximum: 20
	char m_melee_points;

	// default: 0
	// maximum: 20
	char m_sticky_points;

	// default: 0
	// maximum: 20
	char m_splatter_points;

	// default: 0
	// maximum: 20
	char m_killing_spree_points;

	// leader team traits if teams enabled
	c_player_traits m_leader_traits;

	byte pad[2];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_slayer_variant) == 0x260);

struct c_slayer_engine : c_game_engine
{
};