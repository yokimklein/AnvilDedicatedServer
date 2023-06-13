#include "network_life_cycle.h"

bool network_life_cycle_get_observer(c_network_observer** out_observer)
{
	if (*life_cycle_initialized)
	{
		*out_observer = *(c_network_observer**)base_address(0x3EADFD8);
		return true;
	}
	return false;
}