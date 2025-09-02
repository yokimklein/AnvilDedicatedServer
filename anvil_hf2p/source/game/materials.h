#pragma once
#include <cseries/cseries.h>

struct c_global_material_type
{
	c_global_material_type() :
		m_index(NONE)
	{
	}

	bool operator!=(c_global_material_type const& other) const
	{
		return m_index != other.m_index;
	}
	bool operator!() const
	{
		return !m_index;
	}

	word m_index;
};
static_assert(sizeof(c_global_material_type) == 0x2);