#include "game_engine_team.h"
#include <game\game_engine_default.h>
#include <game\game_engine_variant.h>
#include <scenario\scenario_map_variant.h>
#include <cseries\cseries.h>

bool game_engine_variant_has_teams(c_game_variant* variant)
{
	ASSERT(variant != NULL);
	return variant->get_active_variant()->get_miscellaneous_options()->get_teams_enabled();
}

void __cdecl game_engine_recompute_active_teams()
{
	INVOKE(0xDC5E0, game_engine_recompute_active_teams);
}

void __fastcall game_engine_adjust_team_score_for_composition(e_game_team team_index)
{
	INVOKE(0xC9330, game_engine_adjust_team_score_for_composition, team_index);
}

bool game_engine_variant_is_observer_allowed(c_game_variant* variant)
{
	// this function always returns false in ms23, why is that? were observers removed before release?
	ASSERT(variant != NULL);

	// Observers were removed in ODST
	return false;

	//c_game_engine_base_variant* active_variant = variant->get_active_variant();
	//c_game_engine_social_options* social_options = active_variant->get_social_options();
	//return social_options->get_observers_enabled();
}

long __fastcall game_engine_variant_get_maximum_team_count(c_game_variant* variant, long multiplayer_map_id)
{
	ASSERT(variant);
	ASSERT(multiplayer_map_id != _map_id_none);
	return INVOKE(0xDC6F0, game_engine_variant_get_maximum_team_count, variant, multiplayer_map_id);
}

e_multiplayer_team_designator __fastcall game_engine_team_index_to_team_designator(e_game_team team)
{
	return INVOKE(0xDC3B0, game_engine_team_index_to_team_designator, team);
}