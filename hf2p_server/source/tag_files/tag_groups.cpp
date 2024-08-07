#include "tag_groups.h"

void* __cdecl tag_block_get_element_with_size(s_tag_block const* block, long index, long size)
{
	assert(block);
	assert(block->count >= 0);
	assert(VALID_INDEX(index, block->count));
	assert(block->address);

	return block->base + index * size;
}