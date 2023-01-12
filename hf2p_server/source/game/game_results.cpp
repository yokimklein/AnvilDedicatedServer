#include "game_results.h"
#include "..\dllmain.h"

void game_results_notify_player_indices_changed()
{
	FUNCTION_DEF(0xCDBA0, void, __cdecl, game_results_notify_player_indices_changed_call);
	game_results_notify_player_indices_changed_call();
}

void game_results_statistic_set(datum_index absolute_player_index, e_game_team team_index, long statistic, long value)
{
	FUNCTION_DEF(0xCE0B0, void, __fastcall, game_results_statistic_set_call, datum_index absolute_player_index, e_game_team team_index, long statistic, long value);
	game_results_statistic_set_call(absolute_player_index, team_index, statistic, value);
}