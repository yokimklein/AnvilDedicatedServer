#include "simulation_queue_entities.h"
#include <cseries\cseries.h>

void __fastcall simulation_queue_entity_deletion_insert(s_simulation_entity* entity, bool force_cleanup)
{
	INVOKE(ADDRESS_SIMULATION_QUEUE_ENTITY_DELETION_INSERT, simulation_queue_entity_deletion_insert, entity, force_cleanup);
}