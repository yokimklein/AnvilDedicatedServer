#include "shell_windows.h"
#include "cseries\cseries.h"
#include <multithreading\threads.h>

REFERENCE_DECLARE(0x10382B0, s_windows_params, g_windows_params);;

LONG __stdcall TopLevelExceptionFilter(_EXCEPTION_POINTERS* exception_pointers)
{
#ifndef RELEASE_ENABLED
	if (!is_main_thread())
	{
		cache_exception_information(exception_pointers);
		PostThreadMessageA(get_main_thread_id(), WM_NULL, 0, 0);
	
		while (true)
		{
			Sleep(1);
		}
	}
	return generic_exception_filter(exception_pointers->ExceptionRecord->ExceptionCode, exception_pointers);
#else
	cache_exception_information(exception_pointers);
	crashdump_from_exception();
	return 0;
#endif
}
