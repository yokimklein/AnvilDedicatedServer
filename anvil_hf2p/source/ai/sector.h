#pragma once
#include <cseries\cseries.h>

struct c_sector_ref
{
	short structure_bsp_index;
	word sector_index;
};
static_assert(sizeof(c_sector_ref) == 0x4);