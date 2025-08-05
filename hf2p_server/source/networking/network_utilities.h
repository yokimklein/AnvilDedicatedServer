#pragma once
#include <cseries\cseries.h>

class c_network_session_manager;
c_network_session_manager* network_get_session_manager();
void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version);
bool network_initialized();