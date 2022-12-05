#include "scenario_map_variant.h"

c_map_variant::c_map_variant()
{
	FUNCTION_DEF(0xAB2F0, void, __thiscall, c_map_variant_ctor, c_map_variant* thisptr);
	c_map_variant_ctor(this);
}

void c_map_variant::create_default(e_map_id map_id)
{
	FUNCTION_DEF(0xAB380, e_map_id, __thiscall, create_default, c_map_variant* thisptr, e_map_id map_id);
	create_default(this, map_id);
}