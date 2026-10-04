#pragma once
#include "cseries\cseries.h"
#include "cache\cache_file_tag_resource_vtable_list.h"
#include "memory\secure_signature.h"

struct s_cache_file_local_resource_location
{
	ulong flags : 2;
	ulong file_size : 30;
	ulong memory_size;
	s_network_http_request_hash entire_checksum;
};
static_assert(sizeof(s_cache_file_local_resource_location) == 0x1C);

struct s_cache_file_insertion_point_resource_usage
{
	char initial_zone_set_index;
	byte pad[0x3];
	c_static_flags<1024> shared_required_locations;
	c_static_flags<320> local_required_locations;
	byte __dataAC[0x8];
};
static_assert(sizeof(s_cache_file_insertion_point_resource_usage) == 0xB4);

// unused in halo online, zeroed in every map header
struct s_cache_file_shared_resource_usage
{
	s_tag_persistent_identifier shared_layout_identifier;
	ushort shared_location_count;
	ushort local_location_count;
	ulong first_file_offset;
	s_tag_persistent_identifier codec_identifier;
	c_static_array<s_cache_file_local_resource_location, 320> local_locations;
	byte insertion_point_usage_count;
	char pad[0x3];
	c_static_array<s_cache_file_insertion_point_resource_usage, 9> insertion_point_usages;
};
static_assert(sizeof(s_cache_file_shared_resource_usage) == 0x2980);

class c_tag_resource_fixup;
class s_control_fixups_tag_block;
class s_interop_locations_tag_block;
class c_tag_resource_cache_file_datum_handler
{
public:
	
	// $TODO: GET CORRECT OFFSETS
	virtual void get_control_data(c_basic_buffer<ulong>* buffer, long resource_handle);
	virtual c_tag_resource_fixup* get_root_address_fixup(c_tag_resource_fixup* fixup, long resource_handle);
	// is_streamed removed
	virtual void* get_permanent_resource_root_address(long resource_handle, bool a3);
	virtual long get_resource_definition(long resource_handle) const;
	virtual datum_index get_resource_owner(long resource_handle) const; // returns tag index
	virtual void get_control_fixups(c_wrapped_array<s_control_fixups_tag_block>* array, long resource_handle);
	virtual void get_interop_buffer(c_basic_buffer<ulong>* buffer, long resource_handle);
	virtual void get_interop_locations(c_wrapped_array<s_interop_locations_tag_block>* array, long resource_handle);
	virtual bool any_pageable_data(long resource_handle) const;
	virtual long get_pageable_location_handle(long resource_handle) const;
};

class c_tag_resource_runtime_active_set
{
public:
	virtual bool any_resources_active() const;
	virtual bool is_resource_required(datum_index resource_owner, long resource_handle) const;
	virtual bool is_resource_deferred(datum_index resource_owner, long resource_handle) const;
	virtual bool is_resource_pending(datum_index resource_owner, long resource_handle) const;
};
static_assert(sizeof(c_tag_resource_runtime_active_set) == 0x4);

extern void __fastcall tag_resources_lock_game(long& locked);
extern void __fastcall tag_resources_unlock_game(long& locked);
