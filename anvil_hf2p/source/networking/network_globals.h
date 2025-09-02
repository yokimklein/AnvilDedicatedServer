#pragma once
#include <cseries\cseries.h>

constexpr long k_network_maximum_sessions = 3;

struct s_network_globals
{
	bool initialized;
	bool entered;

	// halo 3: thread_id = system_get_current_thread_id()
	// halo reach: thread_index = get_current_thread_index()
	// halo online: unused
	unsigned long thread_id;
};

class c_network_session_manager;

extern s_network_globals& network_globals;
extern short& g_game_port;
extern c_network_session_manager*& g_network_session_manager;