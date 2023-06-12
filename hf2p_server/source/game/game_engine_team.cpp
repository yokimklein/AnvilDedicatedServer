#include "game_engine_team.h"
#include "assert.h"

bool game_engine_variant_has_teams(c_game_variant* variant)
{
	assert(variant != NULL);
	c_game_engine_base_variant* active_variant = variant->get_active_variant();
	c_game_engine_miscellaneous_options* miscellaneous_options = active_variant->get_miscellaneous_options();
	return miscellaneous_options->get_teams_enabled();
}

void game_engine_recompute_active_teams()
{
	FUNCTION_DEF(0xDC5E0, void, __cdecl, game_engine_recompute_active_teams_call);
	game_engine_recompute_active_teams_call();
}

void game_engine_adjust_team_score_for_composition(e_game_team team_index)
{
	FUNCTION_DEF(0xC9330, void, __fastcall, game_engine_adjust_team_score_for_composition_call, e_game_team team_index);
	game_engine_adjust_team_score_for_composition_call(team_index);
}