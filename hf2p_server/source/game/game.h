#pragma once
#include <game\game_options.h>

game_options* game_options_get();
e_game_simulation_type game_simulation_get();
bool game_is_server();
bool game_is_distributed();
bool game_is_playback();
e_game_playback_type game_playback_get();
bool game_is_available();
bool game_is_predicted();
bool __cdecl game_is_authoritative();
bool game_engine_has_teams();
bool game_in_progress();
bool game_is_dedicated_server();
bool game_is_campaign();
bool game_is_multiplayer();
bool game_is_survival();
bool game_is_finished();
e_campaign_difficulty_level game_difficulty_level_get();
void game_get_determinism_versions(long* determinism_version, long* determinism_compatible_version);