#pragma once
#include "tag_files\files_windows.h"
#include "multithreading\synchronized_value.h"

enum e_async_category
{
	_async_category_none = 0,
	_async_category_background_copy,
	_async_category_cache_file_misc,
	_async_category_cache_file_texture,
	_async_category_cache_file_geometry,
	_async_category_cache_file_sound,
	_async_category_cache_file_animation,
	_async_category_text,
	_async_category_saved_games,
	_async_category_online_files,

	k_async_category_count
};

enum e_async_priority
{
	_async_priority_background_idle_loading = 0,
	_async_priority_background_user_task,
	_async_priority_background_important_loading,
	_async_priority_background_important_user_task,
	_async_priority_predicted,
	_async_priority_important_non_blocking,
	_async_priority_blocking_below_sound,
	_async_priority_sound,
	_async_priority_very_important_non_blocking,
	_async_priority_blocking_generic,
	_async_priority_blocking_render,
	_async_priority_blocking_animation,

	k_async_priorities_count
};

extern long __fastcall async_read_position(
	s_file_handle file,
	void* buffer,
	long size,
	ulong offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done);