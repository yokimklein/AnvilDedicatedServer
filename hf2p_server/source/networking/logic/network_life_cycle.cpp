#include "network_life_cycle.h"

bool network_life_cycle_get_observer(c_network_observer** out_observer)
{
	if (*life_cycle_initialized)
	{
		*out_observer = *(c_network_observer**)(module_base + 0x3EADFD8);
		return true;
	}
	return false;
}