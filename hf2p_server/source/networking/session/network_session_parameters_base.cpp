#include "network_session_parameters_base.h"

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