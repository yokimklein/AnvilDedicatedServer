#include "network_session_parameters_game_variant.h"
#include <iostream>
#include <cseries\cseries_events.h>

c_game_variant* c_network_session_parameter_game_variant::get()
{
	if (get_allowed())
	{
		if (m_data.valid)
		{
			return &m_data.game_variant;
		}
	}
	else
	{
		event(_event_warning, "networking:session_parameters:chunked:game_variant: [%s] can't get variant, unavailable", get_session_description());
	}
	return NULL;
}