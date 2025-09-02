#include "network_memory.h"
#include <memory\rockall_heap.h>

REFERENCE_DECLARE(0x1038284, s_network_shared_memory_globals, network_shared_memory_globals);
REFERENCE_DECLARE(0x394BA58, s_network_base_memory_globals, network_base_memory_globals);

void* network_heap_allocate_block(long block_size)
{
	ASSERT(block_size > 0);
	ASSERT(network_shared_memory_globals.heap);
	void* rockall_heap = network_shared_memory_globals.heap->allocate(block_size);
	long allocation_size = network_shared_memory_globals.heap->get_allocation_size(rockall_heap);
	ASSERT(allocation_size > 0);
	ASSERT(allocation_size >= block_size);
	// debug only
	//ASSERT(block_type >= 0 && block_type < k_network_memory_block_count);

	return rockall_heap;
}

void network_heap_free_block(void* block)
{
	ASSERT(block!=NULL);
	long block_size = network_shared_memory_globals.heap->get_allocation_size(block);
	ASSERT(block_size > 0);
	// debug only
	//ASSERT(block_type >= 0 && block_type < k_network_memory_block_count);
	//ASSERT(network_shared_memory_globals.total_status.allocation_count > 0);
	//ASSERT(network_shared_memory_globals.total_status.total_size_bytes>=block_size);
	//ASSERT(network_shared_memory_globals.block_status[block_type].allocation_count>0);
	//ASSERT(network_shared_memory_globals.block_status[block_type].total_size_bytes>=block_size);

	network_shared_memory_globals.heap->get_allocation_size(block);
	network_shared_memory_globals.heap->deallocate(block);
}

// $TODO:
char* network_heap_describe(char* string, long length)
{
	csnzprintf(string, length, "unknown");
	return string;
}