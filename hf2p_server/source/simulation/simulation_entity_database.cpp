#include "simulation_entity_database.h"
#include "assert.h"
#include <stdio.h>
#include "..\networking\network_memory.h"
#include "simulation_gamestate_entities.h"

long c_simulation_entity_database::entity_create(e_simulation_entity_type entity_type)
{
	assert(this->m_world);
	assert(this->m_entity_manager);
	assert(this->m_world->is_distributed());
	assert(this->m_world->is_authority());

	long entity_index = -1;
	void* creation_data = nullptr;
	long creation_data_size = 0;
	void* state_data = nullptr;
	long state_data_size = 0;
	if (this->entity_allocate_creation_data(entity_type, &creation_data_size, &creation_data) && this->entity_allocate_state_data(entity_type, &state_data_size, &state_data))
	{
		entity_index = this->m_entity_manager->create_local_entity();
		if (entity_index == -1)
		{
			printf("c_simulation_entity_database::entity_create: unable to allocate replication instance for new simulation entity (type %d)\n",
				entity_type);
		}
		else
		{
			this->entity_create_internal(entity_index, entity_type, creation_data_size, creation_data, state_data_size, state_data);
			creation_data = nullptr;
			state_data = nullptr;
			printf("c_simulation_entity_database::entity_create: created entity 0x%08X of type %d\n",
				entity_index,
				entity_type);
		}
	}
	else
	{
		printf("c_simulation_entity_database::entity_create: unable to allocate memory for new simulation entity (type %d)\n",
			entity_type);
	}
	if (creation_data != nullptr)
		network_heap_free_block(creation_data);
	if (state_data != nullptr)
		network_heap_free_block(state_data);
	return entity_index;
}

bool c_simulation_entity_database::entity_allocate_creation_data(e_simulation_entity_type entity_type, long* out_creation_data_size, void** out_creation_data)
{
	bool result = true;
	auto entity_definition = this->m_type_collection->get_entity_definition(entity_type);
	assert(entity_definition);
	assert(out_creation_data_size);
	assert(out_creation_data);
	void* creation_data = nullptr;
	long creation_data_size = entity_definition->creation_data_size();
	assert(creation_data_size >= 0 && creation_data_size <= k_simulation_entity_maximum_creation_data_size);
	if (creation_data_size > 0)
	{
		creation_data = network_heap_allocate_block(creation_data_size);
		if (creation_data != nullptr)
		{
			csmemset(creation_data, 0, creation_data_size);
		}
		else
		{
			char heap_description[1024];
			printf("c_simulation_entity_database::entity_allocate_creation_data: OUT OF MEMORY allocating creation data for new simulation entity (type %d, [%d] bytes) [%s]\n",
				entity_type,
				creation_data_size,
				network_heap_describe(heap_description, 1024));
			result = false;
		}
	}
	*out_creation_data_size = creation_data_size;
	*out_creation_data = creation_data;
	return result;
}

bool c_simulation_entity_database::entity_allocate_state_data(e_simulation_entity_type entity_type, long* out_state_data_size, void** out_state_data)
{
	bool result = true;
	auto entity_definition = this->m_type_collection->get_entity_definition(entity_type);
	assert(entity_definition);
	assert(out_state_data_size);
	assert(out_state_data);
	void* state_data = nullptr;
	long state_data_size = entity_definition->state_data_size();
	assert(state_data_size >= 0 && state_data_size <= k_simulation_entity_maximum_state_data_size);
	state_data = network_heap_allocate_block(state_data_size);
	if (state_data != nullptr)
	{
		csmemset(state_data, 0, state_data_size);
	}
	else
	{
		char heap_description[1024];
		printf("c_simulation_entity_database::entity_allocate_creation_data: OUT OF MEMORY allocating state data for new simulation entity (type %d, [%d] bytes) [%s]\n",
			entity_type,
			state_data_size,
			network_heap_describe(heap_description, 1024));
		result = false;
	}
	*out_state_data_size = state_data_size;
	*out_state_data = state_data;
	return result;
}

