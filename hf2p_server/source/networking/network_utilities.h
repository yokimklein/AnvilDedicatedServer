#pragma once
#include <cseries\cseries.h>
#include <networking\network_globals.h>

c_network_session_manager* network_get_session_manager();
void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version);
ulong network_get_time();
bool network_initialized();