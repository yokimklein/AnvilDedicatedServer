#include "game_globals.h"
#include <memory\tls.h>

game_globals_storage* game_globals_get()
{
	s_thread_local_storage* tls = get_tls();

	return (tls && tls->game_globals) ? tls->game_globals : nullptr;
}

float game_difficulty_get_team_value(long value_type, long team)
{
	float return_value;
	// wrapper for userpurge function
	DECLFUNC(base_address(0xA53E0), float, __fastcall, long, long)(value_type, team);
	__asm movss return_value, xmm0;
	return return_value;
}

REFERENCE_DECLARE(base_address(0x103E788), s_globals_definition*, g_globals_definition);