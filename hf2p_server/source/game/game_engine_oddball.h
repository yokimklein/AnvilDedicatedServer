#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_oddball_traits.h>

struct s_game_engine_oddball_variant_definition;
class c_game_engine_oddball_variant : public c_game_engine_base_variant
{
public:
	c_game_engine_oddball_variant* constructor();

	void set(c_game_engine_oddball_variant const* variant, bool force);
	void set(s_game_engine_oddball_variant_definition const* definition);

	bool get_auto_ball_pickup() const;
	void set_auto_ball_pickup(bool auto_ball_pickup);

	bool get_ball_effect_enabled() const;
	void set_ball_effect_enabled(bool ball_effect_enabled);

	short get_score_to_win() const;
	void set_score_to_win(short score_to_win);

	char get_carrying_points() const;
	void set_carrying_points(char carrying_points);

	char get_kill_points() const;
	void set_kill_points(char kill_points);

	char get_ball_kill_points() const;
	void set_ball_kill_points(char ball_kill_points);

	char get_carrier_kill_points() const;
	void set_carrier_kill_points(char carrier_kill_points);

	char get_ball_count() const;
	void set_ball_count(char ball_count);

	short get_ball_spawn_delay() const;
	void set_ball_spawn_delay(short ball_spawn_delay);

	short get_ball_inactive_respawn_delay() const;
	void set_ball_inactive_respawn_delay(short ball_inactive_respawn_delay);

	c_player_traits* get_carrier_traits_writeable();
	c_player_traits const* get_carrier_traits() const;
	void set_carrier_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_oddball_variant_flags, dword_flags, k_oddball_variant_flags> m_variant_flags;
	c_enum<long, short, -1, 1000> m_score_to_win;              // default: 200
	c_enum<long, short, 0, 1000> m_early_win_score;            // default: 180, halo online specific
	c_enum<long, short, -10, 10> m_carrying_points;            // default: 1, points per second when carrying the ball
	c_enum<long, char, -10, 10> m_kill_points;                 // default: 0
	c_enum<long, char, -10, 10> m_ball_kill_points;            // default: 0, number of points for a melee kill with the ball
	c_enum<long, char, -10, 10> m_carrier_kill_points;         // default: 0, number of points for killing the ball carrier
	c_enum<long, char, 1, 3> m_ball_count;                     // default: 1
	c_enum<long, short, 0, 120> m_ball_spawn_delay;            // default: 5, time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	c_enum<long, short, 0, 120> m_ball_inactive_respawn_delay; // default: 30 time until ball respawns if it is inactive

	c_player_traits m_carrier_traits;

	byte m_pad1[2];
};
static_assert(sizeof(c_game_engine_oddball_variant) == 0x200);

struct c_oddball_engine : c_game_engine
{
public:
	//void dump_settings(s_file_reference* file) const;
};

struct s_oddball_globals
{
	struct
	{
		c_static_array<long, 5> ball_spawn_timer;
		c_static_array<real_point3d, 5> ball_spawn_position;
		c_static_array<short, 5> ball_reset_timer;
		c_static_array<long, 5> ball_owner_player_index;
		c_static_array<char, 5> ball_second_timer;
		c_static_array<char, 5> ball_second_count;
	} networked;

	struct
	{
		c_static_array<long, 5> ball_last_held_time;
	} local;
};
static_assert(sizeof(s_oddball_globals) == 0x90);