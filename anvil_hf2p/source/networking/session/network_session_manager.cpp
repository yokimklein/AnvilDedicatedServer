#include "network_session_manager.h"

c_network_session* c_network_session_manager::get_session(s_transport_secure_identifier const* secure_identifier)
{
	return DECLFUNC(ADDRESS_C_NETWORK_SESSION_MANAGER_GET_SESSION, c_network_session*, __thiscall, c_network_session_manager*, s_transport_secure_identifier const*)(this, secure_identifier);
}