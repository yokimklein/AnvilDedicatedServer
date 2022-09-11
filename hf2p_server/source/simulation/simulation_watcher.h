#pragma once
#include "..\cseries\cseries.h"

struct c_simulation_watcher
{
	byte unknown_data[0x16658];
};
static_assert(sizeof(c_simulation_watcher) == 0x16658);