#include "cseries_windows.h"
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>

ulong system_seconds()
{
    return static_cast<ulong>(_time64(0));
}

ulong system_milliseconds()
{
	return timeGetTime();
}

void display_debug_string(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	if (IsDebuggerPresent())
	{
		c_static_string<4096> output = {};
		output.append_print_va(format, list);
		output.append("\n");

		OutputDebugStringA(output.get_string());
	}

	va_end(list);
}