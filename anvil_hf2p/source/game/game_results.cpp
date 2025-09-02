#include "game_results.h"

REFERENCE_DECLARE(0x3FC2750, c_game_results, g_current_game_results);

void __cdecl game_results_notify_player_indices_changed()
{
	INVOKE(0xCDBA0, game_results_notify_player_indices_changed);
}

void game_results_statistic_set(long absolute_player_index, e_game_team team_index, long statistic, long value)
{
	// silly wacky function wrapper because this uses some kind of special calling convention that behaves like __fastcall but with the caller cleaning up the stack
	static void* game_results_statistic_set_call = base_address<void*>(0xCE0B0);
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

#pragma runtime_checks("", off)
void __fastcall game_results_statistic_increment(long player_absolute_index, e_game_team team_index, long statistic, long value)
{
	INVOKE(0xCDFB0, game_results_statistic_increment, player_absolute_index, team_index, statistic, value);
	__asm add esp, 8; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

bool game_results_get_game_finalized()
{
	if (!g_current_game_results.finish_reason)
	{
		return false;
	}
	if (!g_current_game_results.initialized)
	{
		return false;
	}
	return true;
}

c_game_results* game_results_get_final_results()
{
	if (game_results_get_game_finalized())
	{
		return &g_current_game_results;
	}
	return NULL;
}
