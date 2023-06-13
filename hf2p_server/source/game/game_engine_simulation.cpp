#include "game_engine_simulation.h"
#include "assert.h"
#include <memory\tls.h>
#include <game\game_engine_util.h>

// do these belong in game_globals.cpp?
void game_engine_globals_set_statborg_gamestate_index(datum_index index)
{
	assert(current_game_engine());
	auto tls = get_tls();
	tls->game_engine_globals->statborg_gamestate_index = index;
}

datum_index game_engine_globals_get_statborg_gamestate_index()
{
	if (current_game_engine())
	{
		auto tls = get_tls();
		return tls->game_engine_globals->statborg_gamestate_index;
	}
	return -1;
}

void game_engine_globals_set_gamestate_index(datum_index index)
{
	assert(current_game_engine());
	auto tls = get_tls();
	tls->game_engine_globals->gamestate_index = index;
}

datum_index game_engine_globals_get_gamestate_index()
{
	if (current_game_engine())
	{
		auto tls = get_tls();
		return tls->game_engine_globals->gamestate_index;
	}
	return -1;
}

e_simulation_entity_type game_engine_globals_get_simulation_entity_type()
{
	auto game_engine = current_game_engine();
	if (game_engine)
		return game_engine->get_simulation_entity_type();
	return k_simulation_entity_type_none;
}

datum_index game_engine_globals_get_player_gamestate_index(short absolute_player_index)
{
	assert(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	if (current_game_engine())
	{
		auto tls = get_tls();
		auto game_engine_globals = tls->game_engine_globals;
		return game_engine_globals->player_simulation_gamestate_indices[absolute_player_index];
	}
	return -1;
}

void game_engine_globals_set_player_gamestate_index(short absolute_player_index, datum_index index)
{
	assert(current_game_engine());
	assert(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	auto tls = get_tls();
	auto game_engine_globals = tls->game_engine_globals;
	game_engine_globals->player_simulation_gamestate_indices[absolute_player_index] = index;
}