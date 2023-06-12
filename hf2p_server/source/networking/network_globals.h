#pragma once
#include "..\cseries\cseries.h"
#include "session\network_session_manager.h"

constexpr long k_network_maximum_sessions = 3;
// reach replaced this with get_network_protocol_version()
constexpr long k_network_protocol_version = 9; // H3 is 7, ODST is 8

// TODO: confirm if this actually is the session manager and not just a single session
static c_network_session_manager* g_network_session_manager = (c_network_session_manager*)(module_base + 0x3970158); // also 0x1039AD4? which is apparently invalid

static short* g_game_port = (short*)(module_base + 0xE9B7A0);
static bool* network_globals = (bool*)(module_base + 0x3E81A44);