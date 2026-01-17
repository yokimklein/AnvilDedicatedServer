#include "physical_memory_map.h"
#include "cseries\cseries.h"

void* __cdecl _physical_memory_malloc_fixed(memory_stage stage, const char* name, long size, ulong flags)
{
	// stage & name unused
	return DECLFUNC(0xA0580, void*, __cdecl, long, ulong)(size, flags);
}

void* __fastcall _physical_memory_malloc(memory_stage stage, const char* name, long size, ulong flags)
{
	return _physical_memory_malloc_fixed(stage, name, size, flags);
}

void __fastcall physical_memory_free(void* memory)
{
	// empty
}
