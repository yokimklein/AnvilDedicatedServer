#include "scenario.h"
#include <game\game_globals.h>
#include <cassert>

s_game_globals* scenario_get_game_globals()
{
	assert(global_game_globals);
	return global_game_globals;
}