#pragma once
#include "cseries\cseries.h"

class c_network_session;
void anvil_session_start_voting(c_network_session* session);
void anvil_session_update_voting(c_network_session* session);

extern dword g_anvil_return_from_game_time;