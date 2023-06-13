#include "game_globals.h"
#include <memory\tls.h>

game_globals_storage* game_globals_get()
{
	s_thread_local_storage* tls = get_tls();

	return (tls && tls->game_globals) ? tls->game_globals : nullptr;
}