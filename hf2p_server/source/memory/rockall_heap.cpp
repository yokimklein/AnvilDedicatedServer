#include "rockall_heap.h"
#include "assert.h"

void* c_rockall_heap::allocate(long size)
{
	assert(this->m_rockall_front_end);
    void* result = this->m_rockall_front_end->New(size, 0, 0);
    if (result
        || (this->m_rockall_front_end->Truncate(0),
            (result = this->m_rockall_front_end->New(size, 0, 0)) != 0))
    {
        ++this->count;
    }
    return result;
}

long c_rockall_heap::get_allocation_size(void* block)
{
    int size = 0;
    if (!this->m_rockall_front_end->Details(block, &size))
        size = -1;
    return size;
}

void c_rockall_heap::deallocate(void* block)
{
    assert(this->m_rockall_front_end);
    this->m_rockall_front_end->Delete(block, -1);
    if (block != nullptr)
        this->count--;
}