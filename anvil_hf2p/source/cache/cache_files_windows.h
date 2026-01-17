#pragma once
#include <cseries\cseries.h>
#include "cache\cache_files.h"

enum e_map_file_index
{
	_map_file_index_shared_ui = 0,
	_map_file_index_shared_resources,
	_map_file_index_shared_textures,
	_map_file_index_shared_audio,
	_map_file_index_shared_textures_b,
	_map_file_index_shared_video,
	_map_file_index_shared_render_models,
	_map_file_index_shared_lightmaps,

	k_cached_map_file_shared_count = 8,

	_first_multiplayer_cached_map_file = 9,

	k_total_tracked_cached_map_files_count = 17,
	k_no_cached_map_file_index = -1,
};

enum e_cache_file_load_action
{

};

struct s_cache_file_share_map // this probably isn't the correct name
{
	const char* file_path;
	e_map_file_index index;
	e_map_file_index previous_index;
};
static_assert(sizeof(s_cache_file_share_map) == 0xC);

struct s_cached_map_file
{
	s_file_handle file_handle;

	// io_lock_count?
	ulong __unknown4;

	s_cache_file_header header;
	s_indirect_file bulk_read_handle;
	s_file_handle overlapped_handle;
};
static_assert(sizeof(s_cached_map_file) == 0x33A0);
static_assert(OFFSETOF(s_cached_map_file, header) == 0x08);

struct s_failed_map
{
	c_static_string<k_tag_long_string_length> path;
	ulong time;
	long index;
};
static_assert(sizeof(s_failed_map) == 0x108);

struct s_cache_file_table_of_contents
{
	c_static_array<s_cached_map_file, k_total_tracked_cached_map_files_count> map_files;
	e_map_file_index open_map_file_index;
	e_map_file_index locked_map_file_index;
	e_map_file_index pending_map_file_index;
	e_cache_file_load_action pending_action;
	s_failed_map failed_maps[8];
};
static_assert(sizeof(s_cache_file_table_of_contents) == 0x375F0);
static_assert(OFFSETOF(s_cache_file_table_of_contents, map_files) == 0x00);
static_assert(OFFSETOF(s_cache_file_table_of_contents, open_map_file_index) == 0x36DA0);
static_assert(OFFSETOF(s_cache_file_table_of_contents, locked_map_file_index) == 0x36DA4);

extern s_cache_file_table_of_contents& cache_file_table_of_contents;

bool cached_map_file_is_shared(e_map_file_index map_file_index);
bool __fastcall cached_map_file_open_for_running_off_dvd(e_map_file_index index, const char* file_path);
bool __fastcall cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle);