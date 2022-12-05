#pragma once
#include "..\game\game_engine.h"
#include "..\networking\session\network_session.h"

constexpr long k_anvil_dedi_update_rate = 1000 / 60;

bool anvil_dedi_create_session();
bool anvil_dedi_set_map(e_map_id map_id);
bool anvil_dedi_set_gamemode(c_network_session* session, e_engine_variant engine_variant);
void anvil_dedi_test_set_player_data(c_network_session_membership* membership);
bool anvil_dedi_test_key_pressed(long vkey, bool* key_held);