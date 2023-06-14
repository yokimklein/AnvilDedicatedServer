#include "game_statborg.h"
#include <game\game_engine_util.h>
#include <memory\tls.h>
#include <game\game.h>
#include "assert.h"
#include <simulation\game_interface\simulation_game_action.h>
#include <game\game_results.h>

c_game_statborg* game_engine_get_statborg()
{
	if (current_game_engine())
		return &get_tls()->game_engine_globals->statborg;
	// TODO finish for survival
	// if (campaign_survival_enabled)
	//	return &get_tls()->survival_mode_globals->statborg;
	return nullptr;
}

void c_game_statborg::player_added(datum_index player_index)
{
	if (!game_is_predicted())
		this->clear_player_stats(player_index);
}

void c_game_statborg::clear_player_stats(datum_index player_index)
{
	short absolute_player_index = (word)player_index;
	assert(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	csmemset(&this->player[absolute_player_index], 0, sizeof(this->player[absolute_player_index]));
	simulation_action_game_statborg_update(absolute_player_index);
}

void c_game_statborg::adjust_team_stat(e_game_team team_index, long statistic, short unknown, long value)
{
	long team_statistic_plus_unknown = this->team[team_index].statistics[statistic] + unknown;
	word new_team_statistic = 0;
	if (team_statistic_plus_unknown <= -30000 || team_statistic_plus_unknown < 30000)
	{
		new_team_statistic = -30000;
		if (team_statistic_plus_unknown > -30000)
			new_team_statistic = this->team[team_index].statistics[statistic] + unknown;
	}
	else
	{
		new_team_statistic = 30000;
	}
	this->team[team_index].statistics[statistic] = new_team_statistic;
	long update_flag = _simulation_game_statborg_update_team0 + team_index;
	simulation_action_game_statborg_update(update_flag);
	if (value != -1)
		game_results_statistic_set(-1, team_index, value, this->team[team_index].statistics[statistic]);
}