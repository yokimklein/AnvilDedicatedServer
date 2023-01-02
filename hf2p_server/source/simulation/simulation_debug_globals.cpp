#include "simulation_debug_globals.h"
#include "..\cseries\cseries.h"
#include "..\dllmain.h"

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/simulation/simulation_debug_globals.cpp

REFERENCE_DECLARE(module_base + 0x103E768, bool, g_network_interface_show_latency_and_framerate_metrics_on_chud);
REFERENCE_DECLARE(module_base + 0x1038283, bool, g_network_interface_fake_latency_and_framerate_metrics_on_chud);

bool g_network_debug_globals_latency_and_framerate_metrics_enabled = []() -> bool
{
	g_network_interface_show_latency_and_framerate_metrics_on_chud = true;
	g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;

	return true;
}();