#include "simulation_event_handler.h"
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\simulation_type_collection.h>

const char* c_simulation_event_handler::get_event_type_name(e_simulation_event_type event_type)
{
	assert(m_initialized);
	assert(m_type_collection);
	return m_type_collection->get_event_type_name(event_type);
}

void c_simulation_event_handler::send_event(e_simulation_event_type event_type, long entity_reference_count, long const* entity_reference_indices, c_flags<long, ulong, k_maximum_machines> machine_mask, long event_payload_size, void const* event_payload, long cancel_timer)
{
	DECLFUNC(0x3A900, void, __thiscall, c_simulation_event_handler*, e_simulation_event_type, long, long const*, c_flags<long, ulong, k_maximum_machines>, long, void const*, long)(this, event_type, entity_reference_count, entity_reference_indices, machine_mask, event_payload_size, event_payload, cancel_timer);
}