#pragma once
#include "cseries\cseries.h"

//#define TAG_RESOURCE_GET(TYPE, RESOURCE) ((TYPE*)tag_resource_get(RESOURCE))
//#define TAG_RESOURCE_TRY_TO_GET(TYPE, RESOURCE) ((TYPE*)tag_resource_try_to_get(RESOURCE))
//
//template<typename t_type, long k_depth>
//class c_typed_allocation_data_no_destruct
//{
//public:
//	t_type* get()
//	{
//		ASSERT(m_live_object);
//
//		return m_live_object;
//	}
//
//	bool alive()
//	{
//		return m_live_object != nullptr;
//	}
//
//protected:
//	c_basic_buffer<void> m_opaque_storage;
//	t_type* m_live_object;
//	c_allocation_base* m_allocator;
//};
//static_assert(sizeof(c_typed_allocation_data_no_destruct<long, 0>) == 0x10);
//
//// $TODO:
//struct s_cache_file_resource_gestalt;
//class c_cache_file_tag_resource_runtime_manager// :
//	//public c_tag_resource_runtime_listener,
//	//public c_tag_resource_runtime_active_set,
//	//public c_tag_resource_cache_file_prefetch_set,
//	//public c_tag_resource_page_range_allocator,
//	//public c_tag_resource_cache_file_reader,
//	//public c_indirect_cache_file_location_atlas,
//	//private c_physical_memory_contiguous_region_listener,
//	//private c_tag_resource_prediction_atom_generator,
//	//private c_cache_file_resource_stoler
//{
//	// $TODO:
//public:
//	void lock_for_game();
//	bool locked_for_game_UGLY() const;
//
////protected:
//	//class c_cache_file_tag_resource_datum_handler : public c_tag_resource_cache_file_datum_handler
//	//{
//	//private:
//	//	s_cache_file_resource_gestalt* m_cache_file_resource_gestalt;
//	//	c_wrapped_array<s_cache_file_resource_runtime_data_new> m_resource_runtime_data;
//	//	bool m_cache_streaming_data;
//	//	c_cache_file_resource_uber_location_table* m_file_location_table;
//	//};
//	//static_assert(sizeof(c_cache_file_tag_resource_datum_handler) == 0x18);
//	//
//	//class c_cache_file_tag_resource_location_handler : public c_tag_resource_cache_file_location_handler
//	//{
//	//private:
//	//	c_cache_file_resource_uber_location_table* m_file_location_table;
//	//};
//	//static_assert(sizeof(c_cache_file_tag_resource_location_handler) == 0x8);
//	//
//
//	char data[0x24];
//	s_cache_file_resource_gestalt* m_cache_file_resource_gestalt;
//
//	//s_cache_file_resource_runtime_active_zone_state m_active_zone_state;
//	//s_cache_file_resource_runtime_prefetching_state m_prefetching_state;
//	//bool m_loaded_any_resources;
//	//bool m_dirty_active_resource_mask;
//	//bool m_dirty_pending_resource_mask;
//	//bool m_dirty_prefetch_map_state;
//	//c_typed_allocation_data_no_destruct<c_tag_resource_cache_dynamic_predictor, 0> m_dynamic_resource_predictor;
//	//c_tag_resource_cache_precompiled_predictor m_precomputed_resource_predictor;
//	//c_static_flags<32767> m_active_resources_mask;
//	//c_static_flags<32767> m_pending_resources_mask;
//	//c_static_array<const s_cache_file_tag_resource_vtable*, 16> m_vtable_mapping;
//	//c_wrapped_array<void*> m_resource_runtime_data;
//	//c_basic_buffer<void> m_resource_interop_buffer;
//	//c_cache_file_tag_resource_datum_handler m_resource_datum_handler;
//	//c_cache_file_tag_resource_location_handler m_resource_location_handler;
//	//c_basic_buffer<void> m_resource_storage_range;
//	//c_basic_buffer<void> m_desired_resource_storage_range;
//	//c_basic_buffer<void> m_actual_resource_storage_range;
//	//c_basic_buffer<void> m_stoler_range;
//	//bool m_actual_storage_range_read_locked;
//	//
//	//byte __pad2A32D[0x3];
//	//
//	//c_basic_buffer<void> m_writeable_range;
//	//
//	//byte __pad2A33C[0xC];
//	//
//
//	char data2[0x2A2F4];
//	c_thread_safeish_tag_resource_cache m_threaded_tag_resource_cache;
//
//	//c_cache_file_tag_resource_runtime_control_allocation m_cache_file_resource_allocation;
//	//c_basic_buffer<void> m_cache_file_resource_allocation_region;
//	//c_static_sized_dynamic_array<int32, 7> m_shared_file_datum_indices;
//	//c_wrapped_data_array<s_cache_file_tag_resource_runtime_shared_file> m_shared_file_handles;
//	//c_cache_file_resource_uber_location_table m_uber_location_table;
//	//e_game_mode m_last_game_mode;
//	//s_cache_file_resource_prefetch_map_state m_last_ui_prefetch_state;
//	//s_cache_file_resource_prefetch_map_state m_last_game_prefetch_state;
//	//c_cache_file_resource_rollover_table m_rollover_table;
//	//c_cache_file_tag_resource_codec_service m_resource_codec_service;
//	//c_cache_file_resource_optional_cache_backend m_optional_cache_backend;
//	//e_game_mode m_active_game_mode;
//	//bool m_cache_streaming_data;
//	//bool m_cache_pages_for_next_map;
//	//
//	//// $TODO map this
//	//byte __data6ACAA[0x16];
//};
////static_assert(sizeof(c_cache_file_tag_resource_runtime_manager) == 0x6ACC0);
//static_assert(0x00024 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_file_resource_gestalt));
////static_assert(0x00028 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_zone_state));
////static_assert(0x0004C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_prefetching_state));
////static_assert(0x00260 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_loaded_any_resources));
////static_assert(0x00261 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_active_resource_mask));
////static_assert(0x00262 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_pending_resource_mask));
////static_assert(0x00263 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_prefetch_map_state));
////static_assert(0x00264 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dynamic_resource_predictor));
////static_assert(0x00274 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_precomputed_resource_predictor));
////static_assert(0x28298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_resources_mask));
////static_assert(0x29298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_pending_resources_mask));
////static_assert(0x2A298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_vtable_mapping));
////static_assert(0x2A2D8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_runtime_data));
////static_assert(0x2A2E0 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_interop_buffer));
////static_assert(0x2A2E8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_datum_handler));
////static_assert(0x2A300 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_location_handler));
////static_assert(0x2A308 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_storage_range));
////static_assert(0x2A310 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_desired_resource_storage_range));
////static_assert(0x2A318 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_actual_resource_storage_range));
////static_assert(0x2A320 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_stoler_range));
////static_assert(0x2A328 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_actual_storage_range_read_locked));
////static_assert(0x2A32C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_writeable_range));
//static_assert(0x2A340 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_threaded_tag_resource_cache));
////static_assert(0x2A740 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_file_resource_allocation));
////static_assert(0x2A754 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_file_resource_allocation_region));
////static_assert(0x2A75C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_shared_file_datum_indices));
////static_assert(0x2A77C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_shared_file_handles));
////static_assert(0x2A780 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_uber_location_table));
////static_assert(0x2A784 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_game_mode));
////static_assert(0x2A788 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_ui_prefetch_state));
////static_assert(0x2A890 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_game_prefetch_state));
////static_assert(0x2A998 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_rollover_table));
////static_assert(0x6A9A8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_codec_service));
////static_assert(0x6AC14 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_optional_cache_backend));
////static_assert(0x6ACA4 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_game_mode));
////static_assert(0x6ACA8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_streaming_data));
////static_assert(0x6ACA9 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_pages_for_next_map));
////static_assert(0x6ACAA == OFFSETOF(c_cache_file_tag_resource_runtime_manager, __data6ACAA));
//
//class c_cache_file_tag_resource_runtime_manager_allocation : public c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>
//{
//public:
//	//void construct();
//};
//extern c_cache_file_tag_resource_runtime_manager_allocation& g_resource_runtime_manager;

extern void __fastcall tag_resources_lock_game(long& locked);
extern void __fastcall tag_resources_unlock_game(long& locked);