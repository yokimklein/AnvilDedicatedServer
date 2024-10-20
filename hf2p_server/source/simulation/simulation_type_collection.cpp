#include "simulation_type_collection.h"
#include "assert.h"
#include <simulation\simulation_world.h>

c_simulation_entity_definition* c_simulation_type_collection::get_entity_definition(e_simulation_entity_type entity_type)
{
	assert(entity_type >= 0 && entity_type < m_entity_type_count);
	c_simulation_entity_definition* entity_definition = this->m_entity_definitions[entity_type];
	assert(entity_definition != NULL);
	assert(entity_definition->entity_type() == entity_type);
	return entity_definition;
}

c_simulation_event_definition* c_simulation_type_collection::get_event_definition(e_simulation_event_type event_type)
{
	assert(event_type >= 0 && event_type < m_event_type_count);
	c_simulation_event_definition* event_definition = this->m_event_definitions[event_type];
	assert(event_definition != NULL);
	assert(event_definition->event_type() == event_type);
	return event_definition;
}

const char* simulation_entity_type_get_name(e_simulation_entity_type entity_type)
{
	c_simulation_world* simulation_world = simulation_get_world();
	c_simulation_entity_database* entity_database = simulation_world->get_entity_database();
	return entity_database->get_entity_type_name(entity_type);
}

const char* c_simulation_type_collection::get_entity_type_name(e_simulation_entity_type entity_type)
{
	if (entity_type >= 0 && entity_type < this->m_entity_type_count)
		return this->get_entity_definition(entity_type)->entity_type_name();
	else
		return "unknown";
}

const char* c_simulation_type_collection::get_event_type_name(e_simulation_event_type event_type)
{
	if (event_type >= 0 && event_type < this->m_event_type_count)
		return this->get_event_definition(event_type)->event_type_name();
	else
		return "unknown";
}