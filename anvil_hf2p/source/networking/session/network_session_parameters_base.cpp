#include "network_session_parameters_base.h"
#include <networking\session\network_session.h>
#include <stdio.h>
#include <networking\session\network_managed_session.h>
#include <cseries\cseries_events.h>

bool c_network_session_parameter_base::get_allowed() const
{
	return TEST_BIT(m_flags, _network_session_parameter_valid_bit);
}

const char* c_network_session_parameter_base::get_session_description() const
{
	if (m_session && m_session->get_session_id(NULL))
	{
		return managed_session_get_id_string(m_session->managed_session_index());
	}
	else
	{
		return "UNKNOWN";
	}
}

bool c_network_session_parameter_base::set_allowed() const
{
	return m_session->established() && m_session->is_host();
}

long c_network_session_parameter_base::get_change_request_size() const
{
	return get_change_request_payload_size();
}

bool c_network_session_parameter_base::handle_change_request(void const* change_request, long change_request_size)
{
	ASSERT(change_request);
	ASSERT(change_request_size == get_change_request_size());

	bool success = handle_change_request_payload(change_request);
	if (success)
	{
		event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] change request handled",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description);
	}
	return success;
}

void c_network_session_parameter_base::set_update_required()
{
	ASSERT(set_allowed());

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] marking dirty",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);
	m_flags |= FLAG(_network_session_parameter_valid_bit);
	m_transmitted_peer_updates.clear();
	notify_set_update_required();
}

const char* c_network_session_parameter_base::get_set_denied_reason() const
{
	const char* reason = "NONE";
	if (!set_allowed())
	{
		if (m_session->established())
		{
			if (!m_session->is_host())
			{
				reason = "not host";
			}
		}
		else
		{
			reason = "not established";
		}
	}
	return reason;
}
