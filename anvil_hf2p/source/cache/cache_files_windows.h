#pragma once

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

bool cached_map_file_is_shared(e_map_file_index map_file_index);