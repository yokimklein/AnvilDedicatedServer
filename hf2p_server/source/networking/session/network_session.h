#pragma once
#include <cseries\cseries.h>
#include <networking\session\network_session_membership.h>
#include <networking\session\network_session_parameters.h>
#include <networking\session\network_session_parameters_session.h>
#include <networking\session\network_observer.h>
#include <networking\messages\network_messages_session_membership.h>
#include <networking\messages\network_messages_session_protocol.h>
#include <game\players.h>

enum e_network_session_type
{
	_network_session_type_none,
	_network_session_type_squad,
	//_network_session_type_group, // No longer exists in ms29

	k_network_session_type_count
};

enum e_network_session_boot_reason
{
	_network_session_boot_reason_player_booted_player,
	_network_session_boot_reason_join_timeout,
	_network_session_boot_reason_failed_to_load_map,
	_network_session_boot_reason_write_stats_grief_time,
	_network_session_boot_reason_unknown4,
	_network_session_boot_reason_banned,
	_network_session_boot_reason_recreating_session,
	_network_session_boot_reason_join_aborted,
	_network_session_boot_reason_simulation_aborted,
	_network_session_boot_reason_unknown9,
	_network_session_boot_reason_unknown10,
	_network_session_boot_reason_unknown11,
	_network_session_boot_reason_unknown12,

	k_network_session_boot_reason_count
};

enum e_session_game_mode
{
	_session_game_mode_none = 0,
	//_session_game_mode_matchmaking, // removed in later builds
	_session_game_mode_pregame,
	_session_game_mode_ingame,
	//_session_game_mode_postgame, // removed in later builds

	k_session_game_mode_count
};

enum e_network_session_disconnection_policy
{
	_network_session_disconnection_waiting_for_establishment = 0,
	_network_session_disconnection_allowed,
	_network_session_disconnection_reestablish_as_host,

	k_network_session_disconnection_policy_count
};

#pragma pack(push, 4)
struct s_network_session_local_player
{
	s_player_identifier player_identifier;
	s_transport_secure_address secure_address;
	long peer_output_user_index;
};
static_assert(sizeof(s_network_session_local_player) == 0x1C);

struct s_local_player_add_queue_entry
{
	s_player_identifier identifier;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> refuse_reason;
	dword add_time;
};
static_assert(sizeof(s_local_player_add_queue_entry) == 0x10);

class c_network_message_gateway;
class c_network_session_manager;
class c_network_session : c_network_channel_owner
{
public:
	bool handle_join_request(s_transport_address const* address, s_network_message_join_request const* message);
	bool acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason);
	bool handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message);
	bool handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message);
	bool handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message);
	bool handle_host_decline (c_network_channel* channel, s_network_message_host_decline const* message);
	bool handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message);
	bool channel_is_authoritative(c_network_channel* channel);
	bool handle_membership_update(s_network_message_membership_update const* message);
	bool handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	bool handle_parameters_update(s_network_message_parameters_update const* message);
	void join_accept(s_network_session_join_request const* join_request, s_transport_address const* address);
	e_network_join_refuse_reason can_accept_join_request(s_network_session_join_request const* join_request);
	e_network_join_refuse_reason can_accept_any_join_request();
	const char* get_type_string(e_network_session_type session_type);
	const char* get_state_string();
	const char* get_peer_description(long peer_index);
	void abort_pending_join(qword join_nonce);
	bool is_host();
	bool join_allowed_by_privacy();
	e_network_join_refuse_reason can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool unknown);
	bool session_is_full(long joining_peer_count, long joining_player_count);
	void disconnect();
	void disband_peer(long peer_index);
	void boot_peer(long peer_index, e_network_session_boot_reason boot_reason);
	const char* get_id_string();
	bool is_peer_joining_this_session();
	void idle();
	e_network_session_state current_local_state();
	bool disconnected();
	bool established();
	bool membership_is_locked();
	c_network_session_membership* get_session_membership();
	void idle_peer_creating();
	void idle_peer_joining();
	void idle_peer_join_abort();
	void idle_peer_leaving();
	void process_pending_joins();
	e_network_observer_owner observer_owner();
	long get_maximum_player_count();
	void handle_disconnection();
	void check_to_send_time_synchronization();
	void idle_observer_state();
	void check_to_send_membership_update();
	long managed_session_index();
	bool join_abort(s_transport_address const* incoming_address, qword join_nonce);
	c_network_session_parameters* get_session_parameters();
	bool handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message);
	bool peer_request_properties_update(s_transport_secure_address const* secure_address, s_network_session_peer_properties const* peer_properties);
	c_network_session_membership* get_session_membership_unsafe();
	bool join_nonce_is_from_clone_join_or_is_hosts(qword join_nonce);
	void finalize_single_player_add(e_network_join_refuse_reason refuse_reason);
	e_network_session_type session_type();
	bool host_join_nonce_valid();
	void add_pending_join_to_session(qword join_nonce);
	long session_index();
	bool handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	bool leaving_session();
	void time_set(ulong time);
	bool handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	bool handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	bool compare_session_id(s_transport_secure_identifier const* secure_id);
	bool get_session_id(s_transport_secure_identifier* secure_id);
	void peer_request_player_add(const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings);
	long get_session_membership_update_number();
	c_network_session_membership* get_session_membership_for_update();
	long find_peer_by_machine_identifier(s_machine_identifier const* machine_identifier);
	bool host_boot_machine(long peer_index, e_network_session_boot_reason reason);
	void initiate_leave_protocol(bool leave_immediately);
	bool handle_leave_request(s_transport_address const* outgoing_address);
	bool handle_leave_internal(long peer_index);

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index; // TODO: different enum max of 3 here
	c_enum<e_network_session_type, long, _network_session_type_none, k_network_session_type_count> m_session_type;
	c_enum<e_network_session_class, long, _network_session_class_offline, k_network_session_class_count> m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	long : 32;
	c_enum<e_network_session_state, long, _network_session_state_none, k_network_session_state_count> m_local_state;
	long : 32;
	union
	{
		s_network_session_state_peer_creating peer_creating;
		s_network_session_state_peer_joining peer_joining;
		s_network_session_state_peer_join_abort peer_join_abort;
		s_network_session_state_peer_established peer_established;
		s_network_session_state_peer_leaving peer_leaving;
	};
	ulong m_connection_identifier;
	ulong m_time_synchronization_end_time;
	ulong m_time_synchronization_start_time;
	bool m_time_exists;
	ulong m_time;
	long unknown; // recevied packet pointer?
	long m_managed_session_index;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> m_join_refuse_reason;
	qword m_host_join_nonce;
	long : 32;
	c_enum<e_network_session_disconnection_policy, long, _network_session_disconnection_waiting_for_establishment, k_network_session_disconnection_policy_count> m_disconnection_policy;
	s_network_session_local_player m_player_we_are_adding;
	long : 32;
	s_local_player_add_queue_entry m_local_user_player_add;

private:

};
static_assert(sizeof(c_network_session) == 0x1AC098);
#pragma pack(pop)