#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\session\network_session_parameter_types.h>
#include <networking\transport\transport_security.h>

struct s_network_message_parameters_update
{
	s_transport_secure_identifier session_id;
	bool initial_update;
	long64 cleared_parameters;
	long64 updated_parameters;
	byte parameters[k_network_session_parameter_type_count][0x400];
};
static_assert(sizeof(s_network_message_parameters_update) == 0x8C28);

struct s_network_message_parameters_request
{
	s_transport_secure_identifier session_id;
	ulong64 change_request_parameters;
	byte parameters[k_network_session_parameter_type_count][0x400];
};
static_assert(sizeof(s_network_message_parameters_request) == 0x8C18);