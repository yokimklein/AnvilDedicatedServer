#pragma once
#include "..\cseries\cseries.h"
#include "game_engine_scoring.h"
#include "game_engine_spawn_influencer.h"
#include "game_statborg.h"
#include "game_engine_candy_monitor.h"
#include "game_engine_teleporters.h"
#include "..\scenario\scenario_map_variant.h"
#include "game_engine_ctf.h"
#include "game_engine_slayer.h"
#include "game_engine_oddball.h"
#include "game_engine_king.h"
#include "game_engine_sandbox.h"
#include "game_engine_vip.h"
#include "game_engine_juggernaut.h"
#include "game_engine_territories.h"
#include "game_engine_assault.h"
#include "game_engine_infection.h"
#include "game_engine_simulation.h"
#include "..\dllmain.h"

enum e_engine_variant : long
{
	_engine_variant_base,
	_engine_variant_ctf,
	_engine_variant_slayer,
	_engine_variant_oddball,
	_engine_variant_king,
	_engine_variant_sandbox,
	_engine_variant_vip,
	_engine_variant_juggernaut,
	_engine_variant_territories,
	_engine_variant_assault,
	_engine_variant_infection,
};

enum e_game_engine_round_condition
{
	_game_engine_round_condition_unknown0,
	_game_engine_round_condition_unknown1,
	_game_engine_round_condition_unknown2,
	_game_engine_round_condition_unknown3,
	_game_engine_round_condition_unknown4,
	_game_engine_round_condition_unknown5,
	_game_engine_round_condition_unknown6,
	_game_engine_round_condition_unknown7,

	k_number_of_game_engine_round_conditions = 8
};

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
	short current_state;
	short round_index;
	dword round_timer;
	c_flags<e_game_engine_round_condition, byte, k_number_of_game_engine_round_conditions> round_condition_flags;
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
	byte __data10498[0x104];
	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	dword __unknown13D9C;
	dword desired_state;
	bool game_finished;
	dword __unknown13DA8;
	dword __unknown13DAC;
	c_enum<e_game_engine_type, long, k_game_engine_type_count> game_engine_index;
	long multiplayer_weapon_count;
	c_static_array<s_multiplayer_weapon_tracker, 8> multiplayer_weapons;
	c_area_set<c_teleporter_area, 32> teleporters;
	long current_event_identifier;
	c_static_array<s_game_engine_queued_event, 64> event_queue;
	byte __data1584C[0xC];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);

void game_engine_attach_to_simulation();
void game_engine_player_added(datum_index absolute_player_index);
long game_engine_round_time_get();
void game_engine_update_round_conditions();

static c_game_engine** game_engines = (c_game_engine**)(module_base + 0xF01EC0);