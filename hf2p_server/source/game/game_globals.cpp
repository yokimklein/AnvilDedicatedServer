#include "game_globals.h"
#include <memory\tls.h>

game_globals_storage* game_globals_get()
{
	s_thread_local_storage* tls = get_tls();

	return (tls && tls->game_globals) ? tls->game_globals : nullptr;
}

REFERENCE_DECLARE(base_address(0x103E788), s_globals_definition*, g_globals_definition);