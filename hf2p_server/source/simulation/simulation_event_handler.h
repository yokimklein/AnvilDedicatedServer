#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_simulation.h>

enum e_simulation_event_type;
class c_simulation_world;
class c_replication_event_manager;
class c_simulation_type_collection;
class c_simulation_entity_database;
class c_simulation_event_handler
{
public:
	const char* get_event_type_name(e_simulation_event_type event_type);
	void send_event(e_simulation_event_type event_type, long entity_reference_count, long const* entity_reference_indices, c_flags<long, ulong, k_maximum_machines> machine_mask, long event_payload_size, void const* event_payload, long cancel_timer);

protected:
	bool m_initialized;
	c_simulation_world* m_world;
	c_replication_event_manager* m_event_manager;
	c_simulation_type_collection* m_type_collection;
	c_simulation_entity_database* m_entity_database;
	byte __data[4];
};
static_assert(sizeof(c_simulation_event_handler) == 0x18);