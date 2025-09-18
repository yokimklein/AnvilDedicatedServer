#include "simulation_game_entities.h"
#include <cache\cache_files.h>
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <game\game_engine_util.h>
#include <models\model_definitions.h>
#include <objects\object_definitions.h>
#include <physics\physics_model_definitions.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_type_collection.h>
#include <simulation\simulation_world.h>
#include <tag_files\tag_groups.h>
#include <stdio.h>

long simulation_entity_create(e_simulation_entity_type simulation_entity_type, long object_index, datum_index gamestate_index)
{
	c_simulation_world* simulation_world = simulation_get_world();
	if (!simulation_world->is_distributed() || !simulation_world->is_authority())
	{
		return NONE;
	}

	c_simulation_entity_database* entity_database = simulation_world->get_entity_database();
	long entity_index = entity_database->entity_create(simulation_entity_type);
	if (entity_index == NONE)
	{
		event(_event_error, "networking:simulation:entities: failed to create entity (type %d/%s object [0x%08x])",
			simulation_entity_type,
			simulation_entity_type_get_name(simulation_entity_type),
			object_index);
	}
	else
	{
		s_simulation_entity* simulation_entity = entity_database->entity_get(entity_index);
		ASSERT(gamestate_index != NONE);
		simulation_entity->gamestate_index = gamestate_index;
		simulation_gamestate_entity_set_simulation_entity_index(gamestate_index, simulation_entity->entity_index);
	}
	return entity_index;
}

e_simulation_entity_type simulation_entity_type_from_game_engine()
{
	return game_engine_globals_get_simulation_entity_type();
}

void simulation_entity_update(long entity_index, datum_index object_index, c_simulation_object_update_flags& update_flags)
{
	c_simulation_world* world = simulation_get_world();
	if (!world->is_distributed() || !world->is_authority())
	{
		return;
	}

	c_simulation_entity_database* entity_database = world->get_entity_database();
	if (entity_database->entity_is_local(entity_index))
	{
		entity_database->entity_update(entity_index, update_flags, false);
	}
}

e_simulation_entity_type simulation_entity_type_from_object_creation(long object_tag_index, datum_index object_index, bool recycling)
{
	object_definition* object_tag = (object_definition*)tag_get(OBJECT_TAG, object_tag_index);
	switch (object_tag->object.type.get())
	{
		case _object_type_biped:
		{
			return _simulation_entity_type_unit;
		}
		case _object_type_vehicle:
		{
			return _simulation_entity_type_vehicle;
		}
		case _object_type_weapon:
		{
			if (object_index != NONE)
			{
				return k_simulation_entity_type_none;
			}
			return _simulation_entity_type_weapon;
		}
		case _object_type_equipment:
		{
			return _simulation_entity_type_item;
		}
		case _object_type_terminal:
		case _object_type_machine:
		case _object_type_control:
		{
			return _simulation_entity_type_device;
		}
		case _object_type_projectile:
		{
			return _simulation_entity_type_projectile;
		}
		case _object_type_armor: // i'm making an assumption that armors would've synced the same as scenery
		case _object_type_scenery: // seeing as scenery tags were used before armor tags existed
		{
			if (game_engine_is_sandbox())
			{
				if (object_tag->object.multiplayer_object.count() > 0)
				{
					return _simulation_entity_type_generic;
				}
			}
			else if (object_tag->object.model.index != NONE)
			{
				model_definition* model_tag = (model_definition*)tag_get(MODEL_TAG, object_tag->object.model.index);
				if (model_tag->damage_info.count() > 0)
				{
					s_global_damage_info_block* damage_info = model_tag->damage_info.begin();
					if (damage_info->damage_sections.count() > 0)
					{
						return _simulation_entity_type_generic;
					}
					if (damage_info->damage_constraints.count() > 0)
					{
						return _simulation_entity_type_generic;
					}
				}
			}
			return k_simulation_entity_type_none;
		}
		case _object_type_crate:
		{
			if (object_tag->object.model.index == NONE)
			{
				return k_simulation_entity_type_none;
			}
			model_definition* model_tag = (model_definition*)tag_get(MODEL_TAG, object_tag->object.model.index);
			physics_model_definition* physics_model_tag = (physics_model_definition*)tag_get(PHYSICS_MODEL_TAG, model_tag->physics_model.index);
			if (model_tag->physics_model.index != NONE && physics_model_tag->flags.test(_physics_model_flags_make_physical_children_keyframed_bit))
			{
				return k_simulation_entity_type_none;
			}
			return recycling ? _simulation_entity_type_generic_garbage : _simulation_entity_type_generic;
		}
		default:
		{
			return k_simulation_entity_type_none;
		}
	}
}

void simulation_entity_delete(long entity_index, datum_index object_index, datum_index gamestate_index)
{
	c_simulation_world* world = simulation_get_world();
	if (!world->is_distributed())
	{
		return;
	}

	c_simulation_entity_database* entity_database = world->get_entity_database();
	s_simulation_entity* entity = entity_database->entity_get(entity_index);
	ASSERT(gamestate_index != NONE);
	ASSERT(entity->gamestate_index == gamestate_index);
	ASSERT(simulation_gamestate_entity_get_object_index(entity->gamestate_index) == object_index);
	ASSERT(entity->exists_in_gameworld);
	if (entity_database->entity_is_local(entity_index))
	{
		entity->gamestate_index = NONE;
		entity->exists_in_gameworld = false;
		entity_database->entity_delete(entity_index);
		return;
	}
	if (!simulation_reset_in_progress() && game_in_progress())
	{
		if (object_index == NONE)
		{
			VASSERT(0, c_string_builder("networking:simulation:entity: game engine entity deleted with non-local entity 0x%08x type %d",
				entity_index,
				entity->entity_type).get_string());
		}
		else
		{
			VASSERT(0, c_string_builder("networking:simulation:entity: object 0x%08x (%s) deleted with non-local entity 0x%08x type %d",
				object_index,
				tag_name_strip_path(tag_get_name(object_get(object_index)->definition_index)),
				entity_index,
				entity->entity_type).get_string());
		}
	}
	entity->gamestate_index = NONE;
	entity->exists_in_gameworld = false;
}

void simulation_entity_force_update(long entity_index, datum_index object_index, c_simulation_object_update_flags& update_flags)
{
	c_simulation_world* world = simulation_get_world();
	e_simulation_world_type world_type = world->m_world_type;
	if ((world_type != _simulation_world_type_distributed_game_authority
		&& world_type != _simulation_world_type_distributed_game_client)
		|| world_type == _simulation_world_type_distributed_game_client)
	{
		return;
	}

	c_simulation_distributed_world* distributed_world = world->m_distributed_world;
	if (distributed_world->m_entity_database.entity_is_local(entity_index))
	{
		distributed_world->m_entity_database.entity_update(entity_index, update_flags, true);
	}
}