#pragma once
#include <cseries\cseries.h>

enum e_game_engine_event_type
{
	_game_engine_event_type_general,
	_game_engine_event_type_flavor,
	_game_engine_event_type_slayer,
	_game_engine_event_type_ctf,
	_game_engine_event_type_oddball,
	_game_engine_event_type_unused,
	_game_engine_event_type_koth,
	_game_engine_event_type_vip,
	_game_engine_event_type_juggernaut,
	_game_engine_event_type_territories,
	_game_engine_event_type_assault,
	_game_engine_event_type_infection,
	_game_engine_event_type_survival,
	_game_engine_event_type_earn_wp, // saber scoring

	k_number_of_game_engine_event_types
};

struct s_game_engine_event_data
{
	c_enum<e_game_engine_event_type, long, k_number_of_game_engine_event_types> event_type;
	string_id type;
	long identifier;
	long audience_player_index;
	long cause_player_index;
	long cause_team_index;
	long effect_player_index;
	long effect_team_index;
	long event_quantity;
	short dummy_or_territories;
};
static_assert(sizeof(s_game_engine_event_data) == 0x28);

void game_engine_initialize_event(e_game_engine_event_type event_type, string_id event_name, s_game_engine_event_data* event_data);