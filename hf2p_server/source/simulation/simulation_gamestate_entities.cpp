#include "simulation_gamestate_entities.h"
#include "..\memory\tls.h"
#include "assert.h"
#include <stdio.h>

datum_index simulation_gamestate_entity_create()
{
	auto tls = get_tls();
	datum_index gamestate_index = datum_new(tls->simulation_gamestate_entities);
	if (gamestate_index != -1)
	{
		simulation_gamestate_entity_datum* gamestate_entity = (simulation_gamestate_entity_datum*)datum_get(tls->simulation_gamestate_entities, gamestate_index);
		gamestate_entity->object_index = -1;
		gamestate_entity->entity_index = -1;
		gamestate_entity->__unknownC = 0;
	}
	else
	{
		printf("MP/NET/SIMULATION,GAMESTATE: simulation_gamestate_entity_create: failed to allocate simulation gamestate entity\n");
	}
	return gamestate_index;
}

void simulation_gamestate_entity_set_object_index(datum_index gamestate_index, long object_index)
{
	assert(gamestate_index != NONE);
	auto* gamestate_entity = (simulation_gamestate_entity_datum*)datum_get(get_tls()->simulation_gamestate_entities, gamestate_index);
	gamestate_entity->object_index = object_index;
}

long simulation_gamestate_entity_get_object_index(datum_index gamestate_index)
{
	long object_index = -1;
	assert(gamestate_index != NONE);
	auto* gamestate_entity = (simulation_gamestate_entity_datum*)datum_get(get_tls()->simulation_gamestate_entities, gamestate_index);
	if (gamestate_entity != nullptr)
		object_index = gamestate_entity->object_index;
	return object_index;
}

void simulation_gamestate_entity_set_simulation_entity_index(datum_index gamestate_index, long entity_index)
{
	assert(gamestate_index != NONE);
	auto* gamestate_entity = (simulation_gamestate_entity_datum*)datum_get(get_tls()->simulation_gamestate_entities, gamestate_index);
	gamestate_entity->entity_index = entity_index;
}

long simulation_gamestate_entity_get_simulation_entity_index(datum_index gamestate_index)
{
	assert(gamestate_index != NONE);
	auto* gamestate_entity = (simulation_gamestate_entity_datum*)datum_get(get_tls()->simulation_gamestate_entities, gamestate_index);
	return gamestate_entity->entity_index;
}

void simulation_gamestate_entity_delete(datum_index gamestate_index)
{
	assert(gamestate_index != NONE);
	datum_delete(get_tls()->simulation_gamestate_entities, gamestate_index);
}