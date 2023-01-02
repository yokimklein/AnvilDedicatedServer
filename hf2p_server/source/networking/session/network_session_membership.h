#pragma once
#include "..\..\cseries\cseries.h"
#include "..\transport\transport_security.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"
#include "..\..\game\player_configuration.h"
#include <cstring>

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

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	long player_index;
	long player_update_number;
	long controller_index;
	s_player_configuration_from_client client_configuration;
	ulong voice_settings;
};
static_assert(sizeof(s_player_add_queue_entry) == 0x48);

struct s_network_session_peer_connectivity
{
	ushort peer_connectivity_mask;
	ushort peer_probe_mask;
	ulong latency_minimum_msec;
	ulong latency_average_msec;
	ulong latency_maximum_msec;
};
static_assert(sizeof(s_network_session_peer_connectivity) == 0x10);

// this struct shrunk in later builds vs ms23
struct s_network_session_peer_properties
{
	wchar_t peer_name[16];
	wchar_t peer_session_name[32];
	ulong peer_mp_map_mask;
	ulong peer_map;
	e_peer_map_status peer_map_status;
	ulong peer_map_progress_percentage;
	qword peer_game_instance;
	ulong game_start_error;
	ulong connectivity_badness_rating;
	ulong host_badness_rating;
	ulong client_badness_rating;
	s_network_session_peer_connectivity connectivity;
	ulong language;
	ulong determinism_version;
	ulong determinism_compatible_version;
	ulong flags;
};
static_assert(sizeof(s_network_session_peer_properties) == 0xA8);

struct s_network_session_peer
{
	s_transport_secure_address secure_address;
	e_network_session_peer_state connection_state;
	ulong version;
	ulong join_start_time;
	ulong unknown;
	s_network_session_peer_properties properties; // 8?
	qword party_nonce;
	qword join_nonce;
	ulong player_mask;
};
static_assert(sizeof(s_network_session_peer) == 0xE0);

#pragma pack(push, 1)
struct s_network_session_player
{
	long desired_configuration_version;
	s_player_identifier player_identifier;
	long peer_index;
	long player_sequence_number;
	bool player_occupies_a_public_slot;
	byte pad[3];
	long controller_index;
	long unknown2;
	s_player_configuration configuration;
	long voice_settings;
	long unknown3;
};
static_assert(sizeof(s_network_session_player) == 0xB98);
#pragma pack(pop)

#pragma pack(push, 1)
struct s_network_session_peer_channel
{
	bool needs_reestablishment;
	byte unknown1;
	byte unknown2;
	byte unknown3;
	long channel_index;
	ulong expected_update_number;
};
static_assert(sizeof(s_network_session_peer_channel) == 0xC);
#pragma pack(pop)

#pragma pack(push, 1)
struct s_network_session_shared_membership
{
	long update_number;
	long leader_peer_index;
	long host_peer_index;
	long private_slot_count;
	long public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	byte pad[2];
	long peer_count;
	ulong peer_valid_mask;
	s_network_session_peer peers[k_network_maximum_machines_per_session];
	long player_count;
	ulong player_valid_flags;
	s_network_session_player players[k_network_maximum_players_per_session];
	long player_sequence_number;
	long : 32;
};
static_assert(sizeof(s_network_session_shared_membership) == 0xC890);
#pragma pack(pop)

class c_network_session;
struct s_network_message_membership_update;
struct s_network_message_membership_update_peer_properties;

#pragma pack(push, 4)
class c_network_session_membership
{
public:
	long get_first_peer();
	long get_next_peer(long peer_index);
	long get_first_player();
	long get_next_player(long peer_index);
	long get_peer_from_secure_address(s_transport_secure_address const* secure_address);
	bool is_peer_valid(long peer_index);
	bool is_player_valid(long player_index);
	bool add_peer(long peer_index, e_network_session_peer_state peer_state, ulong joining_network_version_number, s_transport_secure_address const* secure_address, qword join_party_nonce, qword join_nonce);
	long find_or_add_player(long peer_index, s_player_identifier const* player_identifier, bool join_from_recruiting);
	s_network_session_player* add_player_internal(long player_index, s_player_identifier const* player_identifier, long peer_index, long player_sequence_number, bool player_occupies_a_public_slot);
	void update_player_data(long player_index, s_player_configuration const* player_config);
	long get_peer_from_incoming_address(s_transport_address const* incoming_address);
	void set_peer_connection_state(long peer_index, e_network_session_peer_state state);
	s_network_session_peer* get_peer(long peer_index);
	s_network_session_player* get_player(long player_index);
	void set_join_nonce(long peer_index, qword join_nonce);
	void increment_update();
	long get_player_count();
	void idle();
	bool all_peers_established();
	long get_observer_channel_index(long peer_index);
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
	qword get_join_nonce(long peer_index);
	bool is_player_in_player_add_queue(s_player_identifier const* player_identifier);
	long find_player_in_player_add_queue(s_player_identifier const* player_identifier);
	void remove_player_from_player_add_queue(s_player_identifier const* player_identifier);
	void commit_player_from_player_add_queue(s_player_identifier const* player_identifier);
	void set_player_properties(long player_index, long desired_configuration_version, long controller_index, s_player_configuration_from_client const* player_data_from_client, long voice_settings);
	long get_creation_timestamp(long peer_index);
	s_player_add_queue_entry* get_first_player_from_player_add_queue();
	void remove_player(long player_index);
	void remove_player_internal(long player_index);
	long get_player_from_identifier(s_player_identifier const* player_identifier);
	bool add_player_to_player_add_queue(s_player_identifier const* player_identifier, long peer_index, long peer_user_index, long controller_index, s_player_configuration_from_client* player_data_from_client, long voice_settings);
	long local_peer_index();
	long get_peer_count();
	void set_peer_needs_reestablishment(long peer_index, bool flags);
	s_transport_secure_address* get_peer_address(long peer_index);
	long get_peer_from_observer_channel(long channel_index);
	bool host_exists_at_incoming_address(s_transport_address const* incoming_address);
	long leader_peer_index();
	bool handle_membership_update(s_network_message_membership_update const* message);
	long update_number();
	long get_membership_update_number(long peer_index);
	bool get_peer_needs_reestablishment(long peer_index);
	bool has_membership();

	c_network_session* m_session;
	long unknown1;
	s_network_session_shared_membership m_baseline;
	s_network_session_shared_membership m_transmitted[k_network_maximum_machines_per_session];
	long m_transmitted_checksums[k_network_maximum_machines_per_session]; // m_transmitted_updates
	bool unknown2; // membership reset bool? membership initialised?
	long m_local_peer_index;
	long m_player_configuration_version;
	s_network_session_peer_channel m_peer_channels[k_network_maximum_machines_per_session];
	s_player_add_queue_entry m_player_add_queue[4];
	long m_player_add_queue_current_index;
	long m_player_add_queue_count;
	long unknown3;
};
static_assert(sizeof(c_network_session_membership) == 0xE1C70);
#pragma pack(pop)

const char* network_session_peer_state_get_string(e_network_session_peer_state state);