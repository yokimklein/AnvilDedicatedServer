#include "game_time.h"
#include "assert.h"
#include <memory\tls.h>

long game_time_get()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	assert(game_time_globals && game_time_globals->initialized);
	return game_time_globals->elapsed_ticks;
}

real game_ticks_to_seconds(long ticks)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	assert(game_time_globals);
	assert(game_time_globals->initialized);
	return game_time_globals->seconds_per_tick * ticks;
}

long game_ticks_to_seconds_ceil(long ticks)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	assert(game_time_globals);
	assert(game_time_globals->initialized);
	return (long)game_ticks_to_seconds(ticks);
}

long game_seconds_integer_to_ticks(long seconds)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	assert(game_time_globals);
	assert(game_time_globals->initialized);
	return seconds * game_time_globals->tick_rate;
}