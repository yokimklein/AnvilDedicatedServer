#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_entities.h>

constexpr short k_maximum_multiplayer_players = 16;

struct s_game_engine_queued_event
{
	dword __time0;
	byte_flags flags;
	byte __data5[0x3];
	byte __data8[0x28];
};
static_assert(sizeof(s_game_engine_queued_event) == 0x30);

void game_engine_globals_set_statborg_gamestate_index(datum_index index);
datum_index game_engine_globals_get_statborg_gamestate_index();
void game_engine_globals_set_gamestate_index(datum_index index);
datum_index game_engine_globals_get_gamestate_index();
e_simulation_entity_type game_engine_globals_get_simulation_entity_type();
datum_index game_engine_globals_get_player_gamestate_index(short absolute_player_index);
void game_engine_globals_set_player_gamestate_index(short absolute_player_index, datum_index index);