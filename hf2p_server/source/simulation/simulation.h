#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/simulation/simulation.hpp

#include "..\cseries\cseries.h"
#include "..\game\player_control.h"
#include "..\game\players.h"
#include "..\replication\replication_encoding.h"
#include "..\units\units.h"
#include "simulation_queue.h"
#include "..\dllmain.h"

constexpr long k_network_maximum_machines_per_session = 17; // 16 in h3 debug
constexpr long k_network_maximum_players_per_session = 16;

enum e_network_game_simulation_protocol
{
	_network_game_simulation_local = 0,

	_network_game_simulation_synchronous,
	_network_game_simulation_distributed,

	k_network_game_simulation_count
};

enum e_game_simulation_type
{
    _game_simulation_none = 0,
    _game_simulation_local,

    _game_simulation_synchronous_client, // predicted
    _game_simulation_synchronous_server, // authoritative

    _game_simulation_distributed_client, // predicted
    _game_simulation_distributed_server, // authoritative

    k_game_simulation_count
};

enum e_simulation_abort_reason
{
	_simulation_abort_reason_exiting_in_game = 0,
	_simulation_abort_reason_exiting_in_match,
	_simulation_abort_reason_failed_to_find_next_map,
	_simulation_abort_reason_playing_film,
	_simulation_abort_reason_reset_failed,
	_simulation_abort_reason_fatal_error,
	_simulation_abort_reason_lost_connection,
	_simulation_abort_reason_out_of_sync,
	_simulation_abort_reason_film_ended,
	_simulation_abort_reason_film_playback_error,

	k_simulation_abort_reason_count
};

enum e_update_queue_node
{
	_update_queue_node_update,
	_update_queue_node_playback_event,

	k_update_queue_node_count,
	k_update_queue_node_invalid = -1
};

struct simulation_machine_update
{
	dword machine_valid_mask;
	c_static_array<s_machine_identifier, k_network_maximum_machines_per_session> machine_identifiers;
};
static_assert(sizeof(simulation_machine_update) == 0x114);

struct s_simulation_camera_update
{
	long camera_type;
	long player_absolute_index;
	real_point3d position;
	real_vector3d forward;
	bool zoomed;
};
static_assert(sizeof(s_simulation_camera_update) == 0x24);

struct simulation_update
{
	long update_number;
	word_flags flags;
	dword_flags player_flags;

	dword_flags action_test_flags;
	c_static_array<player_action, k_network_maximum_players_per_session> player_actions;

	dword valid_actor_mask;
	c_static_array<long, k_network_maximum_players_per_session> actor_unit_indices;
	c_static_array<unit_control_data, k_network_maximum_players_per_session> actor_control;

	bool machine_update_valid;
	simulation_machine_update machine_update;

	dword valid_player_prediction_mask;
	c_static_array<s_player_prediction, k_network_maximum_players_per_session> player_prediction;

	long verify_game_time;
	long verify_random;

	//s_determinism_verification determinism_verification;

	dword valid_camera_mask;
	c_static_array<s_simulation_camera_update, 1> camera_update;

	c_simulation_queue bookkeeping_simulation_queue;
	c_simulation_queue game_simulation_queue;
};
static_assert(sizeof(simulation_update) == 0x1658);

struct s_simulation_update_metadata
{
	dword_flags flags;
	long saved_film_position;
	long saved_film_tick;
};
static_assert(sizeof(s_simulation_update_metadata) == 0xC);

struct s_simulation_update_playback_event
{
	byte __data0[0xC];
	long event_update_number;
};
static_assert(sizeof(s_simulation_update_playback_event) == 0x10);

// unknown struct name using `s_simulation_update_node` for now
struct s_simulation_update_node
{
	c_enum<e_update_queue_node, long, k_update_queue_node_count> node_type;

	byte __data0[0x4];

	simulation_update update;
	s_simulation_update_metadata metadata;
	s_simulation_update_playback_event playback_event;
	s_simulation_update_node* next;
};
static_assert(sizeof(s_simulation_update_node) == 0x1680);

class c_simulation_world;
class c_simulation_watcher;
class c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool fatal_error;
	bool saved_film_revert;
	bool aborted;

	dword network_time_since_abort;

	c_enum<e_simulation_abort_reason, long, k_simulation_abort_reason_count> abort_reason;

	bool reset;
	bool reset_in_progress;
	byte __unknownE;

	bool prepare_to_load_saved_game;

	bool recording_film;
	byte __unknown11;
	byte __unknown12;
	byte __unknown13;

	c_simulation_world* world;
	c_simulation_watcher* watcher;
	c_simulation_type_collection* type_collection;

	dword __unknown20;

	c_static_string<256> status;
	/* - removed in MS29
	bool paused;
	byte __unknown125;
	byte __unknown126;
	byte __unknown127;

	// used in `simulation_update_write_to_buffer`
	simulation_update update;
	dword_flags update_flags;
	*/
};
static_assert(sizeof(s_simulation_globals) == 0x124); // 0x1784 in ms23

static s_simulation_globals* simulation_globals = (s_simulation_globals*)(module_base + 0x4EBEBA8);

//extern bool __cdecl simulation_starting_up(); // 0x004420E0 in ms23
bool simulation_reset_in_progress();