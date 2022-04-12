#pragma once
#include <cstdint>
#include <windows.h>
#include "..\transport\transport_security.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"

enum e_peer_map_status : long
{

};

enum e_network_session_peer_state : long
{

};

struct s_player_configuration_from_client
{
	wchar_t player_name[16];
	byte multiplayer_team;
	byte unknown_team;
	byte active_armor_loadout;
	byte active_weapon_loadout;
	uint32_t hopper_access_flags;
	uint32_t cheating_flags;
	uint32_t user_experience_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_machine_identifier machine_identifier;
	wchar_t player_name[16];
	long team;
	long player_assigned_team;
	byte data[0xB08];
};
static_assert(sizeof(s_player_configuration_from_host) == 0xB40);

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	long player_index;
	long output_user_index;
	long controller_index;
	s_player_configuration_from_client client_configuration;
	uint32_t voice_settings;
};
static_assert(sizeof(s_player_add_queue_entry) == 0x48);

struct s_player_configuration
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0xB70);

struct s_network_session_peer_properties
{
	wchar_t peer_name[16];
	wchar_t peer_session_name[32];
	uint32_t peer_mp_map_mask;
	uint32_t peer_map;
	e_peer_map_status peer_map_status;
	uint32_t peer_map_progress_percentage;
	int64_t peer_game_instance;
	uint32_t nat_type;
	uint32_t connectivity_badness_rating;
	uint32_t host_badness_rating;
	uint32_t client_badness_rating;
	uint16_t peer_connectivity_mask;
	uint16_t peer_probe_mask;
	uint32_t peer_latency_min;
	uint32_t peer_latency_est;
	uint32_t peer_latency_max;
	uint32_t language;
	uint32_t determinism_version;
	uint32_t determinism_compatible_version;
	uint32_t flags;
};
static_assert(sizeof(s_network_session_peer_properties) == 0xA8);

struct s_network_session_peer
{
	s_transport_secure_address secure_address;
	e_network_session_peer_state connection_state;
	long version;
	long join_start_time;
	long unknown;
	s_network_session_peer_properties properties;
	uint64_t unknown_nonce;
	uint64_t join_nonce;
	uint32_t player_mask; // contains player count
};
static_assert(sizeof(s_network_session_peer) == 0xE0);

struct s_network_session_player
{
	long desired_configuration_version;
	s_player_identifier player_identifier;
	long peer_index;
	long player_sequence_number;
	long : 32;
	long controller_index;
	s_player_configuration configuration;
	uint32_t voice_settings;
};
static_assert(sizeof(s_network_session_player) == 0xB90);

struct s_network_session_peer_channel
{
	uint32_t flags;
	long channel_index;
	uint32_t expected_update_number;
};
static_assert(sizeof(s_network_session_peer_channel) == 0xC);

class c_network_session;
class c_network_session_membership
{
public:
	long get_first_peer();
	long get_next_peer(long peer_index);
	long get_peer_from_secure_address(s_transport_secure_address const* secure_address);

	c_network_session* m_session;
	long : 32;
	long m_baseline_update_number;
	long m_leader_peer_index;
	long m_host_peer_index;
	long m_private_slot_count;
	long m_public_slot_count;
	bool m_friends_only;
	long m_peer_count;
	uint32_t m_valid_peer_mask;
	s_network_session_peer m_peers[k_network_maximum_machines_per_session];
	long m_player_count;
	uint32_t m_valid_player_mask;
	s_network_session_player m_players[k_network_maximum_players_per_session];
	long m_player_sequence_number;
	long : 32;
	char m_incremental_update_buffers[k_network_maximum_machines_per_session][0xC890];
	long m_incremental_updates[k_network_maximum_machines_per_session];
	long : 32;
	long m_local_peer_index;
	long m_player_configuration_version;
	s_network_session_peer_channel m_peer_channels[k_network_maximum_machines_per_session];
	s_player_add_queue_entry m_player_add_queue[4];
	long m_player_add_queue_current_index;
	long m_player_add_queue_count;
	byte size_hack[0x80]; // TODO - complete this struct properly
};
static_assert(sizeof(c_network_session_membership) == 0xE1C70); // missing an extra 0x80 bytes (currently 0xE1AE0 w/o the hack)