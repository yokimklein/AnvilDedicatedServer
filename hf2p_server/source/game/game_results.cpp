#include "game_results.h"

void game_results_notify_player_indices_changed()
{
	FUNCTION_DEF(0xCDBA0, void, __cdecl, game_results_notify_player_indices_changed_call);
	game_results_notify_player_indices_changed_call();
}

void game_results_statistic_set(datum_index absolute_player_index, e_game_team team_index, long statistic, long value)
{
	// silly wacky function wrapper because this uses some kind of special calling convention that behaves like __fastcall but with the caller cleaning up the stack
	const auto game_results_statistic_set_call = (void (*)(datum_index absolute_player_index, e_game_team team_index, long statistic, long value))base_address(0xCE0B0);
	__asm
	{
		push value
		push statistic
		mov edx, team_index
		mov ecx, absolute_player_index
		call game_results_statistic_set_call
		add esp, 8
	}
}