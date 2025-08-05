#pragma once
#include <cseries\cseries.h>

class c_replication_entity_manager_view;
class c_simulation_entity_database;

enum e_replication_entity_flags
{
	_replication_entity_allocated_flag,
	_replication_entity_marked_for_deletion_flag,
	_replication_entity_local_flag,

	k_replication_entity_flags_count
};
using c_replication_entity_flags = c_flags<e_replication_entity_flags, byte, k_replication_entity_flags_count>;

struct s_replication_entity_data
{
	c_replication_entity_flags flags;
	byte seed;
	short deletion_mask;
};
static_assert(sizeof(s_replication_entity_data) == 0x4);

class c_replication_entity_manager
{
public:
	long create_local_entity();
	long preallocate_entity();
	long create_local_entity_internal(long absolute_index);
	bool is_entity_allocated(long entity_index);
	s_replication_entity_data* try_and_get_entity(long entity_index);
	bool is_entity_local(long entity_index);
	s_replication_entity_data* get_entity(long entity_index);
	bool is_entity_being_deleted(long entity_index);
	void delete_local_entity(long entity_index);
	void delete_entity_internal(long entity_index);

	c_simulation_entity_database* m_client;
	c_replication_entity_manager_view* m_views[16];
	dword m_view_mask;
	s_replication_entity_data m_entity_data[1024];
	long m_entity_creation_start_position;
};
static_assert(sizeof(c_replication_entity_manager) == 0x104C);
