#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

enum e_multiplayer_event_type
{
	_multiplayer_event_type_general = 0,
	_multiplayer_event_type_flavor,
	_multiplayer_event_type_slayer,
	_multiplayer_event_type_capture_the_flag,
	_multiplayer_event_type_oddball,
	_multiplayer_event_type_dummy,
	_multiplayer_event_type_king_of_the_hill,
	_multiplayer_event_type_vip,
	_multiplayer_event_type_territories,
	_multiplayer_event_type_juggernaut,
	_multiplayer_event_type_assault,
	_multiplayer_event_type_infection,
	_multiplayer_event_type_survival,
	_multiplayer_event_type_earn_wp, // saber scoring

	k_multiplayer_event_type_count
};

struct s_territories_event_data
{
	short territory_index;
};
static_assert(sizeof(s_territories_event_data) == 0x2);

struct s_objective_game_role_change_event_data
{
	long objective_game_role_index;
};
static_assert(sizeof(s_objective_game_role_change_event_data) == 0x4);

struct s_game_engine_event_data
{
	c_enum<e_multiplayer_event_type, long, _multiplayer_event_type_general, k_multiplayer_event_type_count> event_type;
	string_id event_name;
	long event_identifier;
	datum_index audience_player_index;
	datum_index cause_player_index;
	long cause_team_index;
	datum_index effect_player_index;
	long effect_team_index;
	long event_quantity;

	union
	{
		s_territories_event_data territories_event_data;
		s_objective_game_role_change_event_data objective_game_role_event_data;
	};
};
static_assert(sizeof(s_game_engine_event_data) == 0x28);

void game_engine_initialize_event(e_multiplayer_event_type event_type, string_id event_name, s_game_engine_event_data* event_data);