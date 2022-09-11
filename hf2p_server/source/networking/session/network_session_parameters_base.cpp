#include "network_session_parameters_base.h"
#include "network_session.h"
#include <stdio.h>

bool c_network_session_parameter_base::get_allowed()
{
	return this->m_state_flags & 1;
}

// FUNC TODO
const char* c_network_session_parameter_base::get_session_description()
{
	//if (this->m_session && this->m_session.get_session_id(nullptr))
	//	return managed_session_get_id_string(this->m_session.m_managed_session_index);
	//else
	return "UNKNOWN";
}

bool c_network_session_parameter_base::set_allowed()
{
	return this->m_session->established() && this->m_session->is_host();
}

// TODO - TEST THIS
long c_network_session_parameter_base::get_change_request_size()
{
	return this->get_change_request_payload_size();
}

// TODO - TEST THIS
bool c_network_session_parameter_base::handle_change_request(void const* change_request, long change_request_size)
{
	bool success = this->handle_change_request_payload(change_request);
	if (success)
	{
		printf("MP/NET/SESSION,PARAMS: c_network_session_parameter_base::handle_change_request: [%s] parameter %d [%s] change request handled\n",
			this->get_session_description(),
			this->m_type,
			this->m_name);
	}
	return success;
}