#pragma once
#include <cseries\cseries.h>
#include <networking\session\network_observer.h>
#include <networking\session\network_session_membership.h>
#include <networking\session\network_session_parameters.h>

enum e_network_session_type
{
	_network_session_type_none = 0,
	_network_session_type_squad,
	//_network_session_type_group, // No longer exists in ms29

	k_network_session_type_count
};

enum e_network_session_boot_reason
{
	_network_session_boot_user_request_in_game = 0,
	_network_session_boot_join_timeout,
	_network_session_boot_failed_to_load_map,
	_network_session_boot_blocking_stats_write,
	_network_session_boot_from_ui,
	_network_session_boot_quick_disconnect,
	_network_session_boot_for_join,
	_network_session_boot_for_join_abort,
	_network_session_boot_simulation_aborted,
	_network_session_boot_unknown9,
	_network_session_boot_unknown10,
	_network_session_boot_unknown11,
	_network_session_boot_unknown12,

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

struct s_network_message_boot_machine;
struct s_network_message_delegate_leadership;
struct s_network_message_host_decline;
struct s_network_message_join_request;
struct s_network_message_membership_update;
struct s_network_message_parameters_request;
struct s_network_message_parameters_update;
struct s_network_message_peer_connect;
struct s_network_message_peer_establish;
struct s_network_message_peer_properties;
struct s_network_message_player_add;
struct s_network_message_player_properties;
struct s_network_message_player_refuse;
struct s_network_message_player_remove;
struct s_network_message_session_boot;
struct s_network_message_session_disband;
struct s_network_message_time_synchronize;

class c_network_message_gateway;
class c_network_observer;
class c_network_session_manager;
class c_network_channel;
class c_network_session : public c_network_channel_owner
{
public:
	bool handle_join_request(transport_address const* address, s_network_message_join_request const* message);
	bool acknowledge_join_request(transport_address const* address, e_network_join_refuse_reason reason);
	bool handle_peer_connect(transport_address const* outgoing_address, s_network_message_peer_connect const* message);
	bool handle_session_disband(transport_address const* outgoing_address, s_network_message_session_disband const* message);
	bool handle_session_boot(transport_address const* outgoing_address, s_network_message_session_boot const* message);
	bool handle_host_decline (c_network_channel* channel, s_network_message_host_decline const* message);
	bool handle_time_synchronize(transport_address const* outgoing_address, s_network_message_time_synchronize const* message);
	bool channel_is_authoritative(c_network_channel* channel);
	bool handle_membership_update(s_network_message_membership_update const* message);
	bool handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	bool handle_parameters_update(s_network_message_parameters_update const* message);
	void join_accept(s_network_session_join_request const* join_request, transport_address const* address);
	e_network_join_refuse_reason can_accept_join_request(s_network_session_join_request const* join_request);
	e_network_join_refuse_reason can_accept_any_join_request() const;
	static char const* get_type_string(e_network_session_type session_type);
	char const* get_state_string() const;
	char const* get_peer_description(long peer_index) const;
	void abort_pending_join(qword join_nonce);
	bool is_host() const;
	bool join_allowed_by_privacy() const;
	e_network_join_refuse_reason can_accept_player_join_request(s_player_identifier const* player_identifier, s_transport_secure_address const* joining_peer_address, long peer_index, bool player_add_not_join) const;
	bool session_is_full(long joining_peer_count, long joining_player_count) const;
	void disconnect();
	void disband_peer(long peer_index);
	void boot_peer(long peer_index, e_network_session_boot_reason boot_reason);
	char const* get_id_string() const;
	bool is_peer_joining_this_session() const;
	void idle();
	e_network_session_state current_local_state() const;
	bool disconnected() const;
	bool established() const;
	bool membership_is_locked() const;
	c_network_session_membership const* get_session_membership() const;
	void idle_peer_creating();
	void idle_peer_joining();
	void idle_peer_join_abort();
	void idle_peer_leaving();
	void process_pending_joins();
	e_network_observer_owner observer_owner() const;
	long get_maximum_player_count() const;
	void handle_disconnection();
	void check_to_send_time_synchronization();
	void idle_observer_state();
	void check_to_send_membership_update();
	long managed_session_index() const;
	bool join_abort(transport_address const* incoming_address, qword join_nonce);
	c_network_session_parameters* get_session_parameters();
	const c_network_session_parameters* get_session_parameters() const;
	bool handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message);
	bool peer_request_properties_update(s_transport_secure_address const* secure_address, s_network_session_peer_properties const* peer_properties);
	c_network_session_membership const* get_session_membership_unsafe() const;
	bool join_nonce_is_from_clone_join_or_is_hosts(qword join_nonce) const;
	void finalize_single_player_add(e_network_join_refuse_reason refuse_reason);
	e_network_session_type session_type() const;
	void add_pending_join_to_session(qword join_nonce);
	long session_index() const;
	bool handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	bool leaving_session() const;
	void time_set(ulong time);
	bool handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	bool handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	bool compare_session_id(s_transport_secure_identifier const* secure_id) const;
	bool get_session_id(s_transport_secure_identifier* secure_id) const;
	void peer_request_player_add(const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings);
	long get_session_membership_update_number() const;
	c_network_session_membership* get_session_membership_for_update();
	long find_peer_by_machine_identifier(s_machine_identifier const* machine_identifier) const;
	bool host_boot_machine(long peer_index, e_network_session_boot_reason reason);
	void initiate_leave_protocol(bool leave_immediately);
	bool handle_leave_request(transport_address const* outgoing_address);
	bool handle_leave_internal(long peer_index);
	bool peer_joining() const;

	// non source original getter
	const c_network_message_gateway* message_gateway() const { return m_message_gateway; };

private:
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index; // TODO: different enum max of 3 here
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	long : 32;
	e_network_session_state m_local_state;
	long : 32;
	s_local_state_data m_local_state_data;
	ulong m_connection_identifier;
	ulong m_time_synchronization_end_time;
	ulong m_time_synchronization_start_time;
	bool m_time_exists;
	ulong m_time_offset;
	long m_creation_time;
	long m_managed_session_index;
	e_network_join_refuse_reason m_join_refuse_reason;
	qword m_host_join_nonce;
	long : 32;

	//bool m_only_use_squad_session_for_election;
	//bool m_enable_speculative_host_migration;
	//dword m_time_of_last_speculative_host_migration;
	//dword m_time_of_last_speculative_host_migration_check;
	//dword m_speculative_host_migration_disallow_mask;

	e_network_session_disconnection_policy m_disconnection_policy;
	
	struct s_network_session_local_player
	{
		s_player_identifier player_identifier;
		s_transport_secure_address secure_address;
		long peer_output_user_index;
	} m_player_we_are_adding;
	static_assert(sizeof(c_network_session::s_network_session_local_player) == 0x1C);
	long : 32;
	struct s_local_player_add_queue_entry
	{
		s_player_identifier identifier;
		e_network_join_refuse_reason refuse_reason;
		dword request_time;
	} m_local_user_player_add;
	static_assert(sizeof(c_network_session::s_local_player_add_queue_entry) == 0x10);
};
static_assert(sizeof(c_network_session) == 0x1AC098);
