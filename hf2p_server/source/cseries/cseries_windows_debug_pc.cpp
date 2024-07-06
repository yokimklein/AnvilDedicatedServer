#include "cseries_windows_debug_pc.h"
#include <cseries\cseries.h>

long __cdecl exceptions_update()
{
	return INVOKE(0x167CF0, exceptions_update);
}