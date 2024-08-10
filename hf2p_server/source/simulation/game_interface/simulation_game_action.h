#pragma once
#include <memory\data.h>

void simulation_action_game_engine_globals_create();
void simulation_action_game_engine_globals_delete();
void simulation_action_game_statborg_create();
void simulation_action_game_statborg_delete();
void simulation_action_game_ai_create();
void simulation_action_game_ai_delete();
void simulation_action_game_map_variant_create_all();
void simulation_action_game_map_variant_create_for_chunk(long chunk_index);
void simulation_action_game_map_variant_delete();
void simulation_action_breakable_surfaces_create();
//void simulation_action_actor_create(datum_index unit_index);