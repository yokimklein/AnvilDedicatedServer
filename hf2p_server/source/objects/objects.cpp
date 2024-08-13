#include "objects.h"
#include <memory\tls.h>
#include "assert.h"
#include <stdio.h>
#include <cache\cache_files.h>
#include <objects\object_definitions.h>
#include <physics\havok_component.h>
#include <simulation\game_interface\simulation_game_projectiles.h>
#include <simulation\game_interface\simulation_game_items.h>
#include <tag_files\tag_groups.h>

object_datum* __fastcall object_try_and_get_and_verify_type(datum_index object_index, dword object_type_mask)
{
	return INVOKE(0x403000, object_try_and_get_and_verify_type, object_index, object_type_mask);
}

object_datum* object_get(datum_index object_index)
{
	return (object_datum*)object_get_and_verify_type(object_index, _object_mask_object);
}

e_object_type object_get_type(datum_index object_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);
	return object_headers[DATUM_INDEX_TO_ABSOLUTE_INDEX(object_index)].type;
}

bool object_is_multiplayer_cinematic_object(datum_index object_index)
{
	assert(object_index != NONE);
	object_datum* object = object_get(object_index);
	return object->object.simulation_flags.test(_object_simulation_is_multiplayer_cinematic_object);
}

datum_index object_get_ultimate_parent(datum_index object_index)
{
	datum_index parent_index = -1;
	while (object_index != -1)
	{
		parent_index = object_index;
		object_index = object_get(object_index)->object.parent_object_index;
	}
	return parent_index;
}

void object_attach_gamestate_entity(datum_index object_index, datum_index gamestate_index)
{
	object_datum* object = object_get(object_index);
	assert(gamestate_index != NONE);
	assert(object->object.gamestate_index == NONE);
	datum_index ultimate_parent_index = object_get_ultimate_parent(object_index);
	if (object_is_multiplayer_cinematic_object(object_index))
		printf("MP/NET/OBJECTS: object_attach_gamestate_entity: attaching simulation entity to multiplayer cinematic object [0x%08lX]\n", object_index);
	else if (object_is_multiplayer_cinematic_object(ultimate_parent_index))
		printf("MP/NET/OBJECTS: object_attach_gamestate_entity: attaching simulation entity to multiplayer cinematic object (parent) [0x%08lX]\n", ultimate_parent_index);
	object->object.gamestate_index = gamestate_index;
}

void object_detach_gamestate_entity(datum_index object_index, datum_index gamestate_index)
{
	object_datum* object = object_get(object_index);
	assert(gamestate_index != NONE);
	assert(object->object.gamestate_index == gamestate_index);
	object_type_detach_gamestate_entity(object_index);
	object->object.gamestate_index = -1;
	object->object.simulation_flags = 0;
}

e_object_type c_object_identifier::get_type()
{
	return this->m_type.get();
}

void __cdecl object_set_velocities_internal(datum_index object_index, real_vector3d const* transitional_velocity, real_vector3d const* angular_velocity, bool skip_update)
{
	object_datum* object = object_get(object_index);
	c_simulation_object_update_flags update_flags;
	if (transitional_velocity)
	{
		object->object.transitional_velocity = *transitional_velocity;
		update_flags.set_flag(object_index, _simulation_object_update_translational_velocity);
	}
	if (angular_velocity)
	{
		object->object.angular_velocity = *angular_velocity;
		update_flags.set_flag(object_index, _simulation_object_update_angular_velocity);
	}
	if (!skip_update)
	{
		if (!update_flags.m_flags.is_clear())
		{
			simulation_action_object_update_internal(object_index, update_flags);
		}
	}
}

