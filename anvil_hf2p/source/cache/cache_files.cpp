#include "cache_files.h"

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
