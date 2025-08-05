#pragma once
#include <cseries\cseries.h>

enum e_network_time_synchronize_stage
{
	_network_time_synchronize_stage_initiate = 0,
	_network_time_synchronize_stage_reply,

	k_network_time_synchronize_stage_count,
};

class c_network_session_manager;
struct s_network_time_globals
{
	bool time_locked;
	dword time_locked_timestamp;
	c_network_session_manager* session_manager;
};
static_assert(sizeof(s_network_time_globals) == 0xC);

extern s_network_time_globals& network_time_globals;

ulong network_time_get_exact();
ulong system_milliseconds();
ulong network_time_difference_msec(ulong start_time, ulong target_time);
ulong network_time_since(ulong since);

inline ulong network_time_get()
{
	if (network_time_globals.time_locked)
	{
		return network_time_globals.time_locked_timestamp;
	}
	else
	{
		return network_time_get_exact();
	}
}
