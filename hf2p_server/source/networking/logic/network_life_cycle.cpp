#include "network_life_cycle.h"
#include <networking\session\network_session.h>
#include <cseries\cseries.h>

REFERENCE_DECLARE(0x3EADFA8, s_network_life_cycle_globals, life_cycle_globals);

void __fastcall network_life_cycle_end()
{
	INVOKE(0x2AC20, network_life_cycle_end);
}

bool __fastcall network_life_cycle_create_local_squad(e_network_session_class session_class)
{
	return INVOKE(0x2AD00, network_life_cycle_create_local_squad, session_class);
}

bool network_life_cycle_get_observer(c_network_observer** out_observer)
{
	if (!life_cycle_globals.initialized)
		return false;

	*out_observer = life_cycle_globals.state_manager.get_observer();
	return true;
}

c_network_session* network_life_cycle_get_squad_session_for_join()
{
	if (!life_cycle_globals.initialized)
		return nullptr;

	return life_cycle_globals.state_manager.get_active_squad_session();
}

bool network_life_cycle_in_squad_session(c_network_session** out_session)
{
	if (!life_cycle_globals.initialized)
		return false;

	c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
	if (session->current_local_state() == _network_session_state_none)
		return false;

	if (!out_session)
		return false;

	*out_session = session;
	return true;
}