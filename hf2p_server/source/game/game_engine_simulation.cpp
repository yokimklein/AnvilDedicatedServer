#include "game_engine_simulation.h"
#include "assert.h"
#include "..\memory\tls.h"

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
	if (current_game_engine())
	{
		auto game_engine = current_game_engine();
		return game_engine->get_simulation_entity_type();
	}
	return k_simulation_entity_type_none;
}