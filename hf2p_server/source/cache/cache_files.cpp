#include "cache_files.h"

void* __fastcall tag_get(tag group_tag, long index)
{
	return INVOKE(0x83C70, tag_get, group_tag, index);
}