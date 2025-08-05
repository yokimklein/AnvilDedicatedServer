#include "life_cycle_manager.h"

e_life_cycle_state c_life_cycle_state_manager::get_current_state() const
{
	ASSERT(m_current_state >= 0 && m_current_state < k_life_cycle_state_count);
	return m_current_state;
}

c_network_session* c_life_cycle_state_manager::get_active_squad_session() const
{
	ASSERT(m_active_squad_session != NULL);
	return m_active_squad_session;
}

c_network_session* c_life_cycle_state_manager::get_target_session() const
{
	ASSERT(m_target_session != NULL);
	return m_target_session;
}

c_network_observer* c_life_cycle_state_manager::get_observer() const
{
	ASSERT(m_observer != NULL);
	return m_observer;
}
