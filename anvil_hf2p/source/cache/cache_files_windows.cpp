#include "cache_files_windows.h"
#include <game\game.h>

REFERENCE_DECLARE(0x3FE1408, s_cache_file_table_of_contents, cache_file_table_of_contents);

bool cached_map_file_is_shared(e_map_file_index map_file_index)
{
	bool render = !game_disable_rendering() ||
		map_file_index != _map_file_index_shared_resources &&
		map_file_index != _map_file_index_shared_textures &&
		map_file_index != _map_file_index_shared_textures_b &&
		map_file_index != _map_file_index_shared_render_models &&
		map_file_index != _map_file_index_shared_lightmaps;
	
	bool sound = !game_disable_sound() ||
		map_file_index != _map_file_index_shared_audio;

	return render && sound;
}

bool __fastcall cached_map_file_open_for_running_off_dvd(e_map_file_index index, const char* file_path)
{
	return INVOKE(0xED780, cached_map_file_open_for_running_off_dvd, index, file_path);
}

bool __fastcall cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle)
{
	if (map_file_index < k_total_tracked_cached_map_files_count
		&& !cache_file_table_of_contents.map_files[map_file_index].header.path.is_empty()
		&& (map_file_index == cache_file_table_of_contents.open_map_file_index
			|| map_file_index == cache_file_table_of_contents.locked_map_file_index
			|| !map_file_index
			|| map_file_index - 1 <= k_cached_map_file_shared_count
			|| map_file_index == k_cached_map_file_shared_count))
	{
		*out_handle = cache_file_table_of_contents.map_files[map_file_index].file_handle;
		return true;
	}

	return false;
}
