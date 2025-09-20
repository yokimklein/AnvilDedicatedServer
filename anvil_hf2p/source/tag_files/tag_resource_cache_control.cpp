#include "tag_resource_cache_control.h"
#include "tag_files\tag_resource_cache_external_access.h"
#include "game\game.h"
#include "cache\cache_files.h"
#include "main\main_game.h"
#include "cache\cache_file_tag_resource_runtime.h"
#include "tag_files\tag_resource_cache_address_cache.h"

void c_tag_resource_cache_controller::mark_necessary_resources(c_tag_resource_runtime_active_set* runtime_active_set, c_tag_resource_address_cache_control_interface* controller_interface, bool* out_unknown)
{
	static bool s3d_preload_all_resources = true;
	static bool s3d_unload_bitmap_pages_after_fixupped = true;

	s_data_iterator iterator;
	data_iterator_begin(&iterator, m_resource_control_data);
	*out_unknown = false;
	while (data_iterator_next(&iterator))
	{
		long resource_handle = tag_resource_cache_handle_get_external_handle(iterator.index);

		s_tag_resource_control_datum* resource_control = (s_tag_resource_control_datum*)datum_get(m_resource_control_data, iterator.index);

		bool flag_bit_1_set = TEST_BIT(resource_control->flags, 1);
		bool flag_bit_3_set = TEST_BIT(resource_control->flags, 3);

		datum_index tag_index = m_resource_datum_handler->get_resource_owner(resource_handle);
		bool any_pageable_data = m_resource_datum_handler->any_pageable_data(resource_handle);

		bool resource_required = runtime_active_set->is_resource_required(tag_index, resource_handle);
		bool resource_deferred = runtime_active_set->is_resource_deferred(tag_index, resource_handle);
		bool resource_pending = runtime_active_set->is_resource_pending(tag_index, resource_handle);

		bool resource_pending_and_pageable = resource_pending && any_pageable_data;

		if (s3d_preload_all_resources)
		{
			resource_required = (!resource_required ? true : resource_required);
		}

		if (tag_index != NONE)
		{
			if (s3d_unload_bitmap_pages_after_fixupped && !resource_required)
			{
				if (this_tag_is_bitmap_group(tag_index) && !bitmap_group_can_be_sampled(tag_index) && !resource_pending_and_pageable)
				{
					resource_pending_and_pageable = resource_pending;
				}
			}

			bool disable_bitmap_or_bink = false;
			if (game_disable_rendering())
			{
				if (this_tag_is_bitmap_group(tag_index) || this_tag_is_bink_group(tag_index))
				{
					disable_bitmap_or_bink = true;
				}
			}
			else if (this_tag_is_bink_group(tag_index))
			{
				if (!main_game_pending_mainmenu())
				{
					disable_bitmap_or_bink = true;
				}
			}

			if (game_disable_sound() && this_tag_is_sound_group(tag_index) || disable_bitmap_or_bink)
			{
				resource_pending_and_pageable = false;
				resource_pending = false;
				resource_deferred = false;
				resource_required = false;
			}
		}

		SET_BIT(resource_control->flags, 1, resource_required);
		SET_BIT(resource_control->flags, 2, resource_deferred);
		SET_BIT(resource_control->flags, 3, resource_pending_and_pageable);

		if (TEST_BIT(resource_control->flags, 5) && !flag_bit_1_set && !flag_bit_3_set && (resource_required || resource_pending))
		{
			*out_unknown = true;
		}

		if (TEST_BIT(resource_control->flags, 1) || !TEST_BIT(resource_control->flags, 3))
		{
			if (TEST_BIT(resource_control->flags, 2) || TEST_BIT(resource_control->flags, 1))
			{
				acquire_page_reservation(iterator.index);
			}
			else
			{
				controller_interface->staleify_resource_address(iterator.index);
				// pending resources bitvector removed from here since ms23
				release_page_reservation_force(iterator.index);
			}
		}
		else
		{
			controller_interface->staleify_resource_address(iterator.index);
			acquire_page_reservation(iterator.index);
		}

		if (TEST_BIT(resource_control->flags, 1) || !TEST_BIT(resource_control->flags, 2) &&
			!TEST_BIT(resource_control->flags, 2) &&
			(TEST_BIT(resource_control->flags, 1) || !TEST_BIT(resource_control->flags, 3)))
		{
			controller_interface->mark_required_resource(iterator.index);
		}
		else
		{
			controller_interface->mark_unrequired_resource(iterator.index);
		}
	}
}

void c_tag_resource_cache_controller::acquire_page_reservation(datum_index index)
{
	INVOKE_CLASS_MEMBER(0x247C40, c_tag_resource_cache_controller, acquire_page_reservation, index);
}

void c_tag_resource_cache_controller::release_page_reservation_force(datum_index index)
{
	INVOKE_CLASS_MEMBER(0x247D30, c_tag_resource_cache_controller, release_page_reservation_force, index);
}

bool bitmap_group_can_be_sampled(datum_index tag_index)
{
	// $TODO: proper bitmap_definition struct
	//bitmap_definition* bitmap = (bitmap_definition*)tag_get(BITMAP_TAG, tag_index);
	long* bitmap = (long*)tag_get(BITMAP_TAG, tag_index);
	return bitmap[1] < 0;
}

bool this_tag_is_bitmap_group(datum_index tag_index)
{
	return tag_index != NONE && tag_get_group_tag(tag_index) == BITMAP_TAG;
}

bool this_tag_is_bink_group(datum_index tag_index)
{
	return tag_index != NONE && tag_get_group_tag(tag_index) == BINK_TAG;
}

bool this_tag_is_sound_group(datum_index tag_index)
{
	return tag_index != NONE && tag_get_group_tag(tag_index) == SOUND_TAG;
}
