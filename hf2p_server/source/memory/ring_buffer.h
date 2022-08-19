#pragma once

class c_ring_buffer
{
	int ring_size;
	void* elements;
	int element_count;
};
static_assert(sizeof(c_ring_buffer) == 0xC);