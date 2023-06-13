#include "network_memory.h"
#include "assert.h"
#include <memory\rockall_heap.h>

void* network_heap_allocate_block(long block_size)
{
	assert(block_size > 0);
	assert(network_shared_memory_globals->heap);
	void* rockall_heap = network_shared_memory_globals->heap->allocate(block_size);
	// TODO: finish asserts
	//if (rockall_heap != nullptr)
	//{
	//
	//}
	return rockall_heap;
}

void network_heap_free_block(void* block)
{
	network_shared_memory_globals->heap->get_allocation_size(block);
	network_shared_memory_globals->heap->deallocate(block);
}

// TODO
char* network_heap_describe(char* string, long length)
{
	csnzprintf(string, length, "unknown");
	return string;
}