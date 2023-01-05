#pragma once
#include "..\..\cseries\cseries.h"

void simulation_action_game_engine_globals_create();
void simulation_action_game_statborg_create();
void simulation_action_game_ai_create();
void simulation_action_game_map_variant_create_all();
void simulation_action_game_map_variant_create_for_chunk(long chunk_index);
void simulation_action_game_engine_player_create(short player_absolute_index);
void simulation_action_breakable_surfaces_create();
void simulation_action_game_statborg_update(c_flags<long, ulong64, 64>* update_mask);
void simulation_action_game_engine_player_update(short absolute_player_index, ulong update_mask);