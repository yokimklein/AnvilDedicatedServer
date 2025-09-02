#include "tag_groups.h"

void* __cdecl tag_block_get_element_with_size(s_tag_block const* block, long index, long size)
{
	ASSERT(block);
	ASSERT(block->count >= 0);
	ASSERT(VALID_INDEX(index, block->count));
	ASSERT(block->address);

	return block->base + index * size;
}