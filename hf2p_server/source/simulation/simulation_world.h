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

#pragma pack(push, 1)
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
	bool m_local_machine_identifier_valid;
	s_machine_identifier m_local_machine_identifier;
	byte __align1D[0x3];
	long m_local_machine_index;
	e_simulation_world_state m_world_state;
	long m_last_time_disconnected;
	byte __data2C[0x4];
	bool m_time_running;
	bool m_time_immediate_update;
	byte __data32[0x2];
	long __unknown34;
	long m_current_update_number;
	byte __data3C[0x4];
	bool __unknown40;
	bool __unknown41;
	bool m_attached_to_map;
	bool __unknown43;
	long m_join_attempt_count;
	long m_last_time_active;
	long m_establishment_identifier;
	long __unknown50;
	byte __data54[0x10];
	c_simulation_view* m_views[k_network_maximum_players_per_session];
	byte __dataA4[0x4];
	c_simulation_player m_players[k_network_maximum_players_per_session];
	c_simulation_actor m_actors[k_network_maximum_players_per_session];
	byte __data14E8[0x4];
	long m_next_update_dequeue;
	long m_update_queue_latest_entry_received_type;
	long m_update_queue_latest_entry_received_update_number;
	long m_update_queue_length;
	long m_update_queue_number_of_updates;
	long* m_update_queue_head;
	long* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);
#pragma pack(pop)

c_simulation_world* simulation_get_world();