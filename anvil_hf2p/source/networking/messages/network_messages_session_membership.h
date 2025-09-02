#pragma once
#include <cseries\cseries.h>
#include <game\players.h>
#include <game\player_configuration.h>
#include <networking\transport\transport_security.h>
#include <networking\session\network_session_membership.h>
#include <networking\logic\network_join.h>

#pragma pack(push, 1)
struct s_network_message_membership_update_player
{
	long player_index;
	long update_type;
	bool player_location_updated;
	s_player_identifier identifier;
	byte : 8;
	short peer_index;
	long player_addition_number;
	bool player_occupies_a_public_slot;
	bool player_properties_updated;
	short peer_user_index;
	long player_update_number;
	long controller_index;
	long : 32;
	s_player_configuration player_data;
	ulong player_voice;
	ulong : 32;
};
static_assert(sizeof(s_network_message_membership_update_player) == 0xBA0);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_index) == 0x00);
static_assert(OFFSETOF(s_network_message_membership_update_player, update_type) == 0x04);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_location_updated) == 0x08);
static_assert(OFFSETOF(s_network_message_membership_update_player, identifier) == 0x09);
static_assert(OFFSETOF(s_network_message_membership_update_player, peer_index) == 0x12);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_addition_number) == 0x14);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_occupies_a_public_slot) == 0x18);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_properties_updated) == 0x19);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_update_number) == 0x1C);
static_assert(OFFSETOF(s_network_message_membership_update_player, controller_index) == 0x20);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_data) == 0x28);
static_assert(OFFSETOF(s_network_message_membership_update_player, player_voice) == 0xB98);
#pragma pack(pop)

struct s_network_message_membership_update_peer_properties
{
	bool peer_name_updated;
	c_static_wchar_string<16> peer_name;
	c_static_wchar_string<32> peer_session_name;
	ulong game_start_error;
	bool peer_map_id_updated;
	ulong peer_map_id;
	bool peer_map_updated;
	c_enum<e_network_session_map_status, long, _network_session_map_status_none, k_network_session_map_status_count> peer_map_status;
	ulong peer_map_progress_percentage; // 0-100
	bool peer_game_instance_updated;
	ulong64 peer_game_instance;
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

struct s_network_message_membership_update_peer
{
	ulong peer_index;
	ulong peer_update_type;
	e_network_session_peer_state peer_connection_state;
	bool peer_info_updated;
	s_transport_secure_address peer_address;
	byte pad1[3];
	qword peer_party_nonce;
	qword peer_join_nonce;
	ulong network_version_number;
	ulong peer_creation_timestamp;
	bool peer_properties_updated;
	byte pad2[7];
	s_network_message_membership_update_peer_properties peer_properties;
};
static_assert(sizeof(s_network_message_membership_update_peer) == 0x108);

struct s_network_message_membership_update
{
	s_transport_secure_identifier session_id;
	long update_number;
	long incremental_update_number;
	ulong baseline_checksum;
	ushort peer_update_count; // max 34? number of updates rather than peers
	ushort player_update_count; // max 32? ditto
	c_static_array<s_network_message_membership_update_peer, k_network_maximum_machines_per_session> peer_updates;
	c_static_array<s_network_message_membership_update_player, k_network_maximum_players_per_session> player_updates;
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
	long : 32;
};
static_assert(sizeof(s_network_message_membership_update) == 0xCBD8);

struct s_network_message_peer_properties
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address secure_address;
	s_network_session_peer_properties peer_properties;
};
static_assert(sizeof(s_network_message_peer_properties) == 0xC8);

struct s_network_message_delegate_leadership
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address desired_leader_address;
};
static_assert(sizeof(s_network_message_delegate_leadership) == 0x20);

struct s_network_message_boot_machine
{
	s_transport_secure_identifier session_id;
	enum e_network_session_boot_reason reason;
	s_transport_secure_address boot_peer_address;
};
static_assert(sizeof(s_network_message_boot_machine) == 0x24);

struct s_network_message_player_add
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	long user_index;
	long controller_index;
	s_player_configuration_from_client player_data;
	long player_voice;
};
static_assert(sizeof(s_network_message_player_add) == 0x54);

struct s_network_message_player_refuse
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	e_network_join_refuse_reason join_refusal_reason;
};
static_assert(sizeof(s_network_message_player_refuse) == 0x1C);

struct s_network_message_player_remove
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_player_remove) == 0x10);

struct s_network_message_player_properties
{
	s_transport_secure_identifier session_id;
	long player_update_number;
	long controller_index;
	s_player_configuration_from_client player_from_client;
	long player_voice;
};
static_assert(sizeof(s_network_message_player_properties) == 0x4C);