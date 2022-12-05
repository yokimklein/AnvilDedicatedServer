#pragma once
#include "..\dllmain.h"
#include "..\cseries\cseries.h"
#include "session\network_session_manager.h"

constexpr long k_network_maximum_sessions = 3;

// belongs in network_time.h
static bool* network_time_locked = (bool*)(module_base + 0x1038344);
static ulong* g_network_locked_time = (ulong*)(module_base + 0x1038348);

// TODO: confirm if this actually is the session manager and not just a single session
static c_network_session_manager* g_network_session_manager = (c_network_session_manager*)(module_base + 0x3970158); // also 0x1039AD4? which is apparently invalid

static short* g_game_port = (short*)(module_base + 0xE9B7A0);

// reach names - may be inaccurate
static bool* g_network_interface_show_latency_and_framerate_metrics_on_chud = (bool*)(module_base + 0x103E768); // enables netdebug scoreboard widget
static bool* g_network_interface_fake_latency_and_framerate_metrics_on_chud = (bool*)(module_base + 0x1038283); // disables debug statistics when true (and sets local text widget to display in green)

static bool* network_globals = (bool*)(module_base + 0x3E81A44);