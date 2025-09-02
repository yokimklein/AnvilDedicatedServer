#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <game\player_configuration.h>
#include <game\game_engine_simulation.h>

enum e_statborg_entry
{
	_statborg_entry_none = -1,
	_statborg_entry_in_round_score,
	_statborg_entry_in_game_total_score,
	_statborg_entry_kills,
	_statborg_entry_deaths,
	_statborg_entry_suicides,
	_statborg_entry_betrayals,
	_statborg_entry_assists,
	_statborg_entry_rounds_won,

	_statborg_entry_infection_time_as_human,
	_statborg_entry_infection_infections,
	_statborg_entry_infection_zombie_kills,

	_statborg_entry_territories_owned = _statborg_entry_infection_time_as_human,
	_statborg_entry_territories_captures,
	_statborg_entry_territories_ousts,

	_statborg_entry_total_wp,
	_statborg_entry_kills_in_a_row,
	_statborg_entry_multiple_kills,
	_statborg_entry_total_kill_damage_dealt,

	_statborg_entry_unknown15,

	_statborg_entry_team_kills_in_a_row,

	_statborg_entry_unknown17,

	_statborg_entry_sniper_kills_in_a_row,
	_statborg_entry_shotgun_kills_in_a_row,
	_statborg_entry_sword_kills_in_a_row,
	_statborg_entry_vehicle_kills_in_a_row,
	_statborg_entry_team_kills_team,

	_statborg_entry_finalised,

	_statborg_entry_kill_damage_type,

	// one more entry here?

	k_statborg_indices_count
};

struct s_game_statborg_player
{
	c_static_array<word, k_statborg_indices_count> statistics;
	bool __unknown32;
	byte __unknown33;
};
static_assert(sizeof(s_game_statborg_player) == 0x34);

struct s_game_statborg_team
{
	c_static_array<word, 12> statistics;
	bool finalised;
};
static_assert(sizeof(s_game_statborg_team) == 0x1A);

class c_game_statborg
{
public:
	void player_added(datum_index player_index);
	void clear_player_stats(datum_index player_index);
	void adjust_team_stat(e_game_team team_index, long statistic, short unknown, long value);
	void stats_reset_for_round_switch();
	void player_indices_swapped(long absolute_index_a, long absolute_index_b);

	word __unknown0;
	word __unknown2;
	c_static_array<s_game_statborg_player, k_maximum_multiplayer_players> player;
	c_static_array<s_game_statborg_team, k_multiplayer_team_count> team;
	bool stats_started;
	dword __unknown418;
	dword __unknown41C;
	bool finalized;
};
static_assert(sizeof(c_game_statborg) == 0x424);

c_game_statborg* game_engine_get_statborg();