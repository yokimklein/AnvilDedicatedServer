#pragma once
#include <memory\data.h>

struct simulation_gamestate_entity_datum : s_datum_header
{
	word __unknown2;
	dword entity_index;
	dword object_index;
	bool __unknownC; // or byte
};
static_assert(sizeof(simulation_gamestate_entity_datum) == 0x10);

datum_index simulation_gamestate_entity_create();
void simulation_gamestate_entity_set_object_index(datum_index gamestate_index, long object_index);
long simulation_gamestate_entity_get_object_index(datum_index gamestate_index);
void simulation_gamestate_entity_set_simulation_entity_index(datum_index gamestate_index, long entity_index);
long simulation_gamestate_entity_get_simulation_entity_index(datum_index gamestate_index);
void simulation_gamestate_entity_delete(datum_index gamestate_index);