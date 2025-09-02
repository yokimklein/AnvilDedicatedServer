#pragma once
#include <cseries\cseries.h>

class c_ring_buffer
{
	long ring_size;
	void* elements;
	long element_count;
};
static_assert(sizeof(c_ring_buffer) == 0xC);

template<typename t_type, long k_maximum_count>
struct t_static_ring_buffer : public c_ring_buffer
{
	t_type elements[k_maximum_count];
};
static_assert(sizeof(t_static_ring_buffer<long, 16>) == 0x4C);