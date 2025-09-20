#include "main_game.h"
#include "game\game.h"

REFERENCE_DECLARE(0x3F8E588, s_main_game_globals, main_game_globals);

bool main_game_change_in_progress()
{
	return main_game_globals.map_change_pending;
}

bool main_game_loaded_map()
{
	return main_game_globals.game_loaded_status == _game_loaded_status_map_loaded;
}

// non-original name, unique to HO and called in c_tag_resource_cache_controller::mark_necessary_resources
bool main_game_pending_mainmenu()
{
	if (game_options_valid() && game_is_ui_shell())
	{
		return true;
	}

	if (!main_game_globals.map_change_pending || main_game_globals.map_change_pending_unload)
	{
		return false;
	}

	return main_game_globals.pending_game_options.game_mode == _game_mode_mainmenu;
}
