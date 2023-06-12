#include "replication_entity_manager_view.h"
#include "replication_entity_manager.h"
#include "assert.h"
#include <stdio.h>

void c_replication_entity_manager_view::create_entity(long entity_index)
{
	long absolute_index = entity_index & 0x3FF;
	assert(absolute_index >= 0 && absolute_index < NUMBEROF(this->m_entity_data));
	assert(this->m_entity_data[absolute_index].state == _replication_entity_view_state_none);
	this->set_state(entity_index, _replication_entity_view_state_ready);
}

void c_replication_entity_manager_view::set_state(long entity_index, e_replication_entity_view_state view_state)
{
	FUNCTION_DEF(0x20CA0, void, __thiscall, replication_entity_manager_view_set_state, c_replication_entity_manager_view * thisptr, long entity_index, e_replication_entity_view_state view_state);
	replication_entity_manager_view_set_state(this, entity_index, view_state);
}

void c_replication_entity_manager_view::mark_entity_for_deletion(long entity_index)
{
	s_replication_entity_data* entity = this->m_entity_manager->get_entity(entity_index);
	long absolute_index = entity_index & 0x3FF;
	s_replication_entity_view_data* view_entity = &this->m_entity_data[absolute_index];
	assert(view_entity->state != _replication_entity_view_state_none && view_entity->state != _replication_entity_view_state_deleting);
	assert(!TEST_BIT(entity->deletion_mask, this->m_view_index));
	if (view_entity->state != _replication_entity_view_state_ready || this->m_entity_data[absolute_index].flags.test(_replication_entity_view_data_entity_creation_sent_flag))
	{
		if (view_entity->state == _replication_entity_view_state_ready)
		{
			assert(this->m_entity_data[absolute_index].flags.test(_replication_entity_view_data_entity_creation_sent_flag));
			this->set_state(entity_index, _replication_entity_view_state_active);
			printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager_view::mark_entity_for_deletion: [%d] creation sent for entity marked for deletion (ready->active) 0x%08X\n",
				this->m_view_index,
				entity_index);
		}
		entity->deletion_mask |= 1 << this->m_view_index;
		if (view_entity->state == _replication_entity_view_state_active)
		{
			if (!view_entity->entity_mask.is_clear())
			{
				--this->m_statistics.updates_pending;
				assert(this->m_statistics.updates_pending >= 0);
				this->clear_entity_mask(absolute_index);
			}
			++this->m_statistics.deletions_pending;
			this->set_entity_mask(absolute_index);
		}
	}
	else
	{
		this->set_state(entity_index, _replication_entity_view_state_none);
		++this->m_statistics.deletions_unknown;
		printf("MP/NET/REPLICATION,ENTITY: c_replication_entity_manager_view::mark_entity_for_deletion: [%d] entity deleted from ready state 0x%08X\n",
			this->m_view_index,
			entity_index);
	}
}

void c_replication_entity_manager_view::clear_entity_mask(long absolute_index)
{
	FUNCTION_DEF(0x1F7A0, void, __thiscall, clear_entity_mask, c_replication_entity_manager_view* thisptr, long absolute_index);
	clear_entity_mask(this, absolute_index);
}

void c_replication_entity_manager_view::set_entity_mask(long absolute_index)
{
	FUNCTION_DEF(0x1F760, void, __thiscall, set_entity_mask, c_replication_entity_manager_view* thisptr, long absolute_index);
	set_entity_mask(this, absolute_index);
}