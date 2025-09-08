#pragma once
#include "cseries\cseries.h"
#include "multithreading\synchronized_value.h"
#include "cache\restricted_memory_regions.h"

enum
{
	k_maximum_game_state_header_size = 0x10000,
	k_game_state_header_region_size = 0x30000,
	k_game_state_update_region_size = 0x200000,
	k_game_state_render_region_size = 0x2D0000,
	k_game_state_shared_region_size = 0x480000,

	k_game_state_allocation_size = k_game_state_header_region_size + k_game_state_update_region_size + k_game_state_render_region_size + k_game_state_shared_region_size,
	k_game_state_file_size = k_game_state_allocation_size + (k_game_state_shared_region_size * 2),

	k_game_state_compressor_allocation_size = 0x480000,
	k_game_state_maximum_compressed_size = 0x480000,
};
static_assert(0x0980000 == k_game_state_allocation_size);
static_assert(0x1280000 == k_game_state_file_size);

enum e_restricted_section_constants
{
	k_max_section_mirrors = 3 // lowered by 1 since ms23
};

enum e_memory_protection
{
	_memory_protection_noaccess = 0,
	_memory_protection_readonly,
	_memory_protection_readwrite
};

class c_restricted_section
{
public:
	enum e_internal_flags
	{
		_initialized = 0
	};

	ulong m_internal_flags;
	long m_protection_scheme;
	long m_external_lock;
	byte* m_address;
	ulong m_size;
	long m_owner_thread;
};
static_assert(sizeof(c_restricted_section) == 0x18);

class c_restricted_memory_callbacks;
class c_restricted_memory
{
public:
	enum e_internal_flags
	{
		_initialized = 0
	};

	enum e_constants
	{
		k_max_registered_members = 128
	};

	struct s_mirror_slot
	{
		c_interlocked_long valid;
		c_synchronized_long readable_flag;
		c_synchronized_long writable_flag;
		c_restricted_section* restricted_section;
	};
	static_assert(sizeof(s_mirror_slot) == 0x10);

	struct s_registered_member
	{
		ulong original_offset;
		ulong member_offset;
		ulong size;
		void(__cdecl* update_function)(void*);
		void(__cdecl* post_copy_function)(void*);
		void(__cdecl* overwrite_function)(void*);
	};
	static_assert(sizeof(s_registered_member) == 0x18);

	bool initialized();
	ulong add_member
	(
		const char* name,
		const char* type_name,
		long original_requested_size,
		long alignment_bits,
		void(__cdecl* update_function)(void*),
		void(__cdecl* overwrite_function)(void*),
		void(__cdecl* post_copy_function)(void*)
	);
	ulong get_member_offset(ulong member_index);

	c_interlocked_long m_mirror_read_in_progress;
	c_interlocked_long m_mirror_write_in_progress;
	s_mirror_slot m_mirrors[k_max_section_mirrors];
	c_interlocked_long m_mirror_write_position;
	c_interlocked_long m_mirror_read_position;
	c_synchronized_long m_alias_allowed;
	c_synchronized_long m_alias_count;
	c_restricted_section* m_primary_section;
	ulong m_internal_flags;
	ulong m_tls_handle;
	ulong m_subsection_size;
	ulong m_reserved_size;
	long m_mirror_count;
	long m_internal_lock;
	ulong m_registered_member_count;
	s_registered_member m_registered_member[k_max_registered_members];
	c_restricted_memory_callbacks* m_callbacks;
	void* m_primary_backup_memory;
	void* m_mirror_backup_memory;
	long m_region_index;
};
static_assert(sizeof(c_restricted_memory) == 0xC78);

//extern const char* (&g_restricted_region_names)[k_total_restricted_memory_regions];
extern c_restricted_section(&g_restricted_section)[k_total_restricted_memory_regions];
extern c_restricted_memory(&g_restricted_regions)[k_total_restricted_memory_regions];