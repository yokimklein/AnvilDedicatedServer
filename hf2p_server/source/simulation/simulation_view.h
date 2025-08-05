#pragma once
#include <cseries\cseries.h>
#include <game\players.h>
#include <networking\delivery\network_channel.h>
#include <networking\replication\replication_event_manager_view.h>
#include <networking\replication\replication_entity_manager_view.h>
#include <networking\replication\replication_control_view.h>
#include <simulation\simulation_view_telemetry.h>
#include <networking\messages\network_messages_simulation_synchronous.h>

enum e_simulation_view_establishment_mode
{
	_simulation_view_establishment_mode_none = NONE,
	_simulation_view_establishment_mode_detached,
	_simulation_view_establishment_mode_connect,
	_simulation_view_establishment_mode_established,
	_simulation_view_establishment_mode_ready,
	_simulation_view_establishment_mode_joining,
	_simulation_view_establishment_mode_active,

	k_simulation_view_establishment_mode_count
};

enum e_simulation_view_reason
{
	_simulation_view_reason_none = 0,
	_simulation_view_reason_disconnected,
	_simulation_view_reason_out_of_sync,
	_simulation_view_reason_failed_to_join,
	_simulation_view_reason_blocking,
	_simulation_view_reason_catchup_fail,
	_simulation_view_reason_ended,
	_simulation_view_reason_mode_error,
	_simulation_view_reason_player_error,
	_simulation_view_reason_replication_entity,
	_simulation_view_reason_replication_event,
	_simulation_view_reason_replication_game_results,
	_simulation_view_reason_no_longer_authority,
	_simulation_view_reason_signature_invalid,

	k_simulation_view_reason_count
};

enum e_simulation_view_type
{
	_simulation_view_type_none = 0,
	_simulation_view_type_synchronous_to_remote_authority,
	_simulation_view_type_synchronous_to_remote_client,
	_simulation_view_type_distributed_to_remote_authority,
	_simulation_view_type_distributed_to_remote_client,

	k_simulation_view_type_count
};

enum e_simulation_view_synchronous_catchup_stage
{
	_synchronous_catchup_not_in_progress = 0,
	_synchronous_catchup_intiate,
	_synchronous_catchup_preparing_for_gamestate,
	_synchronous_catchup_in_progress,
	_synchronous_catchup_finish,
	_synchronous_catchup_client_decompressing_gamestate,

	k_synchronous_catchup_stage_count
};

class c_simulation_distributed_view : public s_datum_header
{
	c_replication_scheduler m_scheduler;
	c_replication_entity_manager_view m_entity_manager_view;
	c_replication_event_manager_view m_event_manager_view;
	c_replication_control_view m_control_view;
	c_simulation_view_telemetry_provider m_telemetry_provider;
	c_game_results_replicator m_game_results_replicator;
};
static_assert(sizeof(c_simulation_distributed_view) == 0x17E48);

struct s_simulation_view_statistics
{
	s_replication_entity_manager_view_statistics entity_statistics;
	s_replication_event_manager_view_statistics event_statistics;
};
static_assert(sizeof(s_simulation_view_statistics) == 0x24);

class c_simulation_world;
class c_network_observer;
class c_simulation_view
{
public:
	void set_view_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier);
	bool established() const;
	bool exists() const;
	e_simulation_view_establishment_mode get_view_establishment_mode() const;
	bool ready_to_establish() const;
	const char* get_view_description() const;
	bool is_dead(e_simulation_view_reason* out_death_reason) const;

protected:
	long unknown1;
	e_simulation_view_type m_view_type;
	long m_view_datum_index;
	c_simulation_distributed_view* m_distributed_view;
	c_simulation_world* m_world;
	long m_world_view_index;
	s_machine_identifier m_remote_machine_identifier;
	long m_remote_machine_index;
	c_network_observer* m_observer;
	long m_observer_channel_index;
	e_simulation_view_reason m_view_death_reason;
	e_simulation_view_establishment_mode m_view_establishment_mode;
	long m_view_establishment_identifier;
	long m_remote_establishment_mode;
	dword m_remote_establishment_identifier;
	c_network_channel* m_channel;
	long m_channel_connection_identifier;
	c_network_channel_simulation_interface m_channel_interface;
	bool m_simulation_active;
	ulong m_simulation_player_acknowledged_mask;
	long m_synchronous_received_action_number;
	long m_synchronous_acknowledged_update_number;
	long m_synchronous_catchup_attempt_count;
	long m_synchronous_catchup_start_update;
	long m_synchronous_catchup_time_of_last_activity;
	long m_synchronous_catchup_finish_time;
	long m_synchronous_catchup_buffer_offset;
	e_simulation_view_synchronous_catchup_stage m_synchronous_catchup_stage;
	s_network_message_synchronous_gamestate m_checksum_message;
	long m_synchronous_next_action_number;
};
static_assert(sizeof(c_simulation_view) == 0xC0);