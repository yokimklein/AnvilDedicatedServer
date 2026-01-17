#include "cache_files.h"
#include "cache\physical_memory_map.h"
#include "cache\cache_files_windows.h"
#include "multithreading\synchronized_value.h"
#include "cseries\async_helpers.h"
#include "cseries\async.h"

char const* k_cache_strings_file = "maps\\string_ids.dat";
char const* k_cache_tags_file = "maps\\tags.dat";
char const* k_cache_tag_list_file = "maps\\tag_list.csv";
char const* k_cache_resources_file = "maps\\resources.dat";
char const* k_cache_textures_file = "maps\\textures.dat";
char const* k_cache_textures_b_file = "maps\\textures_b.dat";
char const* k_cache_audio_file = "maps\\audio.dat";
char const* k_cache_video_file = "maps\\video.dat";
char const* k_cache_lightmaps_file = "maps\\lightmaps.dat";
char const* k_cache_render_models_file = "maps\\render_models.dat";
char const* k_cache_file_extension = ".map";
char const* k_cache_path_format = "maps\\%s.map";

byte const g_cache_file_creator_key[64]
{
	0x05, 0x11, 0x6A, 0xA3, 0xCA, 0xB5, 0x07, 0xDF,
	0x50, 0xE7, 0x5B, 0x75, 0x6B, 0x4A, 0xBB, 0xF4,
	0xE8, 0x54, 0x8F, 0xC6, 0xD6, 0xCC, 0x92, 0x15,
	0x97, 0xDC, 0xF5, 0xEE, 0xB9, 0x3C, 0x01, 0x3C,
	0x95, 0xCF, 0xB8, 0x58, 0x5A, 0x6F, 0x2E, 0xB9,
	0x30, 0x6D, 0x89, 0x31, 0x2F, 0x83, 0x6F, 0xF0,
	0x9F, 0xE8, 0x37, 0x78, 0xE4, 0xC7, 0xE2, 0x2B,
	0x19, 0x66, 0x11, 0x06, 0x77, 0x24, 0x74, 0x66
};

s_tag_reference g_last_tag_accessed = { .group_tag = _tag_none, .index = _datum_index_none };

REFERENCE_DECLARE(0x3EDDCD0, s_cache_file_globals, g_cache_file_globals);

void* __fastcall tag_get(tag group_tag, datum_index tag_index)
{
	g_last_tag_accessed = { .group_tag = group_tag, .index = tag_index };

	long tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[DATUM_INDEX_TO_ABSOLUTE_INDEX(tag_index)];
	if (tag_absolute_index == NONE)
	{
		return NULL;
	}

	cache_file_tag_instance* tag_instance = g_cache_file_globals.tag_instances[tag_absolute_index];
	//if (!tag_instance)
	//{
	//	return NULL;
	//}
	void* data = tag_instance->base + tag_instance->offset;
	return data;
}

tag tag_get_group_tag(datum_index tag_index)
{
	return g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[tag_index]]->tag_group.group_tag;
}

long __fastcall cache_file_get_global_tag_index(tag group_tag)
{
	return INVOKE(0x82E30, cache_file_get_global_tag_index, group_tag);
}

const char* cache_file_get_build_number()
{
	return g_cache_file_globals.header.build_number.get_string();
}

const char* tag_get_name(long tag_name_index)
{
	// $TODO: !!!
	return "unknown_tag";
}

// $TODO: test this and verify that offsets & globals are correct
s_cache_file_resource_gestalt* __fastcall cache_files_populate_resource_gestalt()
{
	c_wrapped_array<ulong> resource_offsets;
	cache_files_populate_resource_offsets(&resource_offsets);

	long total_resource_fixup_count = 0;
	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		total_resource_fixup_count += g_cache_file_globals.tag_instances[i]->resource_fixup_count & 0xFFFF;
	}

	long resource_fixup_size = sizeof(s_cache_file_tag_resource_data*) * total_resource_fixup_count;
	long resource_gestalt_size = resource_fixup_size + sizeof(s_cache_file_resource_gestalt);

	s_cache_file_resource_gestalt* resource_gestalt = (s_cache_file_resource_gestalt*)_physical_memory_malloc(_memory_stage_level_initialize, NULL, resource_gestalt_size, 0);
	csmemset(resource_gestalt, 0, resource_gestalt_size);

	cache_file_tags_fixup_all_resources(resource_offsets, resource_gestalt);

	if (resource_offsets.m_elements)
	{
		physical_memory_free(resource_offsets.m_elements);
	}

	return resource_gestalt;
}

