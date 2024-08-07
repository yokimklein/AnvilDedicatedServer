#include "main_game.h"

REFERENCE_DECLARE(0x3F8E588, s_main_game_globals, main_game_globals);

bool main_game_change_in_progress()
{
	return main_game_globals.change_in_progress;
}

bool main_game_loaded_map()
{
	return main_game_globals.game_loaded_status == _game_loaded_status_map_loaded;
}