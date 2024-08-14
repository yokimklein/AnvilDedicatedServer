#include "game_engine_group.h"
#include <scenario\scenario.h>
#include <game\multiplayer_definitions.h>
#include <cache\cache_files.h>

game_engine_settings_definition* game_engine_settings_try_and_get()
{
	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr)
	{
		long wezr_index = universal->halo3_game_engine_settings.index;
		if (wezr_index != NONE)
		{
			return (game_engine_settings_definition*)tag_get(GAME_ENGINE_SETTINGS_DEFINITION_TAG, wezr_index);
		}
	}
	return nullptr;
}