#include "main.h"
#include <cseries\cseries.h>

REFERENCE_DECLARE(0x3EE1E48, _main_globals, main_globals);

bool main_is_in_main_loop_pregame()
{
	return main_globals.main_loop_pregame_entered != 0;
}

long __cdecl internal_halt_render_thread_and_lock_resources(const char* file_name, long line_number)
{
	return INVOKE(0x94CB0, internal_halt_render_thread_and_lock_resources, file_name, line_number);
}

void __fastcall main_exit_game()
{
	main_globals.exit_game = true;
}
