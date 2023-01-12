#include "objects.h"
#include "..\memory\tls.h"
#include "assert.h"
#include <stdio.h>

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
	return object->simulation_flags.test(_object_simulation_is_cinematic_object);
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

e_object_data_flags c_object_identifier::get_type()
{
	return this->type.get();
}