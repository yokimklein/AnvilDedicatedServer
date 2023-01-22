#include "network_session_parameters_base.h"
#include "network_session.h"
#include <stdio.h>
#include "network_managed_session.h"
#include "assert.h"

bool c_network_session_parameter_base::get_allowed()
{
	return this->m_state_flags & 1;
}

const char* c_network_session_parameter_base::get_session_description()
{
	if (this->m_session != nullptr && this->m_session->get_session_id(nullptr))
		return managed_session_get_id_string(this->m_session->managed_session_index());
	else
		return "UNKNOWN";
}

bool c_network_session_parameter_base::set_allowed()
{
	return this->m_session->established() && this->m_session->is_host();
}

long c_network_session_parameter_base::get_change_request_size()
{
	return this->get_change_request_payload_size();
}

bool c_network_session_parameter_base::handle_change_request(void const* change_request, long change_request_size)
{
	assert(change_request);
	assert(change_request_size == get_change_request_size());
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

void c_network_session_parameter_base::set_update_required()
{
	assert(this->set_allowed());
	printf("MP/NET/SESSION,PARAMS: c_network_session_parameter_base::set_update_required: [%s] parameter %d [%s] marking dirty\n",
		this->get_session_description(),
		this->m_type,
		this->m_name);
	this->m_state_flags |= 1;
	memset(this->m_peers_updated, 0, k_network_maximum_machines_per_session);
	this->notify_set_update_required();
}

const char* c_network_session_parameter_base::get_set_denied_reason()
{
	const char* reason = "NONE";
	if (!this->set_allowed())
	{
		if (this->m_session->established())
		{
			if (!this->m_session->is_host())
				reason = "not host";
		}
		else
			reason = "not established";
	}
	return reason;
}