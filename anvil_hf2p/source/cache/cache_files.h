#pragma once
#include <cseries\cseries.h>
#include <tag_files\files.h>

void* __fastcall tag_get(tag group_tag, long index);
long __fastcall cache_file_get_global_tag_index(tag group_tag);
const char* cache_file_get_build_number();

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get((GROUP), (INDEX)))
#define TAG_GET_SAFE(GROUP, TYPE, INDEX) ((INDEX) != NONE ? ((TYPE*)tag_get((GROUP), (INDEX))) : NULL)
#define TAG_BLOCK_GET_ELEMENT(BLOCK, INDEX, TYPE) ((TYPE*)tag_block_get_element_with_size((BLOCK), (INDEX), sizeof(TYPE)))

enum e_cache_file_shared_file_type
{
	_cache_file_shared_file_type_ui = 0,
	_cache_file_shared_file_type_resources,
	_cache_file_shared_file_type_textures,
	_cache_file_shared_file_type_textures_b,
	_cache_file_shared_file_type_audio,
	_cache_file_shared_file_type_video,
	_cache_file_shared_file_type_render_models,
	_cache_file_shared_file_type_lightmaps,

	k_number_of_cache_file_shared_file_types
};

#pragma pack(push, 4)
union s_cache_file_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		char data[0x1C];
		c_static_string<k_tag_long_string_length> path;
		c_static_string<k_tag_string_length> build_number;
	};
};
static_assert(OFFSETOF(s_cache_file_header, path) == 0x1C);
static_assert(OFFSETOF(s_cache_file_header, build_number) == 0x11C);
#pragma pack(pop)

struct s_cache_file_globals
{
	// $TODO: 
	char data[0x28];
	s_cache_file_header header;
};
static_assert(OFFSETOF(s_cache_file_globals, header) == 0x28);

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
extern s_cache_file_globals& g_cache_file_globals;