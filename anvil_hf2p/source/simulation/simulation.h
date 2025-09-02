#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/simulation/simulation.hpp

#include <cseries\cseries.h>
#include <game\player_control.h>
#include <game\players.h>
#include <replication\replication_encoding.h>
#include <units\units.h>
#include <simulation\simulation_queue.h>

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

enum e_predictability // TODO: where does this belong?
{
	_predictability0,
	_predictability1,
	_predictability2,
	_predictability3,
	_predictability4,
	_predictability5,

	k_predictability_count
};

enum e_simulation_playback_event
{
	_simulation_playback_event_revert = 0,

	k_simulation_playback_event_count,
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

struct s_simulation_queued_update
{
	e_update_queue_node node_type;

	byte __data0[0x4];

	struct
	{
		simulation_update update;
		s_simulation_update_metadata metadata;
	} update;

	struct
	{
		e_simulation_playback_event event_type;
		long event_data;
		long event_update_number;
	} playback_event;

	s_simulation_queued_update* next_node;
};
static_assert(sizeof(s_simulation_queued_update) == 0x1680);

class c_simulation_world;
class c_simulation_watcher;
class c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool simulation_fatal_error;
	bool simulation_deferred;
	bool simulation_aborted;
	dword simulation_aborted_timestamp;
	e_simulation_abort_reason abort_reason;
	bool simulation_in_initial_state;
	bool simulation_reset_in_progress;
	bool simulation_in_online_networked_session;
	bool gamestate_load_in_progress;
	bool recording_film;
	bool must_close_saved_film;
	bool performed_main_save_and_exit_campaign_immediately_this_map;
	c_simulation_world* world;
	c_simulation_watcher* watcher;
	c_simulation_type_collection* type_collection;
	s_data_array* view_data_array;
	c_static_string<256> status;
	// confirm these two are in ms29
	bool simulation_paused;
	bool handled_determinism_failure;
};
static_assert(sizeof(s_simulation_globals) == 0x128);
static_assert(0x0E == OFFSETOF(s_simulation_globals, simulation_in_online_networked_session));

extern s_simulation_globals& simulation_globals;

//extern bool __cdecl simulation_starting_up(); // 0x004420E0 in ms23
bool simulation_reset_in_progress();
bool simulation_get_node_space_encoding_type(datum_index object_index);
enum e_network_session_boot_reason;
bool simulation_boot_machine(s_machine_identifier const* machine_identifier, e_network_session_boot_reason reason);