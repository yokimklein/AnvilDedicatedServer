#include "network_session_parameter_session.h"
#include "..\..\simulation\simulation.h"
#include <iostream>
#include "..\..\dllmain.h"

long c_network_session_parameter_session_size::get_max_player_count()
{
	long max_player_count = k_network_maximum_players_per_session;
	if (this->get_allowed())
		max_player_count = this->m_data.maximum_player_count;
	else // TODO - ASSERTS
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_session_size::get_max_player_count: [%s] failed to get max player count, unavailable\n",
		this->get_session_description());

	return max_player_count;
}

bool c_network_session_parameter_session_size::set_max_player_count(long player_count)
{
	bool(__thiscall* set_max_player_count)(c_network_session_parameter_session_size* session_size_parameter, long player_count) = reinterpret_cast<decltype(set_max_player_count)>(module_base + 0x2D670);
	return set_max_player_count(this, player_count);
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	bool(__thiscall* set)(c_network_session_parameter_session_mode * thisptr, e_network_session_mode session_mode) = reinterpret_cast<decltype(set)>(module_base + 0x2D820);
	return set(this, session_mode);
}