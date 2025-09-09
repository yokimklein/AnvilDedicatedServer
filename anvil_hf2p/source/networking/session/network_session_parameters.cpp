#include "network_session_parameters.h"
#include <networking\session\network_session.h>
#include <stdio.h>
#include <cseries\cseries_events.h>

void c_network_session_parameters::check_to_send_updates()
{
	ASSERT(m_session->established());
	ASSERT(m_session->is_host());
	const c_network_session_membership* membership = m_session->get_session_membership();
	ASSERT(membership);
	DECLFUNC(0x1A390, void, __thiscall, c_network_session_parameters*)(this);
}

void c_network_session_parameters::check_to_send_change_requests()
{
	ASSERT(m_session->established());
	ASSERT(!m_session->is_host());
	DECLFUNC(0x1A7B0, void, __thiscall, c_network_session_parameters*)(this);
}

void c_network_session_parameters::update()
{
	c_network_session* session = m_session;
	if (session->established())
	{
		if (session->is_host())
		{
			check_to_send_updates();
		}
		else
		{
			check_to_send_change_requests();
		}
	}
}

bool c_network_session_parameters::handle_change_request(bool leader_requested_change, s_network_message_parameters_request const* change_request)
{
	ASSERT(change_request);
	if (!m_session->compare_session_id(&change_request->session_id))
	{
		event(_event_error, "networking:session_parameters: [%s] session id mismatch handling change request", get_session_description());
		return true;
	}

	if (!m_session->is_host())
	{
		event(_event_error, "networking:session_parameters: [%s] not host, can't handle change request", get_session_description());
		return true;
	}

	byte* message_parameters = (byte*)&change_request->parameters;
	// log previously used 0x%08X
	event(_event_status, "networking:session_parameters: [%s] handling session parameter change request [requested 0x%llx]",
		get_session_description(),
		change_request->change_request_parameters);
	long total_size = 0;
	for (long i = 0; i < k_network_session_parameter_type_count; i++)
	{
		if (!TEST_BIT(change_request->change_request_parameters, i))
		{
			continue;
		}

		c_network_session_parameter_base* parameter = m_parameter_interfaces[i];
		long change_request_size = m_parameter_interfaces[i]->get_change_request_size();
		if (total_size + change_request_size > k_network_session_parameter_type_count * 0x400)
		{
			event(_event_error, "networking:session_parameters: [%s] parameter %d [%s] change request size out of bounds of payload size [0x%08X 0x%08X 0x%08X]",
				get_session_description(),
				i,
				parameter->get_parameter_type_description(),
				change_request_size,
				total_size,
				k_network_session_parameter_type_count * 0x400);
			return false;
		}

		// in the original code leader_requested_change is never used and permissions are never checked
		// this legacy H3 bug allows non-leader clients to mess with the m_session
		// CHECK PARAMETER PERMISSION LEVEL AGAINST IS_LEADER BEFORE HANDLING UPDATE
		bool has_permission_level = false;
		switch (parameter->get_change_request_access())
		{
			case _change_request_access_leader:
			{
				has_permission_level = leader_requested_change;
				break;
			}
			case _change_request_access_peer:
			{
				has_permission_level = true;
				break;
			}
			case k_parameter_change_request_access_none:
			case _change_request_access_default:
			default:
			{
				has_permission_level = false;
				break;
			}
		}
		if (!has_permission_level)
		{
			event(_event_warning, "networking:session_parameters: [%s] parameter [%s] lacked permission level to change [leader: %s] [level: %d]",
				get_session_description(),
				parameter->get_parameter_type_description(),
				leader_requested_change ? "true" : "false",
				parameter->get_change_request_access());
			return false;
		}
		
		if (parameter->handle_change_request(message_parameters, change_request_size))
		{
			message_parameters += change_request_size;
			total_size += change_request_size;
		}
		else
		{
			event(_event_error, "networking:session_parameters: [%s] parameter %d failed to handle change request", get_session_description(), i);
			return false;
		}
	}
	return true;
}

const char* c_network_session_parameters::get_session_description()
{
	if (m_session && m_session->get_session_id(NULL))
	{
		return m_session->get_id_string();
	}
	else
	{
		return "UNKNOWN";
	}
}