#pragma once
#include <cseries\cseries.h>

// e_multiplayer_team
// k_multiplayer_team_none = -1

enum e_game_team
{
	_game_team_none = NONE,

	_campaign_team_default = 0,
	_campaign_team_player,
	_campaign_team_human,
	_campaign_team_covenant,
	_campaign_team_flood,
	_campaign_team_sentinel,
	_campaign_team_heretic,
	_campaign_team_prophet,
	_campaign_team_guilty,
	_campaign_team_unused9,
	_campaign_team_unused10,
	_campaign_team_unused11,
	_campaign_team_unused12,
	_campaign_team_unused13,
	_campaign_team_unused14,
	_campaign_team_unused15,

	k_campaign_team_count,

	_multiplayer_team_first = 0,
	_multiplayer_team_red = _multiplayer_team_first,
	_multiplayer_team_blue,
	_multiplayer_team_green,
	_multiplayer_team_yellow,
	_multiplayer_team_purple,
	_multiplayer_team_orange,
	_multiplayer_team_brown,
	_multiplayer_team_grey,

	k_multiplayer_team_count,

	_multiplayer_team_last = _multiplayer_team_grey,
	_multiplayer_team_none = NONE,

	k_maximum_teams = 16
};

enum e_multiplayer_team_designator
{
	_multiplayer_team_designator_defender = 0,
	_multiplayer_team_designator_attacker,
	_multiplayer_team_designator_third_party,
	_multiplayer_team_designator_fourth_party,
	_multiplayer_team_designator_fifth_party,
	_multiplayer_team_designator_sixth_party,
	_multiplayer_team_designator_seventh_party,
	_multiplayer_team_designator_eigth_party,
	_multiplayer_team_designator_neutral,

	k_multiplayer_team_designator_count
};

class c_game_variant;
bool game_engine_variant_has_teams(c_game_variant* variant);
void __cdecl game_engine_recompute_active_teams();
void __fastcall game_engine_adjust_team_score_for_composition(e_game_team team_index);
bool game_engine_variant_is_observer_allowed(c_game_variant* variant);
long __fastcall game_engine_variant_get_maximum_team_count(c_game_variant* variant, long multiplayer_map_id);
e_multiplayer_team_designator __fastcall game_engine_team_index_to_team_designator(e_game_team team);