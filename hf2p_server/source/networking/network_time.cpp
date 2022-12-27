#include "network_time.h"

ulong network_time_get()
{
	if (network_time_locked)
		return *g_network_locked_time;
	else
		return network_time_get_exact();
}

ulong network_time_get_exact()
{
	return system_milliseconds();
}

ulong system_milliseconds()
{
	return timeGetTime();
}

ulong network_time_difference_msec(ulong start_time, ulong target_time)
{
	return target_time - start_time;
}

ulong network_time_since(ulong since)
{
	return network_time_difference_msec(since, network_time_get());
}