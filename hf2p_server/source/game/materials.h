#pragma once
#include <cseries/cseries.h>

struct c_global_material_type
{
	c_global_material_type() :
		m_index(NONE)
	{
	}

	word m_index;
};
static_assert(sizeof(c_global_material_type) == 0x2);