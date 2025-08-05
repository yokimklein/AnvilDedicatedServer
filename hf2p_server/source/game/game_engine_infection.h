#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_infection_traits.h>
#include <game\game_engine_area_set.h>

struct s_game_engine_infection_variant_definition;
class c_game_engine_infection_variant : public c_game_engine_base_variant
{
public:
	c_game_engine_infection_variant* constructor();

	void set(c_game_engine_infection_variant const* variant, bool force);
	void set(s_game_engine_infection_variant_definition const* definition);

	bool get_respawn_on_haven_move() const;
	void set_respawn_on_haven_move(bool respawn_on_haven_move);

	e_infection_safe_havens_settings get_safe_havens() const;
	void set_safe_havens(e_infection_safe_havens_settings safe_havens);

	e_infection_next_zombie_settings get_next_zombie() const;
	void set_next_zombie(e_infection_next_zombie_settings next_zombie);

	e_infection_initial_zombie_count_settings get_initial_zombie_count() const;
	void set_initial_zombie_count(e_infection_initial_zombie_count_settings initial_zombie_count);

	short get_safe_haven_movement_time() const;
	void set_safe_haven_movement_time(short safe_haven_movement_time);

	char get_zombie_kill_points() const;
	void set_zombie_kill_points(char zombie_kill_points);

	char get_infection_points() const;
	void set_infection_points(char infection_points);

	char get_safe_haven_arrival_points() const;
	void set_safe_haven_arrival_points(char safe_haven_arrival_points);

	char get_suicide_points() const;
	void set_suicide_points(char suicide_points);

	char get_betrayal_points() const;
	void set_betrayal_points(char betrayal_points);

	char get_last_man_bonus_points() const;
	void set_last_man_bonus_points(char last_man_bonus_points);

	c_player_traits* get_zombie_traits_writeable();
	c_player_traits const* get_zombie_traits() const;
	void set_zombie_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_first_zombie_traits_writeable();
	c_player_traits const* get_first_zombie_traits() const;
	void set_first_zombie_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_safe_haven_defender_traits_writeable();
	c_player_traits const* get_safe_haven_defender_traits() const;
	void set_safe_haven_defender_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_last_human_traits_writeable();
	c_player_traits const* get_last_human_traits() const;
	void set_last_human_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_infection_variant_flags, byte_flags, k_infection_variant_flags> m_variant_flags;
	c_enum<e_infection_safe_havens_settings, char, _infection_safe_havens_off, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, char, _infection_next_zombie_winner, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, char, _infection_initial_zombie_count_25_percent, k_infection_initial_zombie_count_settings> m_initial_zombie_count;
	c_enum<long, short, 0, 120> m_safe_haven_movement_time;  // default: 30
	c_enum<long, char, -10, 10> m_zombie_kill_points;        // default: 1
	c_enum<long, char, -10, 10> m_infection_points;          // default: 0
	c_enum<long, char, -10, 10> m_safe_haven_arrival_points; // default: 0
	c_enum<long, char, -10, 10> m_suicide_points;            // default: -1
	c_enum<long, char, -10, 10> m_betrayal_points;           // default: -1
	c_enum<long, char, -10, 10> m_last_man_bonus_points;     // default: 0
	c_player_traits m_zombie_traits;
	c_player_traits m_first_zombie_traits;
	c_player_traits m_safe_haven_defender_traits;
	c_player_traits m_last_human_traits;

	byte m_pad1[4];
};
static_assert(sizeof(c_game_engine_infection_variant) == 0x250);

struct c_infection_engine : c_game_engine
{
public:
	//void dump_settings(s_file_reference* file) const;
};

struct s_infection_globals
{
	struct
	{
		word zombie_bitvector;
		word first_zombie_bitvector;
		long safe_haven_timer;
		word zombie_team_bitvector;
		word players_in_safe_haven_bitvector;
	} networked;

	struct
	{
		c_area_set<c_area, 12> safe_havens;
		c_static_array<long, 16> zombie_kills;
		c_static_array<long, 16> human_kills;
		bool emitted_last_man_standing_event;
	} local;
};
static_assert(sizeof(s_infection_globals) == 0x550);