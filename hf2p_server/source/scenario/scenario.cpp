#include "scenario.h"
#include <game\game_globals.h>
#include <game\multiplayer_definitions.h>
#include <cassert>
#include <cache\cache_files.h>

s_game_globals* scenario_get_game_globals()
{
	assert(global_game_globals);
	return global_game_globals;
}

s_game_globals* scenario_try_and_get_game_globals()
{
	if (global_game_globals == nullptr)
		return nullptr;
	return global_game_globals;
}

s_multiplayer_universal_globals_definition* scenario_multiplayer_globals_try_and_get_universal_data()
{
	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	s_multiplayer_globals_definition* multiplayer_globals = nullptr;
	if (game_globals != nullptr)
	{
		multiplayer_globals = (s_multiplayer_globals_definition*)tag_get(MULTIPLAYER_GLOBALS_TAG, game_globals->multiplayer_globals.index);
	}
	if (multiplayer_globals != nullptr)
	{
		return multiplayer_globals_try_and_get_universal_data(multiplayer_globals);
	}
	return nullptr;
}