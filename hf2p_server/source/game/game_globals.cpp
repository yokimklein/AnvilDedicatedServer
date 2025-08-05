#include "game_globals.h"
#include <game\game.h>
#include <game\game_allegiance.h>
#include <memory\tls.h>
#include <scenario\scenario.h>

REFERENCE_DECLARE_ARRAY(0xD3D550, short, k_difficulty_value_indirection_table, k_game_difficulty_value_count);
REFERENCE_DECLARE(0x103E788, s_game_globals*, global_game_globals);

real game_difficulty_get_value_by_difficulty(short value_type, e_campaign_difficulty_level difficulty_level)
{
	ASSERT((value_type >= 0) && (value_type < k_game_difficulty_value_count));
	
	s_game_globals* game_globals = scenario_get_game_globals();
	if (game_globals)
	{
		s_game_globals_difficulty_information* difficulty_information = game_globals_get_difficulty_information(game_globals);
		if (difficulty_information)
		{
			if (difficulty_level <= _campaign_difficulty_level_easy)
			{
				difficulty_level = _campaign_difficulty_level_easy;
			}
			else if (difficulty_level >= _campaign_difficulty_level_legendary)
			{
				return difficulty_information->value[value_type][_campaign_difficulty_level_legendary];
			}
			return difficulty_information->value[value_type][difficulty_level];
		}
	}
	return 1.0f;
}

real __fastcall game_difficulty_get_team_value(short value_type, e_game_team team)
{
	e_campaign_difficulty_level difficulty_level = game_difficulty_level_get();
	if (game_is_multiplayer())
	{
		difficulty_level = _campaign_difficulty_level_normal;
	}
	else if (game_is_campaign() && !game_team_is_enemy(_campaign_team_player, team))
	{
		ASSERT((value_type >= 0) && (value_type < k_game_difficulty_value_count));
		if (k_difficulty_value_indirection_table[value_type] == NONE)
		{
			difficulty_level = _campaign_difficulty_level_normal;
		}
		else
		{
			value_type = k_difficulty_value_indirection_table[value_type];
		}
	}
	return game_difficulty_get_value_by_difficulty(value_type, difficulty_level);
}

s_game_globals_difficulty_information* game_globals_get_difficulty_information(s_game_globals* game_globals)
{
	if (!game_globals->difficulty.count())
	{
		return NULL;
	}

	long is_survival = game_is_survival();
	if (game_globals->difficulty.count() < is_survival)
	{
		return &game_globals->difficulty[0];
	}
	else
	{
		return &game_globals->difficulty[is_survival];
	}
}