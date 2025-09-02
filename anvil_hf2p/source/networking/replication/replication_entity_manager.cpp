#include "replication_entity_manager.h"
#include <stdio.h>
#include <simulation\simulation_entity_definition.h>
#include <networking\replication\replication_entity_manager_view.h>
#include <simulation\simulation_entity_database.h>

long c_replication_entity_manager::create_local_entity()
{
	long entity_index = NONE;
	ASSERT(m_client != NULL);

	long absolute_index = preallocate_entity();
	if (absolute_index == NONE)
	{
		printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::create_local_entity: unable to create local entity, table is full\n");
	}
	else
	{
		entity_index = create_local_entity_internal(absolute_index);
		printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::create_local_entity: local entity created %lx\n", entity_index);
	}
	return entity_index;
}

long c_replication_entity_manager::preallocate_entity()
{
	long entity_index = NONE;
	for (long i = m_entity_creation_start_position; i < NUMBEROF(m_entity_data); ++i)
	{
		if (!m_entity_data[i].flags.test(_replication_entity_allocated_flag))
		{
			entity_index = i;
			break;
		}
	}

	if (entity_index == NONE)
	{
		for (long j = 0; j < m_entity_creation_start_position; ++j)
		{
			if (!m_entity_data[j].flags.test(_replication_entity_allocated_flag))
			{
				entity_index = j;
				break;
			}
		}
	}

	if (entity_index != NONE)
	{
		m_entity_data[entity_index].flags.set_unsafe(_replication_entity_marked_for_deletion_flag);
		m_entity_creation_start_position = (entity_index + 1) % NUMBEROF(m_entity_data);
	}
	return entity_index;
}

long c_replication_entity_manager::create_local_entity_internal(long absolute_index)
{
	s_replication_entity_data* entity_data = &m_entity_data[absolute_index];
	ASSERT(absolute_index >= 0 && absolute_index < NUMBEROF(m_entity_data));
	ASSERT(m_entity_data[absolute_index].flags.get_unsafe() == FLAG(_replication_entity_allocated_flag));
	entity_data->flags.set(_replication_entity_local_flag, true);
	entity_data->seed = (entity_data->seed + 1) % 16; // $TODO: probably a macro here
	long entity_index = absolute_index | SEED_TO_ENTITY_INDEX(entity_data->seed);
	for (long view_index = 0; view_index < NUMBEROF(m_views); view_index++)
	{
		if (m_views[view_index])
		{
			m_views[view_index]->create_entity(entity_index);
		}
	}
	return entity_index;
}

bool c_replication_entity_manager::is_entity_allocated(long entity_index)
{
	return try_and_get_entity(entity_index) != NULL;
}

s_replication_entity_data* c_replication_entity_manager::try_and_get_entity(long entity_index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(entity_index);
	s_replication_entity_data* out_entity_data = NULL;
	if (absolute_index < NUMBEROF(m_entity_data))
	{
		s_replication_entity_data* entity_data = &m_entity_data[absolute_index];
		if (entity_data->flags.test(_replication_entity_allocated_flag))
		{
			if (entity_data->seed == ENTITY_INDEX_TO_SEED(entity_index))
			{
				out_entity_data = entity_data;
			}
		}
	}
	return out_entity_data;
}

bool c_replication_entity_manager::is_entity_local(long entity_index)
{
	s_replication_entity_data* entity = get_entity(entity_index);
	return entity->flags.test(_replication_entity_local_flag);
}

s_replication_entity_data* c_replication_entity_manager::get_entity(long entity_index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(entity_index);
	s_replication_entity_data* entity = &m_entity_data[absolute_index];
	ASSERT(entity_index != NONE);
	ASSERT(absolute_index >= 0 && absolute_index < NUMBEROF(m_entity_data));
	ASSERT(entity->flags.test(_replication_entity_allocated_flag));
	ASSERT(entity->seed == ENTITY_INDEX_TO_SEED(entity_index));
	return entity;
}

bool c_replication_entity_manager::is_entity_being_deleted(long entity_index)
{
	s_replication_entity_data* entity = get_entity(entity_index);
	return entity->flags.test(_replication_entity_marked_for_deletion_flag);
}

void c_replication_entity_manager::delete_local_entity(long entity_index)
{
	s_replication_entity_data* entity = get_entity(entity_index);
	ASSERT(entity->flags.test(_replication_entity_local_flag));
	ASSERT(entity->flags.test(_replication_entity_allocated_flag));
	ASSERT(!entity->flags.test(_replication_entity_marked_for_deletion_flag));
	ASSERT(entity->deletion_mask == 0);
	entity->flags.set(_replication_entity_marked_for_deletion_flag, true);
	printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx\n", entity_index);
	ASSERT(m_client);
	m_client->notify_mark_entity_for_deletion(entity_index, false);
	if (m_view_mask)
	{
		for (long view_index = 0; view_index < NUMBEROF(m_views); view_index++)
		{
			if (TEST_BIT(m_view_mask, view_index))
			{
				ASSERT(m_views[view_index] != NULL);
				m_views[view_index]->mark_entity_for_deletion(entity_index);
			}
		}
	}
	if (!entity->deletion_mask)
	{
		delete_entity_internal(entity_index);
	}
}

void c_replication_entity_manager::delete_entity_internal(long entity_index)
{
	s_replication_entity_data* entity = get_entity(entity_index);
	ASSERT(m_client != NULL);
	ASSERT(entity->flags.test(_replication_entity_allocated_flag));
	ASSERT(entity->flags.test(_replication_entity_marked_for_deletion_flag));
	ASSERT(entity->deletion_mask == 0);
	m_client->entity_delete_internal(entity_index);
	entity->flags.set(_replication_entity_allocated_flag, false);
	printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx\n", entity_index);
}