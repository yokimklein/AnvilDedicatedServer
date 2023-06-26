#pragma once
#include <game\game_engine.h>
#include <networking\session\network_session.h>
#include <game\game_time.h>

const static wchar_t k_anvil_machine_name[16] = L"ANVIL_DEDICATED";
const static wchar_t k_anvil_session_name[32] = L"ANVIL_DEDICATED";

void enable_memory_write(dword base);
void anvil_initialize();
bool anvil_create_session();
void anvil_session_update();
bool anvil_session_set_map(e_map_id map_id);
bool anvil_session_set_gamemode(c_network_session* session, e_engine_variant engine_variant);
void anvil_session_set_test_player_data(c_network_session_membership* membership);
void anvil_launch_scenario(const char* scenario_path);
bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration);
void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status);

bool anvil_key_pressed(long vkey, bool* key_held);

void anvil_get_dedicated_secure_address(s_transport_secure_address* secure_address);
void anvil_get_dedicated_secure_identifier(s_transport_secure_identifier* secure_identifier);