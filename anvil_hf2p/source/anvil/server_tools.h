#pragma once

class c_network_session;
struct s_player_configuration_from_host;
struct s_network_session_parameter_game_start_status;

void anvil_launch_scenario(const char* scenario_path, const wchar_t* map_name);
bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration);
void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status);

// admin tools
void anvil_boot_peer(long peer_index);
void anvil_power_sister(long peer_index);
