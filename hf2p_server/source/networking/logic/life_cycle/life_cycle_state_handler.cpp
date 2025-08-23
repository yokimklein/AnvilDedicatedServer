#include "life_cycle_state_handler.h"

c_life_cycle_state_manager* c_life_cycle_state_handler::get_manager() const
{
	ASSERT(m_manager != NULL);
	return m_manager;
}

bool c_life_cycle_state_handler::all_peers_have_main_menu_ready(c_network_session* session) const
{
	return INVOKE_CLASS_MEMBER(0x4BFF0, c_life_cycle_state_handler, all_peers_have_main_menu_ready, session);
}
