#include "replication_entity_manager_view.h"
#include <networking\replication\replication_entity_manager.h>
#include <stdio.h>
#include <memory\data.h>
#include <cseries\cseries_events.h>

void c_replication_entity_manager_view::create_entity(long entity_index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(entity_index);
	ASSERT(absolute_index >= 0 && absolute_index < NUMBEROF(m_entity_data));
	ASSERT(m_entity_data[absolute_index].state == _replication_entity_view_state_none);
	set_state(entity_index, _replication_entity_view_state_ready);
}

void c_replication_entity_manager_view::set_state(long entity_index, e_replication_entity_view_state view_state)
{
	INVOKE_CLASS_MEMBER(0x20CA0, c_replication_entity_manager_view, set_state, entity_index, view_state);
}

void c_replication_entity_manager_view::mark_entity_for_deletion(long entity_index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(entity_index);
	s_replication_entity_view_data* view_entity = &m_entity_data[absolute_index];
	s_replication_entity_data* entity = m_entity_manager->get_entity(entity_index);

	ASSERT(view_entity->state != _replication_entity_view_state_none && view_entity->state != _replication_entity_view_state_deleting);
	ASSERT(!TEST_BIT(entity->deletion_mask, m_view_index));

	if (view_entity->state == _replication_entity_view_state_ready)
	{
		if (!view_entity->flags.test(_replication_entity_view_data_entity_creation_sent_flag))
		{
			set_state(entity_index, _replication_entity_view_state_none);
			m_statistics.deletions_sent++;
			event(_event_status, "networking:replication:entity:[%d] entity deleted from ready state 0x%08X", m_view_index, entity_index);
			return;
		}

		ASSERT(m_entity_data[absolute_index].flags.test(_replication_entity_view_data_entity_creation_sent_flag));
		set_state(entity_index, _replication_entity_view_state_active);
		event(_event_status, "networking:replication:entity:[%d] creation sent for entity marked for deletion (ready->active) 0x%08X", m_view_index, entity_index);
	}

	entity->deletion_mask |= FLAG(m_view_index);
	if (view_entity->state == _replication_entity_view_state_active)
	{
		if (view_entity->update_mask != 0)
		{
			m_statistics.updates_pending--;
			ASSERT(m_statistics.updates_pending >= 0);
			clear_entity_mask(absolute_index);
		}
		m_statistics.deletions_pending++;
		set_entity_mask(absolute_index);
	}
}

void c_replication_entity_manager_view::clear_entity_mask(long absolute_index)
{
	INVOKE_CLASS_MEMBER(0x1F7A0, c_replication_entity_manager_view, clear_entity_mask, absolute_index);
}

void c_replication_entity_manager_view::set_entity_mask(long absolute_index)
{
	INVOKE_CLASS_MEMBER(0x1F760, c_replication_entity_manager_view, set_entity_mask, absolute_index);
}