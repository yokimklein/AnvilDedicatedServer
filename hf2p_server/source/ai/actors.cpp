#include "actors.h"
#include <memory\tls.h>

void actor_handle_equipment_delete(datum_index actor_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	actor_datum* actor = (actor_datum*)datum_get(*actor_data, actor_index);
	actor->meta.character_equipment_definition_index = NONE;
}