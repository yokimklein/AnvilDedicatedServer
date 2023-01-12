#pragma once
#include "..\cseries\cseries.h"
#include "simulation_view.h"
#include "simulation.h"
#include "simulation_actors.h"
#include "simulation_players.h"
#include "simulation_watcher.h"
#include "simulation_queue.h"
#include "simulation_entity_database.h"
#include "..\networking\replication\replication_event_manager.h"
#include "..\networking\replication\replication_entity_manager.h"

enum e_simulation_world_type : long
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

	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	e_simulation_world_type m_world_type;

	union
	{
#pragma pack(push, 1)
		struct
		{
			bool m_local_machine_identifier_valid;
			s_machine_identifier m_local_machine_identifier;
			byte __align1D[0x3];
		} s;
#pragma pack(pop)

		byte data[sizeof(s)];
	};

	long m_local_machine_index;
	c_enum<e_simulation_world_state, long, k_simulation_world_state_count> m_world_state;
	long m_last_time_disconnected;
	byte __unknown2C;
	bool m_time_running;
	bool m_time_immediate_update;
	byte __unknown32; // pad?
	byte __unknown33; // pad?
	long m_next_update_number;
	long m_current_update_number;
	bool m_synchronous_out_of_sync;
	bool m_synchronous_determinism_failure;
	byte __unknown3E; // pad?
	byte __unknown3F; // pad?
	// c_simulation_world::notify_gamestate_flush
	bool m_notify_gamestate_flushed;
	// c_simulation_world::notify_gamestate_flush_outside_game_tick
	bool m_notify_gamestate_flushed_outside_game_tick;
	// c_simulation_world::attach_to_map
	bool m_attached_to_map;
	// c_simulation_world::skip_next_gamestate_flush
	bool m_skipped_next_gamestate_flush;
	long m_join_attempt_count;
	long m_last_time_active;
	long m_establishment_identifier;
	long __unknown50;
	long __unknown54;
	long __unknown58;
	long __unknown5C;
	long m_view_count;
	c_static_array<c_simulation_view*, k_network_maximum_players_per_session> m_views;
	long __unknownA4;
	c_static_array<c_simulation_player, k_network_maximum_players_per_session> m_players;
	c_static_array<c_simulation_actor, k_network_maximum_players_per_session> m_actors;
	long __unknown14E8;
	long m_next_update_dequeue;
	long m_update_queue_latest_entry_received_type;
	long m_update_queue_latest_entry_received_update_number;
	long m_update_queue_length;
	long m_update_queue_number_of_updates;
	s_simulation_update_node* m_update_queue_head;
	s_simulation_update_node* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);

c_simulation_world* simulation_get_world();