void c_simulation_entity_database::entity_create_internal(long entity_index, e_simulation_entity_type entity_type, long creation_data_size, void* creation_data, long state_data_size, void* state_data)
{
	c_simulation_entity_definition* entity_definition = this->m_type_collection->get_entity_definition(entity_type);
	assert(this->m_entity_manager);
	assert(this->m_entity_manager->is_entity_allocated(entity_index));
	assert(entity_definition);
	assert(creation_data_size == entity_definition->creation_data_size());
	assert(creation_data_size == 0 || creation_data != NULL);
	assert(state_data_size == entity_definition->state_data_size());
	assert(state_data != NULL);
	long absolute_index = entity_index & 0x3FF;
	assert(absolute_index >= 0 && absolute_index < NUMBEROF(this->m_entity_data));
	s_simulation_entity* entity = &this->m_entity_data[absolute_index];
	assert(entity->entity_index == NONE);
	assert(entity->entity_type == NONE);
	entity->entity_index = entity_index;
	entity->entity_type = entity_type;
	entity->exists_in_gameworld = false;
	entity->event_reference_count = 0;
	entity->gamestate_index = -1;
	entity->pending_update_mask = 0;
	entity->force_update_mask = 0;
	entity->creation_data_size = creation_data_size;
	entity->creation_data = creation_data;
	entity->state_data_size = state_data_size;
	entity->state_data = state_data;
}

const char* c_simulation_entity_database::get_entity_type_name(e_simulation_entity_type entity_type)
{
	assert(this->m_initialized);
	assert(this->m_type_collection);
	return this->m_type_collection->get_entity_type_name(entity_type);
}

s_simulation_entity* c_simulation_entity_database::entity_get(long entity_index)
{
	long absolute_index = entity_index & 0x3FF;
	assert(this->m_entity_manager);
	assert(this->m_entity_manager->is_entity_allocated(entity_index));
	assert(this->m_entity_data[absolute_index].entity_index == entity_index);
	return &this->m_entity_data[absolute_index];
}

void c_simulation_entity_database::entity_capture_creation_data(long entity_index)
{
	s_simulation_entity* entity = this->entity_get(entity_index);
	assert(this->m_world);
	assert(this->m_world->is_distributed());
	assert(this->m_world->is_authority());
	assert(this->m_type_collection);
	c_simulation_entity_definition* entity_definition = this->m_type_collection->get_entity_definition(entity->entity_type);
	assert(entity_definition);
	assert(entity->creation_data_size == entity_definition->creation_data_size());
	entity_definition->build_creation_data(entity->gamestate_index, entity->creation_data_size, entity->creation_data);
	assert(entity->state_data_size == entity_definition->state_data_size());
	bool baseline_valid = entity_definition->build_baseline_state_data(entity->creation_data_size, entity->creation_data, entity->state_data_size, entity->state_data);
	assert(baseline_valid);
	// TODO c_simulation_entity_database::entity_validate_creation_data
	entity->exists_in_gameworld = true;
	entity->pending_update_mask = MASK(entity_definition->update_flag_count());
	entity->force_update_mask = 0;
}

bool c_simulation_entity_database::entity_is_local(long entity_index)
{
	assert(this->m_entity_manager);
	assert(this->m_world->is_distributed());
	return this->m_entity_manager->is_entity_local(entity_index);
}

void c_simulation_entity_database::entity_update(long entity_index, c_flags<long, ulong64, 64>* update_mask, bool force_mask_update)
{
	s_simulation_entity* entity = this->entity_get(entity_index);
	assert(!update_mask->is_clear());
	assert(entity->exists_in_gameworld);
	assert(this->m_world);
	assert(this->m_world->is_distributed());
	assert(this->m_world->is_authority());
	assert(this->m_entity_manager);
	assert(this->m_entity_manager->is_entity_local(entity_index));
	assert(!this->m_entity_manager->is_entity_being_deleted(entity_index));
	c_simulation_entity_definition* entity_definition = this->m_type_collection->get_entity_definition(entity->entity_type);
	if (entity_definition->gameworld_attachment_valid(entity->gamestate_index))
	{
		if (simulation_gamestate_entity_get_simulation_entity_index(entity->gamestate_index) != entity->entity_index)
		{
			printf("c_simulation_entity_database::entity_update: entity type %d index 0x%8X (!= 0x%08X) not attached properly to gamestate 0x%8X (update)\n",
				entity_index,
				entity->entity_type,
				simulation_gamestate_entity_get_simulation_entity_index(entity->gamestate_index),
				entity->gamestate_index);
		}
	}
	else
	{
		printf("c_simulation_entity_database::entity_update: entity type %d index 0x%8X not attached properly to gamestate 0x%8X (update)\n",
			entity->entity_type,
			entity_index,
			entity->gamestate_index);
	}
	entity->pending_update_mask |= update_mask->get_unsafe();
	if (force_mask_update)
		entity->force_update_mask |= update_mask->get_unsafe();
}