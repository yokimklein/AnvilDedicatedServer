#include "synchronization.h"
#include <windows.h>

REFERENCE_DECLARE(0x3EE11C4, s_synchronization_globals, g_synch_globals);

void internal_critical_section_enter(long critical_section_id)
{
	if (critical_section_id != NONE)
	{
		ASSERT(g_synch_globals.initialized);
		ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
		EnterCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_section[critical_section_id].critical_section);
		c_critical_section_data& data = g_synch_globals.critical_section[critical_section_id];
		data.locking_thread.set(get_current_thread_index());
		data.lock_count.increment();
	}
}

void internal_critical_section_leave(long critical_section_id)
{
	if (critical_section_id != NONE)
	{
		ASSERT(g_synch_globals.initialized);
		ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
		g_synch_globals.critical_section[critical_section_id].lock_count.decrement();
		LeaveCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_section[critical_section_id].critical_section);
	}
}

bool internal_critical_section_try_and_enter(long critical_section_id)
{
	BOOL result = TRUE;
	if (critical_section_id != NONE)
	{
		ASSERT(g_synch_globals.initialized);
		ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
		result = TryEnterCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_section[critical_section_id].critical_section);
		if (result)
		{
			c_critical_section_data& data = g_synch_globals.critical_section[critical_section_id];
			data.locking_thread.set(get_current_thread_index());
			data.lock_count.increment();
		}
	}
	return result != FALSE;
}

void internal_event_reset(long event_id)
{
	ResetEvent(g_synch_globals.sync_event[event_id].handle);
}

void internal_event_set(long event_id)
{
	SetEvent(g_synch_globals.sync_event[event_id].handle);
}

bool internal_event_wait_timeout(long event_id, ulong timeout_in_milliseconds)
{
	return wait_for_single_object_internal(g_synch_globals.sync_event[event_id].handle, timeout_in_milliseconds);
}

long __fastcall internal_semaphore_release(long semaphore_id)
{
    return INVOKE(0x94200, internal_semaphore_release, semaphore_id);
}

void __fastcall internal_semaphore_take(long semaphore_id)
{
    INVOKE(0x941A0, internal_semaphore_take, semaphore_id);
}

bool synchronization_objects_initialized()
{
	return g_synch_globals.initialized;
}

bool wait_for_single_object_internal(void* handle, ulong timeout_in_milliseconds)
{
	DWORD wait_result;
	return !handle || (wait_result = WaitForSingleObjectEx(handle, timeout_in_milliseconds, FALSE)) == 0 || wait_result == WAIT_ABANDONED;
}

c_critical_section_scope::c_critical_section_scope(long critical_section_id)
{
	m_critical_section_id = critical_section_id;
	m_critical_section_entered = false;
	internal_critical_section_enter(m_critical_section_id);
	m_critical_section_entered = true;
}

c_critical_section_scope::c_critical_section_scope(long critical_section_id, long timeout_in_milliseconds, bool* out_lock_acquired)
{
	m_critical_section_id = critical_section_id;
	m_critical_section_entered = false;
	*out_lock_acquired = false;

	ulong end_time = system_milliseconds() + timeout_in_milliseconds;
	while (true)
	{
		m_critical_section_entered = internal_critical_section_try_and_enter(m_critical_section_id);
		if (m_critical_section_entered)
		{
			break;
		}

		switch_to_thread();

		if (system_milliseconds() >= end_time)
		{
			return;
		}
	}

	*out_lock_acquired = true;
}

c_critical_section_scope::~c_critical_section_scope()
{
	if (m_critical_section_entered)
	{
		internal_critical_section_leave(m_critical_section_id);
	}
}
