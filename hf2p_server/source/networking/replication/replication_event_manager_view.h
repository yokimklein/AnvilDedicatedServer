#pragma once
#include <cseries\cseries.h>
#include <networking\replication\replication_scheduler.h>

struct s_replication_event_manager_view_statistics
{
	long events_sent;
	long events_pending;
	long events_in_transit;
};
static_assert(sizeof(s_replication_event_manager_view_statistics) == 0xC);

class c_replication_outgoing_event;
class c_replication_event_manager;
class c_replication_event_manager_view : public c_replication_scheduler_client
{
public:
	struct c_event_record
	{
		c_replication_outgoing_event* m_event;
		c_event_record* m_next;
	};
	static_assert(sizeof(c_event_record) == 0x8);

	struct c_packet_record
	{
		long m_packet_sequence_number;
		c_event_record* m_event_list;
		c_packet_record* m_next;
	};
	static_assert(sizeof(c_packet_record) == 0xC);

private:
	bool m_initialized;
	bool m_fatal_error;
	long m_view_index;
	c_packet_record* m_packet_list;
	long m_packet_list_length;
	c_replication_event_manager* m_event_manager;
	s_replication_event_manager_view_statistics m_view_statistics;
};
static_assert(sizeof(c_replication_event_manager_view) == 0x28);