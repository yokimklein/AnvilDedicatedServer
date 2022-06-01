#pragma once
#include <cstdint>
#include <windows.h>
#include "..\transport\transport_address.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"
#include "..\..\game\player_configuration.h"

enum e_peer_map_status : long
{

};

enum e_network_session_peer_state : long
{
	_network_session_peer_state_none,
	_network_session_peer_state_rejoining,
	_network_session_peer_state_reserved,
	_network_session_peer_state_disconnected,
	_network_session_peer_state_connected,
	_network_session_peer_state_joining,
	_network_session_peer_state_joined,
	_network_session_peer_state_waiting,
	_network_session_peer_state_established,

	k_network_session_peer_state_count
};

static const char* network_session_peer_states[k_network_session_peer_state_count] = {
	"_none",
	"_rejoining",
	"_reserved",
	"_disconnected",
	"_connected",
	"_joining",
	"_joined",
	"_waiting",
	"_established",
};
const char* network_session_peer_state_get_string(e_network_session_peer_state state);

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	long player_index;
	long output_user_index; // is this desired_configuration_version?
	long controller_index;
	s_player_configuration_from_client client_configuration;
	uint32_t voice_settings;
};
static_assert(sizeof(s_player_add_queue_entry) == 0x48);

// this struct shrunk in later builds vs ms23
struct s_network_session_peer_properties
{
	wchar_t peer_name[16];
	wchar_t peer_session_name[32];
	uint32_t peer_mp_map_mask;
	uint32_t peer_map;
	e_peer_map_status peer_map_status;
	uint32_t peer_map_progress_percentage;
	int64_t peer_game_instance;
	uint32_t game_start_error; // originally written as nat_type, but c_network_session_membership::apply_peer_properties_update sets it to update peer properties' game_start_error
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
	uint32_t version;
	uint32_t join_start_time;
	uint32_t unknown;
	s_network_session_peer_properties properties; // 8?
	uint64_t party_nonce;
	uint64_t join_nonce;
	uint32_t player_mask; // contains player count
};
static_assert(sizeof(s_network_session_peer) == 0xE0);

#pragma pack(push, 4)
struct s_network_session_player
{
	long desired_configuration_version;
	s_player_identifier player_identifier;
	long peer_index; // peer index & peer user index as 2 shorts like membership update player?
	long player_sequence_number;
	bool player_occupies_a_public_slot;
	long controller_index;
	long unknown2;
	s_player_configuration configuration;
	uint32_t voice_settings;
	long unknown3;
};
static_assert(sizeof(s_network_session_player) == 0xB98);
#pragma pack(pop)

struct s_network_session_peer_channel
{
	uint32_t flags;
	long channel_index;
	uint32_t expected_update_number; // membership_update_number?
};
static_assert(sizeof(s_network_session_peer_channel) == 0xC);

#pragma pack(push, 4)
struct s_network_session_shared_membership
{
	s_network_session_shared_membership();

	long update_number;
	long leader_peer_index;
	long host_peer_index;
	long private_slot_count;
	long public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	long peer_count;
	uint32_t valid_peer_mask;
	s_network_session_peer peers[k_network_maximum_machines_per_session];
	long player_count;
	uint32_t valid_player_mask;
	s_network_session_player players[k_network_maximum_players_per_session];
	long player_sequence_number;
	long : 32;
};
static_assert(sizeof(s_network_session_shared_membership) == 0xC890);

class c_network_session;
struct s_network_message_membership_update;
struct s_network_message_membership_update_peer_properties;
class c_network_session_membership
{
public:
	long get_first_peer();
	long get_next_peer(long peer_index);
	long get_peer_from_secure_address(s_transport_secure_address const* secure_address);
	bool is_peer_valid(long peer_index);
	bool add_peer(long peer_index, e_network_session_peer_state peer_state, uint32_t joining_network_version_number, s_transport_secure_address const* secure_address, uint64_t join_party_nonce, uint64_t join_nonce);
	long find_or_add_player(long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting);
	void update_player_data(long player_index, s_player_configuration const* player_config);
	long get_peer_from_incoming_address(s_transport_address const* incoming_address);
	void set_peer_connection_state(long peer_index, e_network_session_peer_state state);
	s_network_session_peer* get_peer(long peer_index);
	void set_join_nonce(long peer_index, uint64_t join_nonce);
	void increment_update();
	long get_player_count();
	void idle();
	bool all_peers_established();
	long get_observer_channel_index(long observer_channel_index);
	long get_host_observer_channel_index();
	long host_peer_index();
	long private_slot_count();
	long public_slot_count();
	bool friends_only();
	c_network_session* get_session();
	void set_slot_counts(long private_slot_count, long public_slot_count, bool friends_only);
	bool has_peer_ever_been_established(long peer_index);
	s_network_session_peer* get_raw_peer(long peer_index);
	e_network_session_peer_state get_peer_connection_state(long peer_index);
	void remove_peer(long peer_index);
	s_network_session_shared_membership* get_current_membership();
	s_network_session_shared_membership* get_transmitted_membership(long peer_index);
	void set_membership_update_number(long peer_index, long update_number);
	void build_membership_update(long peer_index, s_network_session_shared_membership* membership, s_network_session_shared_membership* baseline, s_network_message_membership_update* message);
	void build_peer_properties_update(s_network_session_peer_properties* membership_properties, s_network_session_peer_properties* baseline_properties, s_network_message_membership_update_peer_properties* peer_properties_update);
	void set_peer_address(long peer_index, s_transport_secure_address const* secure_address);
	void set_peer_properties(long peer_index, s_network_session_peer_properties const* peer_properties);
	void copy_current_to_transmitted(long peer_index, s_network_session_shared_membership* current_membership);
	uint64_t get_join_nonce(long peer_index);
	bool is_player_in_player_add_queue(s_player_identifier const* player_identifier);
	long find_player_in_player_add_queue(s_player_identifier const* player_identifier);
	void remove_player_from_player_add_queue(s_player_identifier const* player_identifier);
	void commit_player_from_player_add_queue(s_player_identifier const* player_identifier);
	void set_player_properties(long player_index, long desired_configuration_version, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings);

	c_network_session* m_session;
	long unknown1;
	s_network_session_shared_membership m_baseline;
	s_network_session_shared_membership m_transmitted[k_network_maximum_machines_per_session];
	long m_transmitted_checksums[k_network_maximum_machines_per_session];
	long unknown2;
	long m_local_peer_index;
	long m_player_configuration_version;
	s_network_session_peer_channel m_peer_channels[k_network_maximum_machines_per_session]; // local_peer?
	s_player_add_queue_entry m_player_add_queue[4];
	long m_player_add_queue_current_index;
	long m_player_add_queue_count;
	long unknown3;
};
static_assert(sizeof(c_network_session_membership) == 0xE1C70);
#pragma pack(pop)

long fast_checksum_new();
long fast_checksum_s_network_session_shared_membership(long fast_checksum, s_network_session_shared_membership* shared_membership);