#pragma once

#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_oddball_traits.h>

class c_game_engine_oddball_variant : c_game_engine_base_variant
{
public:
	c_game_engine_oddball_variant() :
		m_variant_flags(),
		m_score_to_win(),
		m_carrying_points(),
		m_unknown_points(),
		m_kill_points(),
		m_ball_kill_points(),
		m_carrier_kill_points(),
		m_ball_count(),
		m_ball_spawn_delay(),
		m_ball_inactive_respawn(),
		m_carrier_traits(),
		pad(),
		unused()
	{
	};
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_oddball_variant_team_scoring_settings, short, k_oddball_variant_team_scoring_settings> m_team_scoring

	c_flags<e_oddball_variant_flags, dword_flags, k_oddball_variant_flags> m_variant_flags;

	// default: 200
	// maximum: 1001
	short m_score_to_win;

	// points per second when carrying the ball
	// default: 180
	// maximum: 1000
	short m_carrying_points;

	// halo online specific
	// default: 1
	// maximum: 20
	short m_unknown_points;

	// default: 0
	// maximum: 20
	char m_kill_points;

	// number of points for a melee kill with the ball
	// default: 0
	// maximum: 20
	char m_ball_kill_points;

	// number of points for killing the ball carrier
	// default: 0
	// maximum: 20
	char m_carrier_kill_points;

	// default: 1
	// maximum: 2
	char m_ball_count;

	// time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	// default: 5
	// maximum: 120
	short m_ball_spawn_delay; // seconds

	// time until ball respawns if it is inactive
	// default: 30
	// maximum: 120
	short m_ball_inactive_respawn; // seconds

	c_player_traits m_carrier_traits;

	byte pad[2];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_oddball_variant) == 0x260);

struct c_oddball_engine : c_game_engine
{
};

struct s_oddball_globals
{
	c_static_array<long, 5> __unknown0;
	c_static_array<real_point3d, 5> __unknown14;
	c_static_array<short, 5> __unknown50;
	c_static_array<long, 5> __unknown5C;
	c_static_array<char, 5> __unknown70;
	c_static_array<char, 5> __unknown75;
	c_static_array<long, 5> __unknown7C;
};
static_assert(sizeof(s_oddball_globals) == 0x90);