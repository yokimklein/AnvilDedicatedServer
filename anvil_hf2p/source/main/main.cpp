#include "main.h"
#include "cseries\cseries.h"
#include "multithreading\threads.h"
#include "game\game.h"
#include "main\console.h"
#include "shell\shell.h"
#include <cache\cache_file_tag_resource_runtime.h>

REFERENCE_DECLARE(0x3EE1E48, _main_globals, main_globals);

bool debug_console_pauses_game = true;

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

void main_loop_pregame_disable(bool disable)
{
	if (is_main_thread())
	{
		if (disable)
		{
			main_globals.main_loop_pregame_entered++;
		}
		else
		{
			main_globals.main_loop_pregame_entered--;
		}
	}
}

c_tag_resources_game_lock::c_tag_resources_game_lock()
{
	tag_resources_lock_game(m_resource_key);
}

c_tag_resources_game_lock::~c_tag_resources_game_lock()
{
	tag_resources_unlock_game(m_resource_key);
}

bool main_time_halted()
{
	bool time_halted = shell_application_is_paused();

	if ((debug_console_pauses_game && debugging_system_has_focus()) && (!game_in_progress() || !game_has_nonlocal_players() || game_is_authoritative_playback()))
	{
		time_halted = true;
	}

	if (main_globals.reset_zone_resources)
	{
		time_halted = true;
	}

	return time_halted;
}