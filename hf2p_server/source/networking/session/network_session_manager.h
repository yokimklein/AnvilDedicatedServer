#pragma once
#include "..\transport\transport_security.h"

class c_network_session;
class c_network_session_manager
{
public:
	c_network_session* get_session(s_transport_secure_identifier const* secure_identifier);

	c_network_session* session[3]; // desired state, transitory state, actual state
};
static_assert(sizeof(c_network_session_manager) == 0xC);