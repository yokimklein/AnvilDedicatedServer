#pragma once
#include <networking\network_globals.h>

class c_network_session;
struct s_transport_secure_identifier;
class c_network_session_manager
{
public:
	c_network_session* get_session(s_transport_secure_identifier const* secure_identifier);

	// local (hosted), group (client syslink), squad (matchmaking)
	c_network_session* session[k_network_maximum_sessions]; // desired state, transitory state, actual state? hosted session, client session, unknown session
};
static_assert(sizeof(c_network_session_manager) == 0xC);