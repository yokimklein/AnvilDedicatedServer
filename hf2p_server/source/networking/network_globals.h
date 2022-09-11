#pragma once
#include "..\cseries\cseries.h"
#include "..\dllmain.h"

constexpr long k_network_maximum_sessions = 3;

// belongs in network_time.h
static bool* network_time_locked = (bool*)(module_base + 0x1038344);
static ulong* g_network_locked_time = (ulong*)(module_base + 0x1038348);