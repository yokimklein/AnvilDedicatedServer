#include "simulation.h"

REFERENCE_DECLARE(0x4EBEBA8, s_simulation_globals, simulation_globals);

bool simulation_reset_in_progress()
{
	return simulation_globals.reset_in_progress;
}