#include "network_session_parameters_game_variant.h"
#include <iostream>

c_game_variant* c_network_session_parameter_game_variant::get()
{
	if (this->get_allowed())
	{
		if (&this->m_data != nullptr)
			return &this->m_data.game_variant;
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_game_variant::get: [%s] can't get variant, unavailable\n",
			this->get_session_description());
	}
	return nullptr;
}