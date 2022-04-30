#include "network_utilities.h"
#include "network_globals.h"

void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version)
{
	*executable_type = 5;
	*executable_version = 11;
	*compatible_version = 11;
}

uint32_t network_get_time() // non-original name
{
	uint32_t time;
	if (network_time_locked)
		time = *g_network_locked_time;
	else
		time = timeGetTime();
	return time;
}