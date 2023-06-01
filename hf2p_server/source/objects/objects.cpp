#include "objects.h"
#include "..\memory\tls.h"
#include "assert.h"
#include <stdio.h>
#include "..\simulation\game_interface\simulation_game_objects.h"

s_object_data* object_get(datum_index object_index)
{
	// TOOD asserts for object_get_and_verify_type
	if (object_index == NONE)
		return nullptr;
	s_object_header* object_header = (s_object_header*)datum_get(get_tls()->object_headers, object_index);
	if (!object_header)
		return nullptr;

	return object_header->data;
}

bool object_is_multiplayer_cinematic_object(datum_index object_index)
{
	assert(object_index != NONE);
	s_object_data* object = object_get(object_index);
	return object->simulation_flags.test(_object_simulation_is_multiplayer_cinematic_object);
}

datum_index object_get_ultimate_parent(datum_index object_index)
{
	datum_index parent_index = -1;
	while (object_index != -1)
	{
		parent_index = object_index;
		object_index = object_get(object_index)->parent_object_index;
	}
	return parent_index;
}

void object_attach_gamestate_entity(datum_index object_index, datum_index gamestate_index)
{
	s_object_data* object = object_get(object_index);
	assert(gamestate_index != NONE);
	assert(object->gamestate_index == NONE); // originally object->object.gamestate_index==NONE
	datum_index ultimate_parent_index = object_get_ultimate_parent(object_index);
	if (object_is_multiplayer_cinematic_object(object_index))
		printf("MP/NET/OBJECTS: object_attach_gamestate_entity: attaching simulation entity to multiplayer cinematic object [0x%08lX]\n", object_index);
	else if (object_is_multiplayer_cinematic_object(ultimate_parent_index))
		printf("MP/NET/OBJECTS: object_attach_gamestate_entity: attaching simulation entity to multiplayer cinematic object (parent) [0x%08lX]\n", ultimate_parent_index);
	object->gamestate_index = gamestate_index;
}

void object_detach_gamestate_entity(datum_index object_index, datum_index gamestate_index)
{
	s_object_data* object = object_get(object_index);
	assert(gamestate_index != NONE);
	assert(object->gamestate_index == gamestate_index); // originally object->object.gamestate_index==gamestate_index
	object_type_detach_gamestate_entity(object_index);
	object->gamestate_index = -1;
	object->simulation_flags = 0;
}

e_object_type c_object_identifier::get_type()
{
	return this->type.get();
}

void __cdecl object_set_velocities_internal(datum_index object_index, const union real_vector3d* transitional_velocity, const union real_vector3d* angular_velocity, bool skip_update)
{
	s_object_data* object = object_get(object_index);
	c_flags<long, ulong64, 64> update_flags = {};
	if (transitional_velocity)
	{
		object->transitional_velocity = *transitional_velocity;
		update_flags.set(4, true);
	}
	if (angular_velocity)
	{
		object->angular_velocity = *angular_velocity;
		update_flags.set(5, true);
	}
	if (!skip_update)
	{
		if (update_flags)
			simulation_action_object_update(object_index, &update_flags);
	}
}

void __fastcall object_set_at_rest(datum_index object_index, bool force_activate)
{
	FUNCTION_DEF(0x127C30, void, __thiscall, c_havok_component__force_activate, void* thisptr, bool unknown);
	s_data_array* g_havok_components = (s_data_array*)*(long*)(module_base + 0x1046CDC);

	s_object_header* object_header_data = (s_object_header*)(get_tls()->object_headers->data);
	s_object_data* object = object_get(object_index);
	if (((1 << object_header_data[(word)object_index].type.get()) & 0x7377) != 0
		&& (object->physics_flags.get_unsafe() & 0x80) != 0
		&& (object->havok_component_index != -1))
	{
		if (force_activate)
			goto simulation_update;
		c_havok_component__force_activate(&g_havok_components->data[128 * (word)object->havok_component_index], false);
	}
	else if (force_activate)
	{
		object->physics_flags |= 0x200u;
		return;
	}
	object->physics_flags &= ~0x200u;
	object_wake(object_index);
	// update here
simulation_update:
	// if item (weapon or equipment)
	if ((object_header_data[(word)object_index].type.get() & 0x14) != 0)
	{
		c_flags<long, ulong64, 64> update_flags = {};
		update_flags.set(14, true);
		simulation_action_object_update(object_index, &update_flags); // e_simulation_item_update_flag
	}
	// else if projectile
	else if ((object_header_data[(word)object_index].type.get() & 0x80) != 0)
	{
		c_flags<long, ulong64, 64> update_flags = {};
		update_flags.set(14, true);
		simulation_action_object_update(object_index, &update_flags); // e_simulation_projectile_update_flag
	}
}