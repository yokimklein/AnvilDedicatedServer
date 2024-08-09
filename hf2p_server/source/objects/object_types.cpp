#include "object_types.h"
#include <objects\objects.h>
#include "assert.h"

REFERENCE_DECLARE_ARRAY(0xEB2448, object_type_definition*, object_type_definitions, k_object_type_count);

void object_type_detach_gamestate_entity(datum_index object_index)
{
	object_datum* object = object_get(object_index);
	object_type_definition* object_type_definition = object_type_definition_get(object->object.object_identifier.get_type());
	assert(object->object.gamestate_index != NONE);
	for (long i = 0; i < 16; i++)
	{
		if (object_type_definition->child_types[i] == nullptr)
			break;
		if (object_type_definition->child_types[i]->detach_simulation_gamestate_index)
			object_type_definition->child_types[i]->detach_simulation_gamestate_index(object_index);
	}
}

object_type_definition* object_type_definition_get(e_object_type object_type)
{
	assert(object_type >= _object_type_biped && object_type < k_object_type_count);
	assert(object_type_definitions[object_type]);
	assert(object_type_definitions[object_type]->group_tag);
	return (object_type_definitions)[object_type];
}