#include "rockall_heap.h"
#include <cseries\cseries.h>

void* c_rockall_heap::allocate(long size)
{
	ASSERT(m_rockall_front_end);
    void* result = m_rockall_front_end->New(size, 0, 0);
    if (result || (m_rockall_front_end->Truncate(0), (result = m_rockall_front_end->New(size, 0, 0)) != 0))
    {
        ++count;
    }
    return result;
}

long c_rockall_heap::get_allocation_size(void* block)
{
    int size = 0;
    if (!m_rockall_front_end->Details(block, &size))
    {
        size = -1;
    }
    return size;
}

void c_rockall_heap::deallocate(void* block)
{
    ASSERT(m_rockall_front_end);
    m_rockall_front_end->Delete(block, -1);
    if (block)
    {
        count--;
    }
}