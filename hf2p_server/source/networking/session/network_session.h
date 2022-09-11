#pragma once
#include "..\..\cseries\cseries.h"
#include "network_session_membership.h"
#include "network_session_parameters.h"
#include "network_observer.h"
#include "..\messages\network_messages_session_membership.h"
#include "..\messages\network_messages_session_protocol.h"

enum e_network_session_type
{
	_network_session_type_none,
	_network_session_type_group,
	_network_session_type_squad,

	k_network_session_type_count
};

static const char* k_session_type_strings[k_network_session_type_count] = {
	"none",
	"group",
	"squad"
};

enum e_network_session_state : long
{
	_network_session_state_none,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,
	// removed in ms29
	//_network_session_state_host_handoff,
	//_network_session_state_host_reestablish,
	//_network_session_state_election,

	k_network_session_state_count
};

enum e_network_session_boot_reason : long // from ms23/ED
{
	_network_session_boot_reason_player_booted_player,
	_network_session_boot_reason_unknown1,
	_network_session_boot_reason_failed_to_load_map,
	_network_session_boot_reason_write_stats_grief_time,
	_network_session_boot_reason_unknown4,
	_network_session_boot_reason_unknown5,
	_network_session_boot_reason_recreating_session,
	_network_session_boot_reason_join_aborted,
	_network_session_boot_reason_simulation_aborted,
	_network_session_boot_reason_unknown9,
	_network_session_boot_reason_unknown10,
	_network_session_boot_reason_unknown11,
	_network_session_boot_reason_unknown12,

	k_network_session_boot_reason_count
};

static const char* k_session_state_strings[k_network_session_state_count] = {
	"none",
	"peer-creating",
	"peer-joining",
	"peer-join-abort",
	"peer-established",
	"peer-leaving",
	"host-established",
	"host-disband",
	// "host-handoff",
	// "host-reestablish",
	// "election"
};

#pragma pack(push, 4)
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
	e_network_join_refuse_reason get_closure_reason();
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
	e_network_observer_owner session_index();
	bool handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	bool leaving_session();
	void time_set(ulong time);
	bool handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	bool handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	bool compare_session_id(s_transport_secure_identifier const* secure_id);
	bool get_session_id(s_transport_secure_identifier* secure_id);
	void peer_request_player_add(const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings);
	long get_session_membership_update_number();

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	e_network_observer_owner m_session_index;
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	long : 32;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	long : 32;
	e_network_session_state m_local_state;
	long : 32;
	char m_local_state_data[0x288]; // host s_transport_address @ byte offset 20
	ulong m_connection_identifier;
	ulong m_time_synchronization_end_time;
	ulong m_time_synchronization_start_time;
	bool m_time_exists;
	ulong m_time;
	long unknown; // recevied packet pointer?
	long m_managed_session_index;
	e_network_join_refuse_reason m_join_refuse_reason;
	qword m_host_join_nonce;
	long : 32;
	ulong m_disconnection_policy;
	s_player_identifier m_player_add_single_player_identifier;
	s_transport_secure_address m_player_add_secure_address;
	long m_player_add_peer_index;
	long : 32;
	s_player_identifier m_player_add_player_identifier;
	e_network_join_refuse_reason m_player_add_join_refuse_reason;
	ulong m_player_add_time;

private:

};
static_assert(sizeof(c_network_session) == 0x1AC098);
#pragma pack(pop)