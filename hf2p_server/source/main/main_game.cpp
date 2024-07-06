#include "main_game.h"

REFERENCE_DECLARE(0x3F8E588, s_main_game_globals, main_game_globals);

bool main_game_change_in_progress()
{
	return main_game_globals.change_in_progress;
}