#pragma once
#include <game\game_variant.h>
#include <game\player_configuration.h>

bool game_engine_variant_has_teams(c_game_variant* variant);
void game_engine_recompute_active_teams();
void game_engine_adjust_team_score_for_composition(e_game_team team_index);