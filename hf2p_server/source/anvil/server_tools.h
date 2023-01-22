#pragma once
#include "..\game\game_engine.h"
#include "..\networking\session\network_session.h"
#include "..\game\game_time.h"

// TODO: replace this with anvil_get_update_rate_ms
constexpr long k_anvil_update_rate_ms = 1000 / 60;
const static wchar_t k_anvil_machine_name[16] = L"ANVIL_DEDICATED";
const static wchar_t k_anvil_session_name[32] = L"ANVIL_DEDICATED";

bool anvil_create_session();
bool anvil_session_set_map(e_map_id map_id);
bool anvil_session_set_gamemode(c_network_session* session, e_engine_variant engine_variant);
void anvil_session_set_test_player_data(c_network_session_membership* membership);
void anvil_launch_scenario(const char* scenario_path);

bool anvil_key_pressed(long vkey, bool* key_held);
long anvil_get_update_rate_ms();

void anvil_get_dedicated_secure_address(s_transport_secure_address* secure_address);
void anvil_get_dedicated_secure_identifier(s_transport_secure_identifier* secure_identifier);