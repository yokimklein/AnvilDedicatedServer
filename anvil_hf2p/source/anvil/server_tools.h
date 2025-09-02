#pragma once
#include <cseries\cseries.h>
#include <scenario\scenario_map_variant.h>
#include <game\game_engine_variant.h>

enum e_map_id;
enum e_game_engine_type;
class c_network_session;
class c_network_session_membership;
struct s_player_configuration_from_host;
struct s_network_session_parameter_game_start_status;
struct s_transport_secure_address;
struct s_transport_secure_identifier;

struct s_anvil_session_vote_pool
{
	s_anvil_session_vote_pool();

	struct s_map_entry
	{
		c_enum<e_map_id, long, _map_id_none, k_map_id_count> map_id;
		char map_title_instance;

	};
	struct s_gamemode_entry
	{
		c_enum<e_game_engine_type, long, _game_engine_type_none, k_game_engine_type_count> engine_index;
		char variant_index;
		char gamemode_title_instance;
	};
	c_static_array<s_map_entry, 11> map_entries;
	c_static_array<s_gamemode_entry, 3> gamemode_entries;
};

extern s_anvil_session_vote_pool g_anvil_session_vote_pool;

void anvil_initialize();
bool anvil_session_create();
void anvil_session_update();
void anvil_session_start_voting(c_network_session* session);
void anvil_session_update_voting(c_network_session* session);
bool anvil_session_set_map(e_map_id map_id);
bool anvil_session_set_gamemode(c_network_session* session, e_game_engine_type engine_index, long variant_index);
void anvil_session_set_test_player_data(c_network_session_membership* membership);
void anvil_launch_scenario(const char* scenario_path, const wchar_t* map_name);
bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration);
void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status);
bool anvil_key_pressed(long vkey, bool* key_held);

// admin tools
void anvil_boot_peer(long peer_index);
void anvil_power_sister(long peer_index);
