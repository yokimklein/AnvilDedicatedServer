#include "cache_files_windows.h"
#include <game\game.h>

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