void cache_file_tags_fixup_all_resources(c_wrapped_array<ulong>& resource_offsets, s_cache_file_resource_gestalt* resource_gestalt)
{
	tag_block_set_elements(&resource_gestalt->resources, resource_gestalt + 1);
	g_cache_file_globals.resource_gestalt = resource_gestalt;

	s_cache_file_tag_resource_data** resource_blocks = resource_gestalt->resources.begin();

	long resource_count = 0;
	ulong resources_size = 0;
	ulong shared_files_available = 0;

	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		cache_address* resource_fixups = reinterpret_cast<cache_address*>(&instance->dependencies[instance->dependency_count + instance->data_fixup_count]);

		for (long resource_fixup_index = 0; resource_fixup_index < instance->resource_fixup_count; resource_fixup_index++)
		{
			cache_address resource_fixup = resource_fixups[resource_fixup_index];

			if (!resource_fixup.value)
			{
				continue;
			}

			if (resource_fixup.persistent)
			{
				resource_fixup.persistent = false;
			}
			resource_fixup.value += (ulong)instance;

			s_tag_resource* resource = (s_tag_resource*)resource_fixup.value;
			if (!resource)
			{
				continue;
			}

			if (!resource->resource_data)
			{
				resource->resource_handle = NONE;
				continue;
			}

			resource_blocks[resource_count] = resource->resource_data;
			s_cache_file_tag_resource_data* resource_data = resource->resource_data;

			byte flags = resource_data->file_location.flags.get_unsafe();

			if (!TEST_MASK(flags, k_cache_file_tag_resource_location_mask))
			{
				shared_files_available = 0;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			if (resource_data->file_location.shared_file_location_index == NONE)
			{
				resource_data->file_location.file_offset = 0;

				shared_files_available = 1;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			long resource_index = 0;
			for (long map_file_index = 0; map_file_index < k_cached_map_file_shared_count - 1; map_file_index++)
			{
				if (!TEST_BIT(flags, map_file_index + 1))
				{
					resource_index += g_cache_file_globals.resource_file_counts_mapping[map_file_index];
					continue;
				}

				if (!cached_map_file_is_shared(e_map_file_index(map_file_index)))
				{
					resource_data->file_location.file_offset = 0;
					continue;
				}

				ASSERT(resource_index + resource_data->file_location.shared_file_location_index < resource_offsets.count());
				resource_data->file_location.file_offset = resource_offsets[resource_index + resource_data->file_location.shared_file_location_index];
				break;
			}

			shared_files_available = 1;
			resource->resource_handle = resource_count++;
			resources_size += resource_data->file_location.size;
		}
	}

	tag_block_set_element_count(&resource_gestalt->resources, resource_count);
	resource_gestalt->resources_size = resources_size;
	resource_gestalt->shared_files_available = shared_files_available;
}

void __fastcall cache_files_populate_resource_offsets(c_wrapped_array<ulong>* resource_offsets)
{
	//INVOKE(0x83170, cache_files_populate_resource_offsets, resource_offsets);
	bool success = true;

	// shared map files - ui
	s_file_handle section_handles[k_cached_map_file_shared_count - 1]{};
	s_cache_file_section_header section_headers[k_cached_map_file_shared_count - 1]{};

	long file_count = 0;
	for (long i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		e_map_file_index next_map_file_index = e_map_file_index(i + 1);

		if (!cached_map_file_is_shared(map_file_index))
		{
			continue;
		}

		if (!cache_file_get_async_file_handle_from_index(next_map_file_index, &section_handles[map_file_index]))
		{
			success = false;
			break;
		}

		c_synchronized_long done = 0;
		c_synchronized_long size = 0;

		async_read_position(
			section_handles[i],
			&section_headers[i],
			sizeof(s_cache_file_section_header),
			0,
			_async_category_none,
			_async_priority_blocking_generic,
			&size,
			&done);

		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

		if (size.peek() != sizeof(s_cache_file_section_header))
		{
			success = false;
			break;
		}

		long section_file_count = section_headers[i].file_count;
		file_count += section_file_count;

		g_cache_file_globals.resource_file_counts_mapping[i] = section_file_count;
	}

	if (!success)
	{
		resource_offsets->set_elements(NULL, 0);
		return;
	}

	ulong* file_offsets = (ulong*)_physical_memory_malloc(_memory_stage_level_initialize, NULL, sizeof(ulong) * file_count, 0);

	long offset_index_offset = 0;
	for (long i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		if (!cached_map_file_is_shared(map_file_index))
		{
			continue;
		}

		void* offsets = (byte*)file_offsets + offset_index_offset;
		long offsets_size = sizeof(long) * section_headers[i].file_count;

		c_synchronized_long done = 0;
		c_synchronized_long size = 0;

		async_read_position(
			section_handles[i],
			offsets,
			offsets_size,
			section_headers[i].file_offsets,
			_async_category_none,
			_async_priority_blocking_generic,
			&size,
			&done);

		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

		if (size.peek() != offsets_size)
		{
			success = false;
			break;
		}

		offset_index_offset += offsets_size;
	}

	if (!success)
	{
		physical_memory_free(file_offsets);
		resource_offsets->set_elements(NULL, 0);
		return;
	}

	resource_offsets->set_elements(file_offsets, file_count);
}
