#pragma once
#include <networking\session\network_session_parameters_base.h>
#include <cseries\cseries_events.h>
#include <iostream>

template <typename t_type>
class c_generic_network_session_parameter_data
{
protected:
	t_type m_data;
	t_type m_change_request_data;
};

template <typename t_type>
class c_generic_network_session_parameter :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<t_type>
{
public:
	inline const t_type* get() const
	{
		if (!get_allowed())
		{
			event(_event_warning, "networking:session_parameters: [%s] failed to get parameter %d [%s], data not available",
				get_session_description(),
				m_parameter_type,
				m_parameter_type_description);
			return NULL;
		}

		return &this->m_data;
	}

	inline bool set(t_type const* parameter)
	{
		ASSERT(parameter);
		event(_event_status, "networking:session_parameters: [%s] parameter type %d [%s] being set", get_session_description(), m_parameter_type, m_parameter_type_description);

		if (set_allowed())
		{
			if (csmemcmp(&this->m_data, parameter, sizeof(t_type)) || !get_allowed())
			{
				this->m_data = *parameter;
				set_update_required();
			}
			return true;
		}
		else
		{
			event(_event_warning, "networking:session_parameters: [%s] failed to set parameter %d [%s], access denied [%s]",
				get_session_description(),
				m_parameter_type,
				m_parameter_type_description,
				get_set_denied_reason());
			return false;
		}
	}
};