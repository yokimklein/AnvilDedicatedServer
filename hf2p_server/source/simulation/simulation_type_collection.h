#pragma once
#include <cseries\cseries.h>
#include <simulation\game_interface\simulation_game_entities.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\simulation_entity_definition.h>

struct c_simulation_event_definition;
class c_simulation_type_collection
{
public:
	c_simulation_entity_definition* get_entity_definition(e_simulation_entity_type entity_type);
	const char* get_entity_type_name(e_simulation_entity_type entity_type);

	long m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;
	long m_event_type_count;
	c_static_array<struct c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};

const char* simulation_entity_type_get_name(e_simulation_entity_type entity_type);