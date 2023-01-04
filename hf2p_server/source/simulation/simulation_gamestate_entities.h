#pragma once
#include "..\memory\data.h"

datum_index simulation_gamestate_entity_create();
void simulation_gamestate_entity_set_object_index(datum_index gamestate_index, long object_index);
void simulation_gamestate_entity_set_simulation_entity_index(datum_index gamestate_index, long entity_index);