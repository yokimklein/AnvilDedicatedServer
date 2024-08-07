#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_scoring.h>
#include <game\game_engine_spawn_influencer.h>
#include <game\game_statborg.h>
#include <game\game_engine_candy_monitor.h>
#include <game\game_engine_teleporters.h>
#include <scenario\scenario_map_variant.h>
#include <game\game_engine_ctf.h>
#include <game\game_engine_slayer.h>
#include <game\game_engine_oddball.h>
#include <game\game_engine_king.h>
#include <game\game_engine_sandbox.h>
#include <game\game_engine_vip.h>
#include <game\game_engine_juggernaut.h>
#include <game\game_engine_territories.h>
#include <game\game_engine_assault.h>
#include <game\game_engine_infection.h>
#include <game\game_engine_simulation.h>
#include <game\game_engine_variant.h>

enum e_game_engine_round_condition
{
	_game_engine_round_condition_unknown0,
	_game_engine_round_condition_unknown1,
	_game_engine_round_condition_welcome, // HUD welcome popup
	_game_engine_round_condition_waiting_for_players, // pre-game scoreboard overlay, followed by gametype popup in kill roster
	_game_engine_round_condition_spawn_players, // countdown & player spawn
	_game_engine_round_condition_match_introduction, // popup widgets
	_game_engine_round_condition_unknown6,
	_game_engine_round_condition_unknown7,

	k_game_engine_round_condition_count = 8
};

enum e_game_engine_end_condition
{
	_game_engine_end_condition_end_default = 0,
	_game_engine_end_condition_game_end_scripting,
	_game_engine_end_condition_should_end_not_enough_living,
	_game_engine_end_condition_should_end_all_dead,
	_game_engine_end_condition_should_end_current_engine_override,
	_game_engine_end_condition_round_end_team_scoring,
	_game_engine_end_condition_round_end_scoring,
	_game_engine_end_condition_game_end_rounds,
	_game_engine_end_condition_game_end_rounds_team_early_victory,
	_game_engine_end_condition_game_end_rounds_early_victory,
	_game_engine_end_condition_game_end_rounds_exceeded,
	_game_engine_end_condition_round_end_time_ran_out,
	_game_engine_end_condition_game_end_external,
	_game_engine_end_condition_game_end_invalid_team_mapping,
	_game_engine_end_condition_game_end_due_to_automation,
	_game_engine_end_condition_game_end_exceeded_maximum_rounds,
	_game_engine_end_condition_round_end_juggernaut_left,
	_game_engine_end_condition_round_end_juggernaut_unknown1,
	_game_engine_end_condition_round_end_juggernaut_unknown2,
	_game_engine_end_condition_round_end_infection,
	_game_engine_end_condition_round_end_ctf_sides,
	_game_engine_end_condition_round_end_editor_reset,
	_game_engine_end_condition_round_end_editor_change_mode,
	_game_engine_end_condition_round_end_vip_vip_killed,

	k_game_engine_end_condition_count
};

enum e_game_engine_state
{
	_game_engine_state_game_over = 0,
	_game_engine_state_round_progress,
	_game_engine_state_round_over,
	_game_engine_state_waiting_for_next_round,

	k_game_engine_state_count
};

//.data:0189ECF0 ; char const* k_game_engine_end_conditions[k_game_engine_end_condition_count]

struct s_player_waypoint_data
{
	byte __data[0x1C];
};
static_assert(sizeof(s_player_waypoint_data) == 0x1C);

struct s_multiplayer_weapon_tracker
{
	dword weapon_index;
	word multiplayer_weapon_identifier;
	dword owner_unit_index;
	dword owner_player_index;
};
static_assert(sizeof(s_multiplayer_weapon_tracker) == 0x10);

struct s_game_engine_globals
{
	dword_flags flags;
	word_flags valid_team_mask;
	word_flags initial_teams;
	word_flags valid_designators;
	word_flags valid_teams;
	word_flags active_teams;
	word game_simulation;
	c_static_array<short, 9> team_designator_to_team_index;
	c_static_array<byte, 8> team_lives_per_round;
	short __unknown2A;
	dword gamestate_index;
	datum_index statborg_gamestate_index;
	c_static_array<datum_index, 16> player_simulation_gamestate_indices;
	byte __data74[0x4];
	c_map_variant map_variant;
	c_enum<e_game_engine_state, short, _game_engine_state_game_over, k_game_engine_state_count> current_state;
	short round_index;
	dword round_timer;
	c_flags<e_game_engine_round_condition, byte, k_game_engine_round_condition_count> round_condition_flags;
	s_game_engine_score_list score_list;

