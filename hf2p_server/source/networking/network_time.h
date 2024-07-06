#pragma once
#include <cseries\cseries.h>

class c_network_session_manager;
struct s_network_time_globals
{
	bool locked;
	dword time;
	c_network_session_manager* session_manager;
};
static_assert(sizeof(s_network_time_globals) == 0xC);

extern s_network_time_globals& network_time_globals;

ulong network_time_get();
ulong network_time_get_exact();
ulong system_milliseconds();
ulong network_time_difference_msec(ulong start_time, ulong target_time);
ulong network_time_since(ulong since);