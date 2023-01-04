#include "replication_entity_manager_view.h"
#include "..\..\dllmain.h"

void c_replication_entity_manager_view::create_entity(long entity_index)
{
	long absolute_index = entity_index & 0x3FF;
	// TODO - asserts once c_replication_entity_manager_view is mapped
	//assert(absolute_index >= 0 && absolute_index < NUMBEROF(this->m_entity_data));
	//assert(this->m_entity_data[absolute_index].state == _replication_entity_view_state_none);
	this->set_state(entity_index, _replication_entity_view_state_ready);
}

void c_replication_entity_manager_view::set_state(long entity_index, e_replication_entity_view_state view_state)
{
	FUNCTION_DEF(0x20CA0, void, __thiscall, replication_entity_manager_view_set_state, c_replication_entity_manager_view * thisptr, long entity_index, e_replication_entity_view_state view_state);
	replication_entity_manager_view_set_state(this, entity_index, view_state);
}