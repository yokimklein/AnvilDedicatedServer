#pragma once
#include "..\cseries\cseries.h"

class c_ring_buffer
{
	long ring_size;
	void* elements;
	long element_count;
};
static_assert(sizeof(c_ring_buffer) == 0xC);