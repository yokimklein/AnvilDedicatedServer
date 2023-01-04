#pragma once
#include "game_interface\simulation_game_entities.h"
#include "simulation_event_handler.h"

class c_simulation_world;
class c_replication_entity_manager;
class c_simulation_type_collection;
class c_simulation_entity_database
{
public:
	long entity_create(e_simulation_entity_type entity_type);
	bool entity_allocate_creation_data(e_simulation_entity_type entity_type, long* out_creation_data_size, void** out_creation_data);
	bool entity_allocate_state_data(e_simulation_entity_type entity_type, long* out_state_data_size, void** out_state_data);
	void entity_create_internal(long entity_index, e_simulation_entity_type entity_type, long creation_data_size, void* creation_data, long state_data_size, void* state_data);
	const char* get_entity_type_name(e_simulation_entity_type entity_type);
	s_simulation_entity* entity_get(long entity_index);
	void entity_capture_creation_data(long entity_index);

	bool m_initialized;
	byte __unknown1;
	c_simulation_world* m_world;
	c_replication_entity_manager* m_entity_manager;
	c_simulation_type_collection* m_type_collection;
	c_static_array<s_simulation_entity, 1024> m_entity_data;
	c_simulation_event_handler m_event_handler;
};
static_assert(sizeof(c_simulation_entity_database) == 0xC028);