#include "network_session_parameters_session.h"
#include <simulation\simulation.h>
#include <iostream>

long c_network_session_parameter_session_size::get_max_player_count()
{
	long max_player_count = k_network_maximum_players_per_session;
	if (this->get_allowed())
		max_player_count = this->m_data.maximum_player_count;
	else
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_session_size::get_max_player_count: [%s] failed to get max player count, unavailable\n",
		this->get_session_description());

	return max_player_count;
}

bool c_network_session_parameter_session_size::set_max_player_count(long player_count)
{
	return DECLFUNC(0x2D670, bool, __thiscall, c_network_session_parameter_session_size*, long)(this, player_count);
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	return DECLFUNC(0x2D820, bool, __thiscall, c_network_session_parameter_session_mode*, e_network_session_mode)(this, session_mode);
}

bool c_network_session_parameter_lobby_vote_set::set(s_network_session_parameter_lobby_vote_set* vote_set)
{
	if (memcmp(&m_data.vote_options, &vote_set->vote_options, sizeof(s_network_session_parameter_lobby_vote_set::vote_options)) != 0
		|| m_data.winning_vote_index != vote_set->winning_vote_index || !this->get_allowed())
	{
		m_data.vote_options = vote_set->vote_options;
		m_data.winning_vote_index = vote_set->winning_vote_index;
		this->set_update_required();
	}
	return true;
}

void c_network_session_parameter_lobby_vote_set::get(s_network_session_parameter_lobby_vote_set* output)
{
	output->vote_options = m_data.vote_options;
	output->winning_vote_index = m_data.winning_vote_index;
}