#pragma once
#include <cseries\cseries.h>

void* __fastcall tag_get(tag group_tag, long index);
long __fastcall cache_file_get_global_tag_index(tag group_tag);

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get(GROUP, INDEX))

extern char const* k_cache_strings_file;
extern char const* k_cache_tags_file;
extern char const* k_cache_tag_list_file;
extern char const* k_cache_resources_file;
extern char const* k_cache_textures_file;
extern char const* k_cache_textures_b_file;
extern char const* k_cache_audio_file;
extern char const* k_cache_video_file;
extern char const* k_cache_lightmaps_file;
extern char const* k_cache_render_models_file;
extern char const* k_cache_file_extension;
extern char const* k_cache_path_format;