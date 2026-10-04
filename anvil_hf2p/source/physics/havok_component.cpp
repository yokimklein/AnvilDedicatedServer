#include "havok_component.h"

REFERENCE_DECLARE(ADDRESS_G_HAVOK_COMPONENTS, c_smart_data_array<c_havok_component>, g_havok_components);

s_data_array* havok_components_get_data_array()
{
	return g_havok_components;
}

void c_havok_component::force_activate(bool active)
{
	DECLFUNC(ADDRESS_C_HAVOK_COMPONENT_FORCE_ACTIVATE, void, __thiscall, c_havok_component*, bool)(this, active);
}