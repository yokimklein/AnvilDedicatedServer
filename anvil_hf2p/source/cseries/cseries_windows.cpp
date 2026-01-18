#include "cseries_windows.h"
#include "cseries\cseries.h"
#include "main\main.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include <timeapi.h>
#include <cstdlib>

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

void system_get_date_and_time(char* buffer, short buffer_size, bool short_date_and_time)
{
	SYSTEMTIME system_time;
	GetLocalTime(&system_time);

	if (short_date_and_time)
	{
		csnzprintf(
			buffer,
			buffer_size,
			"%02d%02d%02d_%02d%02d%02d",
			system_time.wMonth,
			system_time.wDay,
			system_time.wYear % 100,
			system_time.wHour,
			system_time.wMinute,
			system_time.wSecond);
	}
	else
	{
		csnzprintf(
			buffer,
			buffer_size,
			"%02d.%02d.%02d %02d:%02d:%02d.%03d",
			system_time.wMonth,
			system_time.wDay,
			system_time.wYear % 100,
			system_time.wHour,
			system_time.wMinute,
			system_time.wSecond,
			system_time.wMilliseconds);
	}
}

void system_abort()
{
	main_halt_and_catch_fire();
}

void system_exit()
{
	exit(NONE);
}