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

void* __fastcall tag_get(tag group_tag, long index)
{
	return INVOKE(0x83C70, tag_get, group_tag, index);
}

long __fastcall cache_file_get_global_tag_index(tag group_tag)
{
	return INVOKE(0x82E30, cache_file_get_global_tag_index, group_tag);
}