	union
	{
		s_ctf_globals ctf_globals;
		//s_slayer_globals slayer_globals;
		s_oddball_globals oddball_globals;
		s_king_globals king_globals;
		s_sandbox_globals sandbox_globals;
		s_vip_globals vip_globals;
		s_juggernaut_globals juggernaut_globals;
		s_territories_globals territories_globals;
		s_assault_globals assault_globals;
		s_infection_globals infection_globals;

		// probably contains more bytes than it should
		byte globals_storage[0x1800];
	};

	word timer;
	word __unknownF992;
	dword game_variant_round_time_limit_ticks_per_second;
	real fade_to_black_amount[4];
	byte fade_to_black_active_user_mask;
	ushort game_over_timer;
	ushort next_shot_id;
	c_static_array<s_dead_player_info, 64> spawn_influencers;
	c_game_statborg statborg;
	long __unknown102D4;
	c_static_array<s_player_waypoint_data, 16> player_waypoints;
	c_static_array<char[16], 16> player_data10498;
	long __unknown10598;
	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	dword __unknown13D9C;
	c_enum<e_game_engine_state, long, _game_engine_state_game_over, k_game_engine_state_count> desired_state;
	bool game_finished;
	dword __unknown13DA8;
	dword __unknown13DAC;
	c_enum<e_game_engine_type, long, _game_engine_type_none, k_game_engine_type_count> game_engine_index;
	long multiplayer_weapon_count;
	c_static_array<s_multiplayer_weapon_tracker, 8> multiplayer_weapons;
	c_area_set<c_teleporter_area, 32> teleporters;
	long current_event_identifier;
	c_static_array<s_game_engine_queued_event, 64> event_queue;
	byte __data1584C[0xC];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);
static_assert(0x2C == OFFSETOF(s_game_engine_globals, gamestate_index));
static_assert(0x30 == OFFSETOF(s_game_engine_globals, statborg_gamestate_index));
static_assert(0x34 == OFFSETOF(s_game_engine_globals, player_simulation_gamestate_indices));
static_assert(0x78 == OFFSETOF(s_game_engine_globals, map_variant));
static_assert(0xE108 == OFFSETOF(s_game_engine_globals, current_state));
static_assert(0xE10C == OFFSETOF(s_game_engine_globals, round_timer));
static_assert(0xE110 == OFFSETOF(s_game_engine_globals, round_condition_flags));
static_assert(0xF994 == OFFSETOF(s_game_engine_globals, game_variant_round_time_limit_ticks_per_second));
static_assert(0xF998 == OFFSETOF(s_game_engine_globals, fade_to_black_amount));
static_assert(0xF9A8 == OFFSETOF(s_game_engine_globals, fade_to_black_active_user_mask));
static_assert(0xF9AA == OFFSETOF(s_game_engine_globals, game_over_timer));
static_assert(0xFEB0 == OFFSETOF(s_game_engine_globals, statborg));
static_assert(0x13DA0 == OFFSETOF(s_game_engine_globals, desired_state));
static_assert(0x13DB0 == OFFSETOF(s_game_engine_globals, game_engine_index));

void game_engine_attach_to_simulation();
void game_engine_detach_from_simulation_gracefully();
void __fastcall game_engine_player_added(datum_index absolute_player_index);
long game_engine_round_time_get();
void game_engine_update_round_conditions();
bool game_engine_round_condition_test(e_game_engine_round_condition condition);
void game_engine_update_time();
bool game_engine_in_round();
bool __fastcall game_engine_player_is_playing(datum_index player_index);
void __fastcall game_engine_player_set_spawn_timer(datum_index player_index, long timer_ticks);
void __fastcall game_engine_get_multiplayer_string(string_id id, c_static_wchar_string<1024>* out_multiplayer_string);

extern c_game_engine* (&game_engines)[k_game_engine_type_count];