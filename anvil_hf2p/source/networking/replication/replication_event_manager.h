#pragma once
#include <cseries\cseries.h>

class c_replication_entity_manager;
class c_replication_event_manager_view;
class c_simulation_event_handler;

enum e_replication_event_state
{
	_replication_event_state_initializing = 0,
	_replication_event_state_ready,

	k_replication_event_state_count
};

class c_replication_outgoing_event
{
public:
	e_replication_event_state m_state;
	long m_type;
	dword m_timestamp;
	dword m_cancel_timer_milliseconds;
	long m_entity_references[2];
	void* m_payload_data;
	long m_payload_data_size;
	dword m_outgoing_mask;
	dword m_pending_acknowledgement_mask;
	c_replication_outgoing_event* m_next;
};
static_assert(sizeof(c_replication_outgoing_event) == 0x2C);

class c_replication_event_manager
{
public:
	c_replication_entity_manager* m_entity_manager;
	c_simulation_event_handler* m_client;
	dword m_view_mask;
	c_replication_event_manager_view* m_views[16];
	long m_outgoing_event_count;
	c_replication_outgoing_event* m_outgoing_event_list;
};
static_assert(sizeof(c_replication_event_manager) == 0x54);