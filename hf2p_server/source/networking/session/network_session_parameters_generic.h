#pragma once
#include "network_session_parameters_base.h"
#include <iostream>
#include "assert.h"

template <typename t_type>
class c_generic_network_session_parameter_data
{
public: // TEMPORARY - TODO, SET THIS BACK TO PROTECTED
	t_type m_data;
	t_type m_change_request_data;
};

template <typename t_type>
class c_generic_network_session_parameter : public c_network_session_parameter_base, c_generic_network_session_parameter_data<t_type>
{
public:
	t_type* get()
	{
		if (this->get_allowed())
			return &this->m_data;
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: %s: [%s] failed to get parameter %d [%s], data not available\n", __FUNCTION__, this->get_session_description(), this->m_type, this->m_name);
		return nullptr;
	}

	bool set(t_type* parameter)
	{
		assert(parameter);
		printf("MP/NET/SESSION,PARAMS: %s: [%s] parameter type %d [%s] being set\n", __FUNCTION__, this->get_session_description(), this->m_type, this->m_name);
		if (this->set_allowed())
		{
			if (csmemcmp(&this->m_data, parameter, sizeof(t_type)) || !this->get_allowed())
			{
				this->m_data = *parameter;
				this->set_update_required();
			}
			return true;
		}
		else
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: %s: [%s] failed to set parameter %d [%s], access denied [%s]\n",
				__FUNCTION__,
				this->get_session_description(),
				this->m_type,
				this->m_name,
				this->get_set_denied_reason());
			return false;
		}
	}
};