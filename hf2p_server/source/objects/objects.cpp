#include "objects.h"
#include <memory\tls.h>
#include "assert.h"
#include <stdio.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <cache\cache_files.h>
#include <objects\object_definitions.h>
#include <physics\havok_component.h>

s_object_data* object_get(datum_index object_index)
{
	// TOOD asserts for object_get_and_verify_type
	if (object_index == NONE)
		return nullptr;
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);
	s_object_header* object_header = (s_object_header*)datum_get(*object_headers, object_index);
	if (!object_header)
		return nullptr;

	return object_header->data;
}

e_object_type object_get_type(datum_index object_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);
	return object_headers[DATUM_INDEX_TO_ABSOLUTE_INDEX(object_index)].type;
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
	c_simulation_object_update_flags update_flags{};
	if (transitional_velocity)
	{
		object->transitional_velocity = *transitional_velocity;
		update_flags.set_flag(object_index, _simulation_object_update_translational_velocity);
	}
	if (angular_velocity)
	{
		object->angular_velocity = *angular_velocity;
		update_flags.set_flag(object_index, _simulation_object_update_angular_velocity);
	}
	if (!skip_update)
	{
		if (!update_flags.m_flags.is_clear())
			simulation_action_object_update_internal(object_index, update_flags);
	}
}

void __fastcall object_set_at_rest(datum_index object_index, bool force_activate)
{
	s_object_data* object = object_get(object_index);
	if (TEST_BIT(_object_mask_physics, object_get_type(object_index)) && object->physics_flags.test(_object_connected_to_physics_bit) && object->havok_component_index != -1)
	{
		if (force_activate)
			goto simulation_update;

		c_havok_component* havok_component = (c_havok_component*)datum_get(havok_components_get_data_array(), object->havok_component_index);
		havok_component->force_activate(false);
	}
	else if (force_activate)
	{
		object->physics_flags.set(_object_physics_unknown_9_bit, true);
		return;
	}
	object->physics_flags.set(_object_physics_unknown_9_bit, false);
	object_wake(object_index);
	// update here
simulation_update:
	if (TEST_BIT(_object_mask_item, object_get_type(object_index)))
		simulation_action_object_update(object_index, _simulation_item_update_set_at_rest);
	else if (TEST_BIT(_object_mask_projectile, object_get_type(object_index)))
		simulation_action_object_update(object_index, _simulation_projectile_update_set_at_rest);
}

const char* object_describe(datum_index object_index)
{
	// TODO:
	return "UNKNOWN";
}

void __fastcall object_set_damage_owner(datum_index object_index, s_damage_owner* damage_owner, bool skip_update)
{
	s_object_data* object = object_get(object_index);
	s_object_definition* object_definition = (s_object_definition*)tag_get('obje', object->definition_index);
	if (skip_update || !TEST_BIT(_object_mask_projectile, object_get_type(object_index)) && !object_definition->object_flags.test(_object_preserves_initial_damage_owner_bit))
	{
		object->damage_owner = *damage_owner;
		if (!skip_update)
			simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
	}
}

void __fastcall object_wake(datum_index object_index)
{
	INVOKE(0x3FBE70, object_wake, object_index);
}

void __fastcall object_set_requires_motion(datum_index object_index)
{
	INVOKE(0x403E50, object_set_requires_motion, object_index);
}

bool __fastcall object_needs_rigid_body_update(datum_index object_index)
{
	return INVOKE(0x3FE620, object_needs_rigid_body_update, object_index);
}

void __fastcall attachments_update(datum_index object_index)
{
	INVOKE(0x409070, attachments_update, object_index);
}

void __fastcall object_compute_node_matrices(datum_index object_index)
{
	INVOKE(0x4056A0, object_compute_node_matrices, object_index);
}

datum_index __fastcall object_new(s_object_placement_data* placement_data)
{
	return INVOKE(0x3FCEE0, object_new, placement_data);
}

// Had to disable RTC here otherwise it'll throw an exception before add esp 4 corrects the stack
#pragma runtime_checks("", off)
void __fastcall object_set_garbage(long object_index, bool unknown_bool, long collection_ticks)
{
	INVOKE(0x403C50, object_set_garbage, object_index, unknown_bool, collection_ticks);
	__asm add esp, 4; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)