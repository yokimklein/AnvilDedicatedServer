#pragma once
#include "network_session_parameters_base.h"

struct s_network_session_parameter_matchmaking_messaging
{
	int unknown0;
	int unknown4;
	int unknown8;
};

struct c_network_session_parameter_matchmaking_messaging : c_network_session_parameter_base
{
	s_network_session_parameter_matchmaking_messaging m_data;
	s_network_session_parameter_matchmaking_messaging m_requested_data;
};