#include <memory\read_write_lock.h>
#include <multithreading\synchronization.h>

c_read_write_lock::c_read_write_lock() :
	m_write_lock(k_invalid_critical_section),
	m_read_lock(k_invalid_semaphore),
	m_max_readers(NONE)
{
}

c_read_write_lock::~c_read_write_lock()
{
	m_write_lock = k_invalid_critical_section;
	m_read_lock = k_invalid_semaphore;
	m_max_readers = NONE;
}

void c_read_write_lock::read_lock()
{
	internal_semaphore_take(m_read_lock);
}

void c_read_write_lock::read_unlock()
{
	internal_semaphore_release(m_read_lock);
}

void c_read_write_lock::setup(long critcal_section_index, long semaphore_index)
{
	INVOKE_CLASS_MEMBER(0x1B49B0, c_read_write_lock, setup, critcal_section_index, semaphore_index);
}

void c_read_write_lock::write_lock()
{
	INVOKE_CLASS_MEMBER(0x1B4A50, c_read_write_lock, write_lock);
}

void c_read_write_lock::write_unlock()
{
	INVOKE_CLASS_MEMBER(0x1B4B30, c_read_write_lock, write_unlock);
}

