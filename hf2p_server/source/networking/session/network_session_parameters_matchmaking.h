#pragma once
#include "network_session_parameters_base.h"

struct s_network_session_parameter_matchmaking_messaging
{
	long type;
	long display_cookie;
	long player_sequence_number;
};
static_assert(sizeof(s_network_session_parameter_matchmaking_messaging) == 0xC);

class c_network_session_parameter_matchmaking_messaging : public c_network_session_parameter_base
{
	s_network_session_parameter_matchmaking_messaging m_data;
	s_network_session_parameter_matchmaking_messaging m_requested_data;
};