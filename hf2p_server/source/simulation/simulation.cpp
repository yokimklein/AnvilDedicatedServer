#include "simulation.h"

bool simulation_reset_in_progress()
{
	return simulation_globals->reset_in_progress;
}