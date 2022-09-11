#pragma once
#include "..\..\cseries\cseries.h"
#include "..\..\game\players.h"
#include "..\..\game\player_configuration.h"
#include "..\session\network_session_membership.h"

#pragma pack(push, 1)
struct s_network_message_membership_update_player
{
	ulong player_index;
	ulong update_type;
	bool player_location_updated;
	s_player_identifier identifier;
	byte : 8;
	ushort peer_index;
	ulong player_addition_number;
	bool player_occupies_a_public_slot;
	bool player_properties_updated;
	ushort : 16;
	ulong player_update_number;
	ulong controller_index;
	ulong : 32;
	s_player_configuration player_data;
	ulong player_voice;
	ulong : 32;
};
static_assert(sizeof(s_network_message_membership_update_player) == 0xBA0);
#pragma pack(pop)

struct s_network_message_membership_update_peer_properties
{
	bool peer_name_updated;
	wchar_t peer_name[16]; // offset 60? code uses 58
	wchar_t peer_session_name[32]; // offset 92? code uses 90
	ulong game_start_error;
	bool peer_map_id_updated;
	ulong peer_map_id;
	bool peer_map_updated;
	ulong peer_map_status; // 0-4
	ulong peer_map_progress_percentage; // 0-100
	bool peer_game_instance_exists;
	long64 peer_game_instance;
	bool available_multiplayer_map_mask_updated;
	ulong available_multiplayer_map_mask;
	bool peer_connection_updated;
	ulong connectivity_badness_rating;
	ulong host_badness_rating;
	ulong client_badness_rating;
	ulong language;
	s_network_session_peer_connectivity connectivity;
	bool versions_updated;
	ulong determinism_version;
	ulong determinism_compatible_version;
	bool flags_updated;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_membership_update_peer_properties) == 0xC8);

#pragma pack(push, 8)
struct s_network_message_membership_update_peer
{
	ulong peer_index;
	ulong peer_update_type;
	ulong peer_connection_state;
	bool peer_info_updated;
	s_transport_secure_address peer_address;
	qword peer_party_nonce;
	qword peer_join_nonce;
	ulong network_version_number;
	ulong peer_creation_timestamp;
	bool peer_properties_updated;
	s_network_message_membership_update_peer_properties peer_properties_update;
};
static_assert(sizeof(s_network_message_membership_update_peer) == 0x108);

struct s_network_message_membership_update : s_network_message
{
	s_transport_secure_identifier session_id;
	long update_number;
	long incremental_update_number;
	ulong baseline_checksum;
	ushort peer_update_count; // max 34? number of updates rather than peers
	ushort player_update_count; // max 32? ditto
	s_network_message_membership_update_peer peers[k_network_maximum_machines_per_session];
	s_network_message_membership_update_player players[k_network_maximum_players_per_session];
	bool player_addition_number_updated;
	ulong player_addition_number;
	bool leader_updated;
	ulong leader_peer_index;
	bool host_updated;
	ulong host_peer_index;
	bool slot_counts_updated;
	ulong private_slot_count;
	ulong public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	ulong checksum;
};
static_assert(sizeof(s_network_message_membership_update) == 0xCBD8);
#pragma pack(pop)

struct s_network_message_peer_properties : s_network_message
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address secure_address;
	s_network_session_peer_properties peer_properties;
};
static_assert(sizeof(s_network_message_peer_properties) == 0xC8);

struct s_network_message_delegate_leadership : s_network_message
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address desired_leader_address;
};
static_assert(sizeof(s_network_message_delegate_leadership) == 0x20);

struct s_network_message_boot_machine : s_network_message
{
	s_transport_secure_identifier session_id;
	enum e_network_session_boot_reason reason;
	s_transport_secure_address boot_peer_address;
};
static_assert(sizeof(s_network_message_boot_machine) == 0x24);

#pragma pack(push, 4)
struct s_network_message_player_add : s_network_message
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	ulong player_update_number;
	ulong controller_index;
	s_player_configuration_from_client configuration;
	ulong voice_settings;
};
static_assert(sizeof(s_network_message_player_add) == 0x54);
#pragma pack(pop)

#pragma pack(push, 4)
struct s_network_message_player_refuse : s_network_message
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	e_network_join_refuse_reason refuse_reason;
};
static_assert(sizeof(s_network_message_player_refuse) == 0x1C);
#pragma pack(pop)

struct s_network_message_player_remove : s_network_message
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_player_remove) == 0x10);

struct s_network_message_player_properties : s_network_message
{
	s_transport_secure_identifier session_id;
	ulong player_update_number; // or user_index?
	ulong controller_index;
	s_player_configuration_from_client player_from_client;
	ulong player_voice;
};
static_assert(sizeof(s_network_message_player_properties) == 0x4C);