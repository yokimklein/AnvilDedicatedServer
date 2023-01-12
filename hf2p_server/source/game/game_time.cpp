#include "game_time.h"
#include "assert.h"
#include "..\memory\tls.h"

long game_time_get()
{
	auto game_time_globals = get_tls()->game_time_globals;
	assert(game_time_globals && game_time_globals->initialized);
	return game_time_globals->elapsed_ticks;
}

float game_ticks_to_seconds(long ticks)
{
	auto game_time_globals = get_tls()->game_time_globals;
	assert(game_time_globals);
	assert(game_time_globals->initialized);
	return game_time_globals->seconds_per_tick * ticks;
}