#pragma once
#include "..\messages\network_message_type_collection.h"

class c_network_session;
class c_network_session_manager
{
public:
	c_network_session* get_session(s_transport_secure_identifier const* secure_identifier);

	c_network_session* session[3];
	//
	//
};

