#pragma once
#include "..\..\cseries\cseries.h"

enum e_replication_entity_view_state
{
	_replication_entity_view_state_none,
	_replication_entity_view_state_ready,
	_replication_entity_view_state_replicating,
	_replication_entity_view_state_active,
	_replication_entity_view_state_deleting,
};

class c_replication_entity_manager_view
{
public:
	void create_entity(long entity_index);
	void set_state(long entity_index, e_replication_entity_view_state view_state);
};