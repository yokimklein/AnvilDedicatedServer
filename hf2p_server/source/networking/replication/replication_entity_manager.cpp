#include "replication_entity_manager.h"
#include "assert.h"
#include <stdio.h>
#include "..\..\simulation\simulation_entity_definition.h"
#include "..\..\dllmain.h"
#include "replication_entity_manager_view.h"

long c_replication_entity_manager::create_local_entity()
{
	long entity_index = -1;
	assert(this->m_client != NULL);
	long absolute_index = this->preallocate_entity();
	if (absolute_index == -1)
	{
		printf("c_replication_entity_manager::create_local_entity: unable to create local entity, table is full\n");
	}
	else
	{
		entity_index = this->create_local_entity_internal(absolute_index);
		printf("c_replication_entity_manager::create_local_entity: local entity created %lx\n", entity_index);
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
	long entity_index = absolute_index | (entity_data->seed << 28);
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
			if (entity_data->seed == entity_index >> 28)
				out_entity_data = entity_data;
		}
	}
	return out_entity_data;
}