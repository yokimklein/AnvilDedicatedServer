#pragma once
#include <networking\session\network_session_parameters_base.h>

struct s_network_session_parameter_matchmaking_messaging
{
	long type;
	long display_cookie;
	long player_sequence_number;
};
static_assert(sizeof(s_network_session_parameter_matchmaking_messaging) == 0xC);

class c_network_session_parameter_matchmaking_messaging : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_matchmaking_messaging>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_messaging) == 0x48);