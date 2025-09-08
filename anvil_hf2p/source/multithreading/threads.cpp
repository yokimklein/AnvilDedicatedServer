#include "threads.h"
#include "memory\tls.h"
#include <windows.h>

REFERENCE_DECLARE(0x1044AD0, s_thread_system_globals, g_thread_globals);
REFERENCE_DECLARE(0x49B1298, c_interlocked_long, g_thread_owning_device);

bool is_main_thread()
{
    TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
    return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_main;
}

bool switch_to_thread()
{
	return SwitchToThread() == TRUE;
}

long get_current_thread_index()
{
    TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
    return g_registered_thread_index;
}
