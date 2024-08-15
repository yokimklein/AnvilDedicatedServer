#include "simulation.h"
#include <game\game.h>
#include <simulation\simulation_watcher.h>

REFERENCE_DECLARE(0x4EBEBA8, s_simulation_globals, simulation_globals);

bool simulation_reset_in_progress()
{
	return simulation_globals.reset_in_progress;
}

bool simulation_get_node_space_encoding_type(datum_index object_index)
{
	return object_get_type(object_index) == _object_type_biped;
}

bool simulation_boot_machine(s_machine_identifier const* machine_identifier, e_network_session_boot_reason reason)
{
	assert(machine_identifier);
	if (game_is_playback())
		return true;
	else
		return simulation_globals.watcher->boot_machine(machine_identifier, reason);
}