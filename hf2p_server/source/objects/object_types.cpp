#include "object_types.h"
#include "objects.h"
#include "assert.h"

void object_type_detach_gamestate_entity(datum_index object_index)
{
	s_object_data* object = object_get(object_index);
	s_object_type_definition* object_type_definition = object_type_definition_get(object->object_identifier.get_type());
	assert(object->gamestate_index != NONE);
	for (long i = 0; i < 16; i++)
	{
		if (object_type_definition->child_types[i] == nullptr)
			break;
		if (object_type_definition->child_types[i]->detach_simulation_gamestate_index)
			object_type_definition->child_types[i]->detach_simulation_gamestate_index(object_index);
	}
}

s_object_type_definition* object_type_definition_get(e_object_data_flags object_type)
{
	assert(object_type >= _object_type_biped && object_type < k_number_of_object_data_flags);
	assert(object_type_definitions[object_type]);
	assert(object_type_definitions[object_type]->group_tag);
	return (object_type_definitions)[object_type];
}