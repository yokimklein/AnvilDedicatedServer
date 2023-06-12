#include "replication_entity_manager.h"
#include "assert.h"
#include <stdio.h>
#include "..\..\simulation\simulation_entity_definition.h"
#include "replication_entity_manager_view.h"

long c_replication_entity_manager::create_local_entity()
{
	long entity_index = -1;
	assert(this->m_client != NULL);
	long absolute_index = this->preallocate_entity();
	if (absolute_index == -1)
	{
		printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::create_local_entity: unable to create local entity, table is full\n");
	}
	else
	{
		entity_index = this->create_local_entity_internal(absolute_index);
		printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::create_local_entity: local entity created %lx\n", entity_index);
	}
	return entity_index;
}

long c_replication_entity_manager::preallocate_entity()
{
	long entity_index = -1;
	for (long i = this->unknown_count; i < NUMBEROF(this->m_entity_data); ++i)
	{
		if (!this->m_entity_data[i].flags.test(_replication_entity_allocated_flag))
		{
			entity_index = i;
			break;
		}
	}
	if (entity_index == -1)
	{
		for (long j = 0; j < this->unknown_count; ++j)
		{
			if (!this->m_entity_data[j].flags.test(_replication_entity_allocated_flag))
			{
				entity_index = j;
				break;
			}
		}
	}
	if (entity_index != -1)
	{
		this->m_entity_data[entity_index].flags.set_raw_bits(_replication_entity_marked_for_deletion_flag);
		this->unknown_count = (entity_index + 1) % NUMBEROF(this->m_entity_data);
	}
	return entity_index;
}

long c_replication_entity_manager::create_local_entity_internal(long absolute_index)
{
	auto entity_data = &this->m_entity_data[absolute_index];
	assert(absolute_index >= 0 && absolute_index < NUMBEROF(this->m_entity_data));
	assert(this->m_entity_data[absolute_index].flags.get_unsafe() == FLAG(_replication_entity_allocated_flag));
	entity_data->flags.set(_replication_entity_local_flag, true);
	entity_data->seed = (entity_data->seed + 1) % 16;
	long entity_index = absolute_index | SEED_TO_ENTITY_INDEX(entity_data->seed);
	for (long i = 0; i < 16; i++)
	{
		if (this->m_views[i] != nullptr)
			this->m_views[i]->create_entity(entity_index);
	}
	return entity_index;
}

bool c_replication_entity_manager::is_entity_allocated(long entity_index)
{
	return this->try_and_get_entity(entity_index) != nullptr;
}

s_replication_entity_data* c_replication_entity_manager::try_and_get_entity(long entity_index)
{
	long absolute_index = entity_index & 0x3FF;
	s_replication_entity_data* out_entity_data = nullptr;
	if (absolute_index < 1024)
	{
		s_replication_entity_data* entity_data = &this->m_entity_data[absolute_index];
		if (entity_data->flags.test(_replication_entity_allocated_flag))
		{
			if (entity_data->seed == ENTITY_INDEX_TO_SEED(entity_index))
				out_entity_data = entity_data;
		}
	}
	return out_entity_data;
}

bool c_replication_entity_manager::is_entity_local(long entity_index)
{
	s_replication_entity_data* entity = this->get_entity(entity_index);
	return entity->flags.test(_replication_entity_local_flag);
}

s_replication_entity_data* c_replication_entity_manager::get_entity(long entity_index)
{
	long absolute_index = entity_index & 1023;
	s_replication_entity_data* entity = &this->m_entity_data[absolute_index];
	assert(entity_index != NONE);
	assert(absolute_index >= 0 && absolute_index < NUMBEROF(this->m_entity_data));
	assert(entity->flags.test(_replication_entity_allocated_flag));
	assert(entity->seed == ENTITY_INDEX_TO_SEED(entity_index));
	return entity;
}

bool c_replication_entity_manager::is_entity_being_deleted(long entity_index)
{
	s_replication_entity_data* entity = this->get_entity(entity_index);
	return entity->flags.test(_replication_entity_marked_for_deletion_flag);
}

void c_replication_entity_manager::delete_local_entity(long entity_index)
{
	s_replication_entity_data* entity = this->get_entity(entity_index);
	assert(entity->flags.test(_replication_entity_local_flag));
	assert(entity->flags.test(_replication_entity_allocated_flag));
	assert(!entity->flags.test(_replication_entity_marked_for_deletion_flag));
	assert(entity->deletion_mask == 0);
	entity->flags.set(_replication_entity_marked_for_deletion_flag, true);
	printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx\n", entity_index);
	assert(this->m_client);
	this->m_client->notify_mark_entity_for_deletion(entity_index, false);
	if (this->m_view_mask)
	{
		for (long view_index = 0; view_index < 16; view_index++)
		{
			if ((this->m_view_mask & (1 << view_index)) != 0)
			{
				assert(this->m_views[view_index] != NULL);
				this->m_views[view_index]->mark_entity_for_deletion(entity_index);
			}
		}
	}
	if (!entity->deletion_mask)
		this->delete_entity_internal(entity_index);
}

void c_replication_entity_manager::delete_entity_internal(long entity_index)
{
	s_replication_entity_data* entity = this->get_entity(entity_index);
	assert(this->m_client != NULL);
	assert(entity->flags.test(_replication_entity_allocated_flag));
	assert(entity->flags.test(_replication_entity_marked_for_deletion_flag));
	assert(entity->deletion_mask == 0);
	this->m_client->entity_delete_internal(entity_index);
	entity->flags.set(_replication_entity_allocated_flag, false);
	printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx\n", entity_index);
}