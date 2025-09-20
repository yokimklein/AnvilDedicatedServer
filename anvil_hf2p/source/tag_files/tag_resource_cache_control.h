#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

struct s_tag_resource_control_datum : s_datum_header
{
	ushort flags;
	byte __data4[0x4];
	//s_tag_resource_location_handle_struct page_handle;
	ulong page_handle;
};
static_assert(sizeof(s_tag_resource_control_datum) == 0xC);

class c_tag_resource_cache_controller;
class c_tag_resource_cache_control_datum_info
{
public:
	c_tag_resource_cache_controller* m_cache_controller;
};

class c_tag_resource_page_table_control_interface;
class c_tag_resource_runtime_listener;
class c_tag_resource_cache_file_datum_handler;
class c_tag_resource_runtime_active_set;
class c_tag_resource_address_cache_control_interface;
class c_tag_resource_cache_controller
{
public:
	void mark_necessary_resources(c_tag_resource_runtime_active_set* runtime_active_set, c_tag_resource_address_cache_control_interface* controller_interface, bool* out_unknown);
	void acquire_page_reservation(datum_index index);
	void release_page_reservation_force(datum_index index);

	c_allocation_base* m_tracking_data_allocator;
	void* m_tracking_allocation_base;
	c_smart_data_array<s_tag_resource_control_datum> m_resource_control_data;

	// $TODO: verify fields
	//c_wrapped_flags m_optional_resources;
	//c_wrapped_flags m_usable_resources;
	//c_wrapped_flags m_potentially_active_resources;
	//c_wrapped_flags m_pages_ready_resources;
	////c_wrapped_flags m_pending_resources; // removed since ms23
	//c_wrapped_flags m_last_published_deferred_resources;
	//c_wrapped_flags m_demanded_deferred_resources;
	//c_tag_resource_page_table_control_interface* m_resource_page_table;
	//c_tag_resource_runtime_listener* m_resource_listener;
	char data[60];

	c_tag_resource_cache_file_datum_handler* m_resource_datum_handler;

	// $TODO: verify fields
	//c_tag_resource_cache_control_datum_info m_control_datum_info;
	//ulong m_usage_frame_index;
	//ulong m_update_since_demand_frame_counter;
	////bool m_any_deferred_demands_pending;
	////bool m_optional_caching_enabled;
	//bool __unknown5C;
	//bool m_prediction_can_occur;
	//bool m_can_load_all_desired_resources;
	//bool m_throttle_demand_to_io;
};
//static_assert(sizeof(c_tag_resource_cache_controller) == 0x60);
static_assert(OFFSETOF(c_tag_resource_cache_controller, m_resource_control_data) == 0x08);
static_assert(OFFSETOF(c_tag_resource_cache_controller, m_resource_datum_handler) == 0x48);

bool bitmap_group_can_be_sampled(datum_index tag_index);
bool this_tag_is_bitmap_group(datum_index tag_index);
bool this_tag_is_bink_group(datum_index tag_index);
bool this_tag_is_sound_group(datum_index tag_index);