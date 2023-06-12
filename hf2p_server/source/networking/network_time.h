#pragma once
#include "..\cseries\cseries.h"

// belongs in network_time.h
static bool* network_time_locked = (bool*)(module_base + 0x1038344); // network_time_globals
static ulong* g_network_locked_time = (ulong*)(module_base + 0x1038348); // belongs in network_time_globals + 4

ulong network_time_get();
ulong network_time_get_exact();
ulong system_milliseconds();
ulong network_time_difference_msec(ulong start_time, ulong target_time);
ulong network_time_since(ulong since);