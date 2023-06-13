#pragma once
#include <cseries\cseries.h>

class c_simulation_world;
class c_replication_event_manager;
class c_simulation_type_collection;
class c_simulation_entity_database;
class c_simulation_event_handler
{
	bool m_initialized;
	c_simulation_world* m_world;
	c_replication_event_manager* m_event_manager;
	c_simulation_type_collection* m_type_collection;
	c_simulation_entity_database* m_entity_database;
	byte __data[4];
};
static_assert(sizeof(c_simulation_event_handler) == 0x18);