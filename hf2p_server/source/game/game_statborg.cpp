#include "game_statborg.h"
#include <game\game_engine_util.h>
#include <memory\tls.h>
#include <game\game.h>
#include "assert.h"
#include <simulation\game_interface\simulation_game_action.h>
#include <game\game_results.h>
#include <simulation\game_interface\simulation_game_statborg.h>

c_game_statborg* game_engine_get_statborg()
{
	if (current_game_engine())
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		return &game_engine_globals->statborg;
	}

	// TODO finish for survival
	//if (campaign_survival_enabled)
	//{
	//	TLS_DATA_GET_VALUE_REFERENCE(survival_mode_globals);
	//	return &survival_mode_globals->statborg;
	//}

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
	long update_flag = _simulation_statborg_update_team0 + team_index;
	simulation_action_game_statborg_update(update_flag);
	if (value != NONE)
		game_results_statistic_set(NONE, team_index, value, this->team[team_index].statistics[statistic]);
}

void c_game_statborg::stats_reset_for_round_switch()
{
	for (long i = 0; i < k_maximum_multiplayer_players; i++)
	{
		s_game_statborg_player* player_stats = &player[i];
		player_stats->kills_in_a_row = 0;
		player_stats->multiple_kills = 0;
		player_stats->total_kill_damage_dealt = 0;
		player_stats->unknown15 = 0;
		player_stats->team_kills_in_a_row = 0;
		player_stats->unknown17 = 0;
		player_stats->sniper_kills_in_a_row = 0;
		player_stats->shotgun_kills_in_a_row = 0;
		player_stats->sword_kills_in_a_row = 0;
		player_stats->vehicle_kills_in_a_row = 0;
		player_stats->kill_damage_type = 0;
		player_stats->__unknown32 = false;
		player_stats->__unknown33 = 0;
		player_stats->team_kills_team = -1;

		word in_round_score = player_stats->in_round_score;
		if (in_round_score != 0 && !player_stats->finalised)
		{
			player_stats->in_game_total_score += in_round_score;
			game_results_statistic_set(i, _game_team_none, _statborg_entry_rounds_won, player_stats->in_game_total_score);
			if (game_is_finished())
				player_stats->finalised = true;
			else
				player_stats->in_round_score = 0;
			simulation_action_game_statborg_update(_simulation_statborg_update_player0 + i);
		}
	}
	for (long i = 0; i < k_multiplayer_max_team_game_and_ffa_game_team_count; i++)
	{
		s_game_statborg_team* team_stats = &team[i];
		word in_round_score = team_stats->statistics[0];
		if (in_round_score != 0 && team_stats->finalised)
		{
			team_stats->statistics[1] += in_round_score;
			game_results_statistic_set(NONE, (e_game_team)i, _statborg_entry_rounds_won, team_stats->statistics[1]);
			if (game_is_finished())
				team_stats->finalised = true;
			else
				team_stats->statistics[0] = 0;
			simulation_action_game_statborg_update(_simulation_statborg_update_team0 + i);
		}
	}
	game_engine_scoring_notify_statborg_reset();
}

void c_game_statborg::player_indices_swapped(long absolute_index_a, long absolute_index_b)
{
	if (!game_is_predicted())
	{
		assert(absolute_index_a >= 0 && absolute_index_a < k_maximum_multiplayer_players);
		assert(absolute_index_b >= 0 && absolute_index_b < k_maximum_multiplayer_players);
		s_game_statborg_player player_temp;
		csmemcpy(&player_temp, &player[absolute_index_a], sizeof(s_game_statborg_player));
		csmemcpy(&player[absolute_index_a], &player[absolute_index_b], sizeof(s_game_statborg_player));
		csmemcpy(&player[absolute_index_b], &player_temp, sizeof(s_game_statborg_player));
		simulation_action_game_statborg_update(FLAG(absolute_index_a));
		simulation_action_game_statborg_update(FLAG(absolute_index_b));
	}
}