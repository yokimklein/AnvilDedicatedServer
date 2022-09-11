#include "network_utilities.h"
#include "network_globals.h"
#include <timeapi.h>

void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version)
{
	*executable_type = 5;
	*executable_version = 11;
	*compatible_version = 11;
}

ulong network_get_time() // non-original name
{
	ulong time;
	if (network_time_locked)
		time = *g_network_locked_time;
	else
		time = timeGetTime();
	return time;
}