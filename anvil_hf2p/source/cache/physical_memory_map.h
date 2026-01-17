#pragma once

enum memory_stage
{
	_memory_stage_initial = 0,
	_memory_stage_game_initialize,
	_memory_stage_resize_initialize,
	_memory_stage_resize_available,
	_memory_stage_map_configuration,
	_memory_stage_level_initialize,
	_memory_stage_resize_locked,
	_memory_stage_in_level,

	k_memory_stage_count
};

extern void* __cdecl _physical_memory_malloc_fixed(memory_stage stage, const char* name, long size, ulong flags);
extern void* __fastcall _physical_memory_malloc(memory_stage stage, const char* name, long size, ulong flags);
extern void __fastcall physical_memory_free(void* memory);
