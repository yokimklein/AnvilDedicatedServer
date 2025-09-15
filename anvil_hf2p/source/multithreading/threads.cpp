#include "threads.h"
#include "memory\tls.h"
#include "multithreading\synchronization.h"
#include <windows.h>

REFERENCE_DECLARE(0x1044AD0, s_thread_system_globals, g_thread_globals);
REFERENCE_DECLARE(0x49B1298, c_interlocked_long, g_thread_owning_device);

c_interlocked_long thread_should_assert[k_registered_thread_count]{};
c_interlocked_long thread_should_crash[k_registered_thread_count]{};

bool current_thread_should_exit()
{
	long thread_index = get_current_thread_index();
	if (!VALID_INDEX(thread_index, k_registered_thread_count))
	{
		VASSERT("invalid thread index");
	}
	return g_thread_globals.thread_should_exit[thread_index].peek() == TRUE;
}

bool current_thread_should_assert()
{
	long thread_index = get_current_thread_index();
	if (VALID_INDEX(thread_index, k_registered_thread_count))
	{
		return thread_should_assert[thread_index].peek() == TRUE;
	}

	return false;
}

bool current_thread_should_crash()
{
	long thread_index = get_current_thread_index();
	if (VALID_INDEX(thread_index, k_registered_thread_count))
	{
		return thread_should_crash[thread_index].peek() == TRUE;
	}

	return false;
}

void current_thread_update_test_functions()
{
	ASSERT(!current_thread_should_assert());
	if (current_thread_should_crash())
	{
		*(const char**)NULL = "Forced crash to test the minidump system, this is awesome";
	}
}

bool is_main_thread()
{
    TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
    return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_main;
}

void __fastcall start_thread(e_registered_threads thread_index)
{
	INVOKE(0xA5D20, start_thread, thread_index);
	//g_thread_globals.thread_should_exit[thread_index].set(FALSE);
	//g_thread_globals.thread_has_crashed[thread_index].set(FALSE);
	//initialize_thread(thread_index);
}

void stop_thread(e_registered_threads thread_index)
{
    if (registered_thread_handle_valid(thread_index) && wait_for_thread_to_exit(thread_index, INFINITE))
	{
		thread_destroy(g_thread_globals.thread_handle[thread_index]);
		unregister_thread(thread_index);
	}
}

bool switch_to_thread()
{
	return SwitchToThread() == TRUE;
}

ulong __stdcall thread_execution_wrapper(void* parameter)
{
	return ulong();
}

bool thread_has_crashed(e_registered_threads thread_index)
{
	return g_thread_globals.thread_has_crashed[thread_index].peek() == TRUE;
}

void unregister_thread(long thread_index)
{
	ASSERT(VALID_INDEX(thread_index, k_registered_thread_count));
	g_thread_globals.thread_handle[thread_index] = INVALID_HANDLE_VALUE;
	g_thread_globals.thread_id[thread_index] = NONE;
	g_thread_globals.thread_user_data[thread_index] = NULL;
}

bool wait_for_thread_to_exit(long thread_index, ulong timeout_in_milliseconds)
{
	ASSERT(g_thread_globals.initialized.peek() != FALSE, "thread management has not been initialized");
	ASSERT(VALID_INDEX(thread_index, k_registered_thread_count), "invalid thread index");
	g_thread_globals.thread_should_exit[thread_index].set(TRUE);
	if (timeout_in_milliseconds == INFINITE)
	{
		return wait_for_single_object_internal(g_thread_globals.thread_handle[thread_index], INFINITE);
	}
	return wait_for_single_object_internal(g_thread_globals.thread_handle[thread_index], timeout_in_milliseconds);
}

long get_current_thread_index()
{
    TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
    return g_registered_thread_index;
}

bool registered_thread_handle_valid(e_registered_threads thread_index)
{
	return g_thread_globals.thread_handle[thread_index] != INVALID_HANDLE_VALUE;
}

void thread_destroy(void* thread_handle)
{
	CloseHandle(thread_handle);
}