#include "main_game.h"

bool main_game_change_in_progress()
{
	return *g_is_loading;
}