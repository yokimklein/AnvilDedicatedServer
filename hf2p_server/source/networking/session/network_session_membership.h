#pragma once
#include <cstdint>
#include <windows.h>
#include "..\transport\transport_address.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"

struct s_player_configuration_from_client
{
	wchar_t player_name[16];
	byte multiplayer_team;
	byte unknown21;
	byte active_armor_loadout;
	byte active_weapon_loadout;
	uint32_t hopper_access_flags;
	uint32_t cheating_flags;
	uint32_t user_experience_flags;
};

struct s_player_configuration_from_host
{
	s_machine_identifier machine_identifier;
	wchar_t player_name[16];
	int team;
	int player_assigned_team;
	byte data30[1632];
	byte data690[1172];
	byte data[28];
};

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	int player_index;
	int output_user_index;
	int controller_index;
	s_player_configuration_from_client client_configuration;
	uint32_t voice_settings;
};

struct s_player_configuration
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};

struct s_network_session_peer
{
	s_transport_secure_address secure_address;
	uint32_t connection_state;
	char data[204];
};

struct s_network_session_player
{
	int desired_configuration_version;
	s_player_identifier player_identifier;
	int peer_index;
	int player_sequence_number;
	long : 32;
	int controller_index;
	s_player_configuration configuration;
	uint32_t voice_settings;
};

struct s_network_session_peer_channel
{
	uint32_t flags;
	int channel_index;
	uint32_t expected_update_number;
};

class c_network_session;
class c_network_session_membership
{
public:
	c_network_session* m_session;
	long : 32;
	int m_baseline_update_number;
	int m_leader_peer_index;
	int m_host_peer_index;
	int m_private_slot_count;
	int m_public_slot_count;
	bool m_friends_only;
	int m_peer_count;
	uint32_t m_valid_peer_mask;
	s_network_session_peer m_peers[k_network_maximum_machines_per_session];
	int m_player_count;
	uint32_t m_valid_player_mask;
	s_network_session_player m_players[k_network_maximum_players_per_session];
	int m_player_sequence_number;
	long : 32;
	char m_incremental_update_buffers[k_network_maximum_machines_per_session][51344];
	int m_incremental_updates[k_network_maximum_machines_per_session];
	long : 32;
	int m_local_peer_index;
	int m_player_configuration_version;
	long : 32;
	s_network_session_peer_channel m_peer_channels[k_network_maximum_machines_per_session];
	s_player_add_queue_entry m_player_add_queue[4];
	int m_player_add_queue_current_index;
	int m_player_add_queue_count;

private:

};

