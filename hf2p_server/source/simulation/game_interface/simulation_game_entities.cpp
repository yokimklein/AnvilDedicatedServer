#include "simulation_game_entities.h"
#include "assert.h"
#include "..\..\cseries\cseries.h"
#include "..\simulation_world.h"
#include <stdio.h>
#include "..\simulation_gamestate_entities.h"
#include "..\simulation_type_collection.h"
#include "..\..\game\game_engine_simulation.h"

long simulation_entity_create(e_simulation_entity_type simulation_entity_type, long object_index, datum_index gamestate_index)
{
	long entity_index = -1;
	c_simulation_world* simulation_world = simulation_get_world();
	if (simulation_world->is_distributed() && simulation_world->is_authority())
	{
		c_simulation_entity_database* entity_database = simulation_world->get_entity_database();
		entity_index = entity_database->entity_create(simulation_entity_type);
		if (entity_index == -1)
		{
			printf("simulation_entity_create: failed to create entity (type %d/%s object [0x%08x])\n",
				simulation_entity_type,
				simulation_entity_type_get_name(simulation_entity_type),
				object_index);
		}
		else
		{
			s_simulation_entity* simulation_entity = entity_database->entity_get(entity_index);
			assert(gamestate_index != NONE);
			simulation_entity->gamestate_index = gamestate_index;
			simulation_gamestate_entity_set_simulation_entity_index(gamestate_index, simulation_entity->entity_index);
		}
	}
	return entity_index;
}

e_simulation_entity_type simulation_entity_type_from_game_engine()
{
	return game_engine_globals_get_simulation_entity_type();
}

void simulation_entity_update(long entity_index, long unknown, c_flags<long, ulong64, 64>* update_mask)
{
	c_simulation_world* world = simulation_get_world();
	if (world->is_distributed() && world->is_authority())
	{
		c_simulation_entity_database* entity_database = world->get_entity_database();
		if (entity_database->entity_is_local(entity_index))
			entity_database->entity_update(entity_index, update_mask, false);
	}
}