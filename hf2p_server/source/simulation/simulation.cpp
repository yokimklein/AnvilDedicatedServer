#include "simulation.h"

REFERENCE_DECLARE(0x4EBEBA8, s_simulation_globals, simulation_globals);

bool simulation_reset_in_progress()
{
	return simulation_globals.reset_in_progress;
}

bool simulation_get_node_space_encoding_type(datum_index object_index)
{
	return object_get_type(object_index) == _object_type_biped;
}