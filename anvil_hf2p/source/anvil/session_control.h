#pragma once
#include "cseries\cseries.h"

enum e_map_id;
enum e_game_engine_type;
class c_network_session;
bool anvil_session_create();
void anvil_session_update();
bool anvil_session_set_map(e_map_id map_id);
bool anvil_session_set_gamemode(c_network_session* session, e_game_engine_type engine_index, long variant_index, ulong time_limit);