#pragma once
#include "..\game\game_engine.h"
#include "..\networking\session\network_session.h"

// TODO: replace with game_time_globals->tick_rate
constexpr long k_game_tick_rate = 1000 / 60;

bool anvil_create_session();
bool anvil_session_set_map(e_map_id map_id);
bool anvil_session_set_gamemode(c_network_session* session, e_engine_variant engine_variant);
void anvil_session_set_test_player_data(c_network_session_membership* membership);

bool anvil_key_pressed(long vkey, bool* key_held);