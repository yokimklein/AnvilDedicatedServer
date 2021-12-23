#pragma once
#include "network_session_parameters_base.h"

struct s_network_session_parameter_lobby_vote_set
{
	long : 32;
	long : 32;
};

struct c_network_session_parameter_lobby_vote_set : c_network_session_parameter_base
{
	s_network_session_parameter_lobby_vote_set m_data;
	s_network_session_parameter_lobby_vote_set m_requested_data;
};