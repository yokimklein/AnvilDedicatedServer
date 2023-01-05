#pragma once
#include "..\..\cseries\cseries.h"

enum e_replication_entity_flags
{
	_replication_entity_allocated_flag,
	_replication_entity_marked_for_deletion_flag,
	_replication_entity_local_flag,

	k_replication_entity_flags_count
};

struct s_replication_entity_data
{
	c_flags<e_replication_entity_flags, byte, k_replication_entity_flags_count> flags;
	byte seed;
	byte __data[2];
};
static_assert(sizeof(s_replication_entity_data) == 0x4);

class c_simulation_entity_database;
class c_replication_entity_manager_view;
struct s_replication_entity_data;
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

	c_simulation_entity_database* m_client;
	c_static_array<c_replication_entity_manager_view*, 16> m_views;
	dword m_view_mask;
	c_static_array<s_replication_entity_data, 1024> m_entity_data;
	long unknown_count;
};
static_assert(sizeof(c_replication_entity_manager) == 0x104C);
