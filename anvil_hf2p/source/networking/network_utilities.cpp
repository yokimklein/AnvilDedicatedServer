#include "network_utilities.h"
#include <networking\session\network_session_manager.h>

void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version)
{
	ASSERT(executable_type);
	ASSERT(executable_version);
	ASSERT(compatible_version);

	*executable_type = 5;
	*executable_version = 11;
	*compatible_version = 11;
}

c_network_session_manager* network_get_session_manager()
{
	return g_network_session_manager;
}

bool network_initialized()
{
	return network_globals.initialized;
}