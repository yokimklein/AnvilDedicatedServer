#pragma once
#include <cseries\cseries.h>

class c_game_engine;
class c_game_variant;
struct s_game_engine_event_data;

bool game_engine_is_sandbox();
c_game_engine* current_game_engine();
bool game_engine_running();
c_game_variant* current_game_variant();
void __fastcall game_engine_send_event(s_game_engine_event_data* event_data);
short __fastcall game_engine_get_multiplayer_weapon_selection_absolute_index(long name);
string_id __fastcall game_engine_get_multiplayer_weapon_selection_name(short absolute_index);