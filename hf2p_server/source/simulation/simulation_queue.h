#pragma once
#include "..\cseries\cseries.h"

struct c_simulation_queue
{
	byte unknown_data[0x1C];
};
static_assert(sizeof(c_simulation_queue) == 0x1C);