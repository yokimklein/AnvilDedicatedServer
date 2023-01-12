#pragma once
#include "game_engine.h"
#include "game_variant.h"
#include "game_engine_event_definitions.h"

bool game_engine_is_sandbox();
c_game_engine* current_game_engine();
bool game_engine_running();
c_game_variant* current_game_variant();
void game_engine_send_event(s_game_engine_event_data* event_data);