void __fastcall object_set_at_rest(datum_index object_index, bool force_activate)
{
	object_datum* object = object_get(object_index);
	if (TEST_BIT(_object_mask_physics, object_get_type(object_index)) && object->object.physics_flags.test(_object_connected_to_physics_bit) && object->object.havok_component_index != -1)
	{
		if (force_activate)
			goto simulation_update;

		c_havok_component* havok_component = (c_havok_component*)datum_get(havok_components_get_data_array(), object->object.havok_component_index);
		havok_component->force_activate(false);
	}
	else if (force_activate)
	{
		object->object.physics_flags.set(_object_physics_unknown_9_bit, true);
		return;
	}
	object->object.physics_flags.set(_object_physics_unknown_9_bit, false);
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
	object_datum* object = object_get(object_index);
	struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object->definition_index);
	if (skip_update || !TEST_BIT(_object_mask_projectile, object_get_type(object_index)) && !object_definition->object_flags.test(_object_preserves_initial_damage_owner_bit))
	{
		object->object.damage_owner = *damage_owner;
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

	//object_datum* object = object_try_and_get_and_verify_type(object_index, -1);
	//if (object->object.gamestate_index == NONE)
	//	return false;
	//if (object->object.havok_component_index == NONE || object->object.object_identifier.type.get() != _object_type_crate)
	//	return false;
	//c_havok_component* havok_component = (c_havok_component*)datum_get(havok_components_get_data_array(), object->havok_component_index);
	//for (long i = 0; i < havok_component->m_havok_rigid_bodies.m_size; i++)
	//{
	//	c_havok_component::c_rigid_body* rigid_body = havok_component->m_havok_rigid_bodies.m_data + (i * sizeof(c_havok_component::c_rigid_body));
	//	char rb_type = *(char*)((size_t)rigid_body->m_rigid_body + 0xA8);
	//}
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
void __fastcall object_set_garbage(datum_index object_index, bool unknown_bool, long collection_ticks)
{
	INVOKE(0x403C50, object_set_garbage, object_index, unknown_bool, collection_ticks);
	__asm add esp, 4; // Fix usercall & cleanup stack
}

bool __fastcall object_set_position_internal(datum_index object_index, real_point3d* desired_position, real_vector3d* desired_forward, real_vector3d* desired_up, s_location const* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected)
{
	INVOKE(0x3FBF90, object_set_position_internal, object_index, desired_position, desired_forward, desired_up, location, compute_node_matrices, set_havok_object_position, in_editor, disconnected);
	__asm add esp, 0x1C; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

object_header_datum const* object_header_get(datum_index object_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);
	return static_cast<object_header_datum*>(datum_try_and_get(*object_headers, object_index));
}

void* object_get_and_verify_type(datum_index object_index, dword object_type_mask)
{
	//ASSERT(game_state_is_locked(), "someone is calling object_get when the game state is locked");

	object_header_datum const* object_header = object_header_get(object_index);
	if (!object_header)
		return NULL;

	object_datum* object = object_header->datum;

	if (!_bittest((long*)&object_type_mask, object->object.object_identifier.m_type.get()))
	{
	//	c_static_string<256> string_builder;
	//	string_builder.print_line("got an object type we didn't expect (expected one of 0x%08x but got #%d).",
	//		object_type_mask,
	//		object->object_identifier.m_type.get());
	//
		assert(!_bittest((long*)&object_type_mask, object->object.object_identifier.m_type.get())/*, string_builder.get_string()*/);
	}

	return object;
}

void __fastcall object_get_origin_interpolated(datum_index object_index, real_point3d* out_origin)
{
	INVOKE(0x401390, object_get_origin_interpolated, object_index, out_origin);
	/*
	object_datum* object = object_get(object_index);
	if (object->object.gamestate_index != NONE
		&& object->object.simulation_flags.test(_object_simulation_requires_interpolation_bit)
		&& simulation_get_object_interpolated_position(object_index, out_origin))
	{
		if (object->object.parent_object_index != NONE)
		{
			real_matrix4x3* node_matrix = object_get_node_matrix(object->object.parent_object_index, object->object.parent_node_index);
			matrix4x3_transform_point(node_matrix, out_origin, out_origin);
		}
	}
	else
	{
		object_get_origin(object_index, out_origin);
	}
	*/
}

void* __cdecl object_header_block_get(long object_index, object_header_block_reference const* reference)
{
	object_header_datum const* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	assert(reference->offset > 0);
	assert(reference->size > 0);
	assert(reference->offset + reference->size <= object_header->data_size);

	return (byte*)object + reference->offset;
}

void* __cdecl object_header_block_get_with_count(long object_index, object_header_block_reference const* reference, unsigned int element_size, long* element_count)
{
	assert(element_count);

	if (reference->offset == NONE)
	{
		*element_count = 0;
		return NULL;
	}

	void* block = object_header_block_get(object_index, reference);

	assert(reference->size % element_size == 0);

	*element_count = reference->size / element_size;

	return block;
}

real_matrix4x3* object_get_node_matrices(datum_index object_index, long* out_node_count)
{
	object_datum* object = object_get(object_index);
	assert(out_node_count);
	return (real_matrix4x3*)object_header_block_get_with_count(object_index, &object->object.node_matrices, 0x34, out_node_count);
}

void object_get_damage_owner(datum_index object_index, s_damage_owner* out_owner)
{
	object_datum* object = object_get(object_index);
	*out_owner = object->object.damage_owner;
}