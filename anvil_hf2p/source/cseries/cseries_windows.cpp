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