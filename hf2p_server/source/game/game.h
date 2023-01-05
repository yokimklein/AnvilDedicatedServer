#pragma once
#include "game_options.h"

game_options* game_options_get();
bool game_is_server();
bool game_is_distributed();
bool game_is_playback();
e_game_playback_type game_playback_get();
bool game_is_available();