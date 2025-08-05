#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_entities.h>
#include <game\game_engine_event_definitions.h>

constexpr short k_maximum_multiplayer_players = 16;
constexpr short k_maximum_players = 16;
constexpr short k_maximum_machines = 17;

enum e_game_engine_queued_event_flags
{
	_game_engine_queued_event_valid_bit = 0,
	_game_engine_queued_event_played_bit,
	_game_engine_queued_event_long_delay_bit,

	k_game_engine_queued_event_flags_count
};

struct s_game_engine_queued_event
{
	dword received_game_time;
	c_flags<e_game_engine_queued_event_flags, byte, k_game_engine_queued_event_flags_count> flags;
	s_game_engine_event_data event_data;
};
static_assert(sizeof(s_game_engine_queued_event) == 0x30);

void game_engine_globals_set_statborg_gamestate_index(datum_index index);
datum_index game_engine_globals_get_statborg_gamestate_index();
void game_engine_globals_set_gamestate_index(datum_index index);
datum_index game_engine_globals_get_gamestate_index();
e_simulation_entity_type game_engine_globals_get_simulation_entity_type();
datum_index game_engine_globals_get_player_gamestate_index(short absolute_player_index);
void game_engine_globals_set_player_gamestate_index(short absolute_player_index, datum_index index);
void game_engine_event_build_player_response_list(struct s_game_engine_event_data const* game_engine_event, datum_index* player_response_list, long player_response_list_max_count, long* player_response_list_count_out);