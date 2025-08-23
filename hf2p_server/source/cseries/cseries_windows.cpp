#include "cseries_windows.h"
#include <time.h>

dword system_seconds()
{
    return static_cast<dword>(_time64(0));
}
