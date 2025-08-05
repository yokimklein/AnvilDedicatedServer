#pragma once
#include <cseries\cseries.h>
#include <simulation\simulation_view.h>
#include <simulation\simulation.h>
#include <simulation\simulation_actors.h>
#include <simulation\simulation_players.h>
#include <simulation\simulation_watcher.h>
#include <simulation\simulation_queue.h>
#include <simulation\simulation_entity_database.h>
#include <networking\replication\replication_event_manager.h>
#include <networking\replication\replication_entity_manager.h>

enum e_simulation_world_type
{
	_simulation_world_type_none,
	_simulation_world_type_local,
	_simulation_world_type_local_playback,
	_simulation_world_type_synchronous_game_authority,
	_simulation_world_type_synchronous_game_client,
	_simulation_world_type_synchronous_playback_authority,
	_simulation_world_type_synchronous_playback_client,
	_simulation_world_type_distributed_game_authority,
	_simulation_world_type_distributed_game_client,

	k_simulation_world_type_count
};

enum e_simulation_world_state
{
	_simulation_world_state_none = 0,
	_simulation_world_state_dead,
	_simulation_world_state_disconnected,
	_simulation_world_state_joining,
	_simulation_world_state_active,
	_simulation_world_state_handoff,

	k_simulation_world_state_count
};

enum e_update_queue_state
{
	_update_queue_state_normal = 0,
	_update_queue_state_peaking,
	_update_queue_state_throttling,
	_update_queue_state_cooling,

	k_update_queue_state_count,
};

class c_simulation_distributed_world
{
public:
	c_replication_entity_manager m_entity_manager;
	c_replication_event_manager m_event_manager;
	c_simulation_entity_database m_entity_database;
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

class c_simulation_world
{
public:
	void update_establishing_view(c_simulation_view* simulation_view);
	bool exists();
	bool is_distributed();
	bool is_authority();
	bool is_active();
	c_simulation_entity_database* get_entity_database();
	c_simulation_event_handler* get_event_handler();
	long get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier);

	struct s_world_state_data_disconnected
	{
		ulong disconnected_timestamp;
	};
	static_assert(sizeof(s_world_state_data_disconnected) == 0x4);

	struct s_world_state_data_joining
	{
		ulong join_start_timestamp;
		ulong join_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_joining) == 0x8);

	struct s_world_state_data_active
	{
		ulong active_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_active) == 0x4);

	struct s_world_state_data
	{
		union
		{
			s_world_state_data_disconnected disconnected;
			s_world_state_data_joining joining;
			s_world_state_data_active active;
		};
	};
	static_assert(sizeof(s_world_state_data) == 0x8);

	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	e_simulation_world_type m_world_type;
	bool m_local_machine_identifier_valid;
#pragma pack(push, 1)
	s_machine_identifier m_local_machine_identifier;
#pragma pack(pop)
	long m_local_machine_index;
	e_simulation_world_state m_world_state;
	s_world_state_data m_world_state_data;
	bool m_time_running;
	bool m_time_immediate_update;
	long m_next_update_number;
	long m_acknowledged_update_number;
	bool m_out_of_sync;
	bool m_out_of_sync_determinism_failure;
	bool m_world_clean;
	bool m_gamestate_flushed;
	bool m_gamestate_flushed_outside_game_tick;
	bool m_gamestate_modified_initial_state;
	bool m_attached_to_map;
	bool m_gamestate_flush_client_skip;
	long m_unsuccessful_join_attempts;
	ulong m_last_active_timestamp;
	long m_next_view_establishment_identifier;
	long m_joining_total_wait_msec;
	long m_maximum_queued_updates;
	e_update_queue_state m_update_queue_state;
	ulong m_update_queue_state_time;
	long m_view_count;
	c_simulation_view* m_views[k_network_maximum_players_per_session];
	c_simulation_player m_players[k_network_maximum_players_per_session];
	c_simulation_actor m_actors[k_network_maximum_players_per_session];
	ulong m_synchronous_catchup_initiation_failure_timestamp;
	long m_update_queue_next_update_number_to_dequeue;
	e_update_queue_node m_update_queue_latest_entry_received_type;
	long m_update_queue_latest_entry_received_update_number;
	long m_update_queue_length;
	long m_update_queue_number_of_updates;
	s_simulation_queued_update* m_update_queue_head;
	s_simulation_queued_update* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);

c_simulation_world* simulation_get_world();