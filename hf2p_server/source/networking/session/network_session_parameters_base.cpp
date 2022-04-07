#include "network_session_parameters_base.h"

bool c_network_session_parameter_base::get_allowed()
{
	return this->m_state_flags & 1;
}