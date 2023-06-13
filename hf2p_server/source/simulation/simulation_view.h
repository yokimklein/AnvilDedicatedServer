#pragma once
#include <cseries\cseries.h>
#include <game\players.h>
#include <networking\delivery\network_channel.h>
#include <networking\replication\replication_event_manager_view.h>
#include <networking\replication\replication_entity_manager_view.h>
#include <networking\replication\replication_control_view.h>

enum e_simulation_view_establishment_mode : long
{
	_simulation_view_establishment_mode_none = -1,
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

struct c_simulation_distributed_view
{
	byte __data0[0x4];
	c_replication_scheduler m_scheduler;
	c_replication_entity_manager_view m_entity_manager_view;
	c_replication_event_manager_view m_event_manager_view;
	c_replication_control_view m_control_view;
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

	long unknown1;
	c_enum<e_simulation_view_type, long, k_simulation_view_type_count> m_view_type;
	long m_view_datum_index;
	c_simulation_distributed_view* m_distributed_view;
	c_simulation_world* m_world;
	long m_world_view_index;
	s_machine_identifier m_remote_machine_identifier;
	long m_remote_machine_index;
	c_network_observer* m_observer;
	long m_observer_channel_index;
	long m_view_death_reason;
	long m_view_establishment_mode;
	long m_view_establishment_identifier;
	long m_valid_view_establishment_identifier;
	long unknown2;
	c_network_channel* m_channel;
	long m_channel_connection_identifier;
	c_network_channel_simulation_interface m_simulation_interface;
	// synchronous catchup data
	byte __data84[0x3C];
};
static_assert(sizeof(c_simulation_view) == 0xC0);