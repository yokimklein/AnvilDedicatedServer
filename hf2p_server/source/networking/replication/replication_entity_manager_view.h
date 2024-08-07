#pragma once
#include <cseries\cseries.h>
#include <networking\replication\replication_scheduler.h>

enum e_replication_entity_view_state
{
	_replication_entity_view_state_none,
	_replication_entity_view_state_ready,
	_replication_entity_view_state_replicating,
	_replication_entity_view_state_active,
	_replication_entity_view_state_deleting,

	k_number_of_replication_entity_view_states
};

enum e_replication_entity_view_data_entity_flags
{
	_replication_entity_view_data_entity_creation_sent_flag,

	k_number_of_replication_entity_view_data_entity_flags
};

struct s_replication_entity_view_data
{
	c_flags<e_replication_entity_view_data_entity_flags, short, k_number_of_replication_entity_view_data_entity_flags> flags;
	c_enum<e_replication_entity_view_state, short, _replication_entity_view_state_none, k_number_of_replication_entity_view_states> state;
	long entity_index;
	c_flags<long, ulong64, 64> entity_mask;
	byte __data[0x8];
};
static_assert(sizeof(s_replication_entity_view_data) == 0x18);

struct s_replication_entity_manager_view_statistics
{
	long creations_unknown;
	long creations_pending;
	long updates_unknown;
	long updates_pending;
	long deletions_unknown;
	long deletions_pending;
};
static_assert(sizeof(s_replication_entity_manager_view_statistics) == 0x18);

class c_replication_entity_packet_record;
class c_replication_entity_status_record;
class c_replication_entity_manager;
class c_replication_entity_manager_view : c_replication_scheduler_client
{
public:
	void create_entity(long entity_index);
	void set_state(long entity_index, e_replication_entity_view_state view_state);
	void mark_entity_for_deletion(long entity_index);
	void clear_entity_mask(long absolute_index);
	void set_entity_mask(long absolute_index);

	byte __data4[0x4];
	bool m_initialized;
	bool m_replicating;
	bool m_fatal_error;
	byte __unknownB; // pad?
	long m_view_index;
	dword m_view_mask;
	dword m_replication_start_time;
	c_replication_entity_manager* m_entity_manager;
	c_replication_entity_packet_record* m_packet_records;
	c_replication_entity_status_record* m_outgoing_packet;
	byte __data14[0x4];
	c_static_array<s_replication_entity_view_data, 1024> m_entity_data;
	c_static_flags<1024> __flags6028;
	s_replication_entity_manager_view_statistics m_statistics;
};
static_assert(sizeof(c_replication_entity_manager_view) == 0x60C0);