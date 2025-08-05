#include "game_time.h"
#include <memory\tls.h>

long game_time_get()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals && game_time_globals->initialized);
	return game_time_globals->time;
}

real game_ticks_to_seconds(long ticks)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_length * ticks;
}

long game_ticks_to_seconds_ceil(long ticks)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return (long)game_ticks_to_seconds(ticks);
}

long game_seconds_integer_to_ticks(long seconds)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return seconds * game_time_globals->tick_rate;
}

real game_seconds_to_ticks_real(real seconds)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return seconds * game_time_globals->tick_rate;
}

real game_tick_length()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_length;
}

long game_tick_rate()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_rate;
}

long game_seconds_to_ticks_round(real seconds)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	real tick_rate = game_time_globals->tick_rate * seconds;
	return long(tick_rate + ((tick_rate < 0.0f ? -1.0f : 1.0f) / 2));
}