#include "scenario.h"
#include <game\game_globals.h>
#include <game\multiplayer_definitions.h>
#include <cache\cache_files.h>

s_game_globals* scenario_get_game_globals()
{
	ASSERT(global_game_globals);
	return global_game_globals;
}

s_game_globals* scenario_try_and_get_game_globals()
{
	if (!global_game_globals)
	{
		return NULL;
	}
	return global_game_globals;
}

s_multiplayer_universal_globals_definition* scenario_multiplayer_globals_try_and_get_universal_data()
{
	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	multiplayer_globals_definition* multiplayer_globals = NULL;
	if (game_globals)
	{
		multiplayer_globals = (multiplayer_globals_definition*)tag_get(MULTIPLAYER_GLOBALS_TAG, game_globals->multiplayer_globals.index);
	}
	if (multiplayer_globals)
	{
		return multiplayer_globals_try_and_get_universal_data(multiplayer_globals);
	}
	return NULL;
}

s_multiplayer_runtime_globals_definition* scenario_multiplayer_globals_try_and_get_runtime_data()
{
	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	multiplayer_globals_definition* multiplayer_globals = NULL;
	if (game_globals)
	{
		multiplayer_globals = (multiplayer_globals_definition*)tag_get(MULTIPLAYER_GLOBALS_TAG, game_globals->multiplayer_globals.index);
	}
	if (multiplayer_globals)
	{
		return &multiplayer_globals->runtime[0];
	}
	return NULL;
}