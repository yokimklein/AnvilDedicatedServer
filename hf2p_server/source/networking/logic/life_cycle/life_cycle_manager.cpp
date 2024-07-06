#include "life_cycle_manager.h"
#include <cassert>

c_network_session* c_life_cycle_state_manager::get_active_squad_session() const
{
	assert(m_active_squad_session != NULL);
	return m_active_squad_session;
}

c_network_session* c_life_cycle_state_manager::get_target_session() const
{
	assert(m_target_session != NULL);
	return m_target_session;
}

//c_network_session* c_life_cycle_state_manager::get_group_session() const
//{
//	assert(m_group_session != NULL);
//	return m_group_session;
//}

c_network_observer* c_life_cycle_state_manager::get_observer() const
{
	assert(m_observer != NULL);
	return m_observer;
}