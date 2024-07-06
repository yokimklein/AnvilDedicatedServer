#pragma once

class c_game_variant;

bool game_engine_variant_has_teams(c_game_variant* variant);
void __cdecl game_engine_recompute_active_teams();
void __fastcall game_engine_adjust_team_score_for_composition(enum e_game_team team_index);
bool game_engine_variant_is_observer_allowed(c_game_variant* variant);
long __fastcall game_engine_variant_get_maximum_team_count(c_game_variant* variant, long multiplayer_map_id);