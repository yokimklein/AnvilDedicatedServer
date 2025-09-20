#pragma once
#include "cseries\cseries.h"

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
