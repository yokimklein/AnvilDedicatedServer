#include "simulation_game_objects.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_world.h>
#include <stdio.h>
#include <simulation\game_interface\simulation_game_statborg.h>
#include <simulation\game_interface\simulation_game_action.h>

void __cdecl simulation_action_object_create(datum_index object_index)
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	long entity_count = 0;
	e_simulation_entity_type entity_types[4] = {};
	long entity_object_indices[4] = {};
	simulation_action_object_create_build_entity_types(object_index, NONE, 4, &entity_count, entity_types, entity_object_indices);
	ASSERT(entity_count <= NUMBEROF(entity_object_indices));
	if (entity_count <= 0)
	{
		return;
	}
	for (long i = 0; i < entity_count; i++)
	{
		long entity_object_index = entity_object_indices[i];
		ASSERT(entity_object_index != NONE);
		ASSERT(object_get(entity_object_index)->object.gamestate_index == NONE);
		datum_index gamestate_index = simulation_gamestate_entity_create();
		object_attach_gamestate_entity(entity_object_index, gamestate_index);
		simulation_gamestate_entity_set_object_index(gamestate_index, entity_object_index);
		if (game_is_playback())
		{
			continue;
		}

		long entity_index = simulation_entity_create(entity_types[i], entity_object_indices[i], gamestate_index);
		if (entity_index == NONE)
		{
			printf("MP/NET/SIMULATION,ACTION: simulation_action_object_create: object entity creation failed! 0x%8X/%d\n", entity_object_indices[i], entity_types[i]);
		}
		else
		{
			TLS_DATA_GET_VALUE_REFERENCE(object_headers);
			c_simulation_entity_database* entity_database = simulation_get_world()->get_entity_database();
			object_datum* object = object_get(entity_object_index);
			object_header_datum* object_header = (object_header_datum*)datum_get(*object_headers, entity_object_index);
			s_simulation_entity* entity = entity_database->entity_get(entity_index);
			ASSERT(!object_header->flags.test(_object_header_being_deleted_bit));
			ASSERT(entity->gamestate_index != NONE);
			// TODO: finish print
			printf("MP/NET/SIMULATION,ACTION: simulation_action_object_create: %s '%s' index 0x%08X created entity type %d/%s index 0x%8X\n",
				/*object_type_get_name(object->object_identifier->get_type(object_identifier->type))*/"object_type_name",
				/*tag_name_strip_path(tag_get_name(*object_identifier))*/"tag_name",
				entity_object_index,
				entity_types[i],
				/*simulation_entity_type_get_name(entity_types[i])*/"entity_type_name",
				entity_index);
			entity_database->entity_capture_creation_data(entity_index);
		}
	}
}

void simulation_action_object_create_build_entity_types(datum_index object_index, datum_index last_object_index, long maximum_entity_count, long* out_entity_count, e_simulation_entity_type* entity_types, long* entity_object_indices)
{
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);
	object_datum* object = object_get(object_index);
	object_header_datum* object_header = (object_header_datum*)datum_get(*object_headers, object_index);
	datum_index object_ultimate_parent = object_get_ultimate_parent(object_index);
	if (object_header->flags.test(_object_header_being_deleted_bit)
		|| object->object.gamestate_index != NONE
		|| object_is_multiplayer_cinematic_object(object_index)
		|| object_is_multiplayer_cinematic_object(object_ultimate_parent))
	{
		return;
	}

	e_simulation_entity_type entity_type = simulation_entity_type_from_object_creation(object->definition_index, last_object_index, object->object.recycling_flags.test(_object_recycling_candidate));
	if (entity_type != k_simulation_entity_type_none && *out_entity_count < maximum_entity_count)
	{
		ASSERT(VALID_INDEX(*out_entity_count, maximum_entity_count));
		entity_types[*out_entity_count] = entity_type;
		entity_object_indices[(*out_entity_count)++] = object_index;
		object_datum* current_object = object;
		for (datum_index i = object->object.first_child_object_index; i != NONE; i = current_object->object.next_object_index)
		{
			current_object = object_get(i);
			if (current_object->object.flags.test(_object_created_with_parent_bit))
			{
				simulation_action_object_create_build_entity_types(i, object_index, maximum_entity_count, out_entity_count, entity_types, entity_object_indices);
			}
		}
	}
}

void simulation_action_object_delete(datum_index object_index)
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	object_datum* object = object_get(object_index);
	if (object->object.gamestate_index == NONE)
	{
		return;
	}

	if (!game_is_playback())
	{
		long entity_index = simulation_gamestate_entity_get_simulation_entity_index(object->object.gamestate_index);
		if (entity_index == NONE)
		{
			printf("MP/NET/SIMULATION,ACTION: simulation_action_object_delete: object 0x%8X gamestate index 0x%8X not attached to entity (can't delete entity)\n",
				object_index,
				object->object.gamestate_index);
		}
		else
		{
			simulation_entity_delete(entity_index, object_index, object->object.gamestate_index);
		}
	}
	simulation_gamestate_entity_delete(object->object.gamestate_index);
	object_detach_gamestate_entity(object_index, object->object.gamestate_index);
}

void simulation_action_object_update_internal(datum_index object_index, c_simulation_object_update_flags& update_flags)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	object_datum* object = object_get(object_index);
	if (object->object.gamestate_index == NONE)
	{
		return;
	}

	long entity_index = simulation_gamestate_entity_get_simulation_entity_index(object->object.gamestate_index);
	if (entity_index == NONE)
	{
		return;
	}
	
	simulation_entity_update(entity_index, object_index, update_flags);
}

void __fastcall simulation_action_object_force_update(datum_index object_index, c_simulation_object_update_flags& flags)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	object_datum* object = object_get(object_index);
	if (object->object.gamestate_index == NONE)
	{
		return;
	}

	long entity_index = simulation_gamestate_entity_get_simulation_entity_index(object->object.gamestate_index);
	if (entity_index == NONE)
	{
		printf("MP/NET/SIMULATION,ACTION: simulation_action_object_force_update_internal: failed to get entity index for gamestate 0x%8X (object %s)\n",
			object->object.gamestate_index,
			object_describe(object_index));
	}
	else
	{
		simulation_entity_force_update(entity_index, object_index, flags);
	}
}

long simulation_object_get_authoritative_entity(datum_index object_index)
{
	return simulation_object_get_entity_internal(object_index, false);
}

long simulation_object_get_entity_internal(datum_index object_index, bool safe)
{
	if (game_is_playback())
	{
		printf("MP/NET/SIMULATION,OBJECTS: simulation_object_get_entity_internal: attempting to call simulation_object_get_authoritative_entity() during saved film\n");
		return NONE;
	}

	if (object_index == NONE)
	{
		return NONE;
	}

	c_simulation_world* world = simulation_get_world();
	c_simulation_entity_database* entity_database = world->get_entity_database();
	object_datum* object = object_get(object_index);
	if (object->object.gamestate_index == NONE)
	{
		return NONE;
	}

	long simulation_entity_index = simulation_gamestate_entity_get_simulation_entity_index(object->object.gamestate_index);
	if (simulation_entity_index == NONE)
	{
		printf("MP/NET/SIMULATION,OBJECTS: simulation_object_get_entity_internal: failed to get entity index for gamestate 0x%8X (object %s) during simulation_object_get_authoritative_entity_internal()\n",
			object->object.gamestate_index,
			object_describe(object_index));
		return NONE;
	}

	if (!entity_database->entity_is_local(simulation_entity_index))
	{
		return NONE;
	}

	return simulation_entity_index;
}