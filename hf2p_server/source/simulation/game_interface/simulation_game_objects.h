#pragma once
#include "simulation_game_entities.h"
#include "..\..\cseries\cseries.h"

void simulation_action_object_create(datum_index object_index);
void simulation_action_object_create_build_entity_types(datum_index object_index, datum_index last_object_index, long maximum_entity_count, long* out_entity_count, e_simulation_entity_type* entity_types, long* object_indices);
void simulation_action_object_update(datum_index object_index, c_flags<long, ulong64, 64>* update_flags);
void simulation_action_object_delete(datum_index object_index);