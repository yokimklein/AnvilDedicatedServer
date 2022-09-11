#pragma once
#include "..\..\cseries\cseries.h"
#include "network_session_parameters_base.h"
#include "network_session_membership.h"
#include "..\..\game\game_options.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"
#include "..\..\interface\gui_pregame_setup_manager.h"
#include "..\..\interface\user_interface_networking.h"

// non-original names, may be incorrect
enum e_network_game_privacy
{
	_network_game_privacy_open_to_public,
	_network_game_privacy_open_to_friends,
	_network_game_privacy_invite_only,
	_network_game_privacy_closed,

	k_network_game_privacy_count
};

enum e_network_session_closed_status
{
	_network_session_closed_none,
	_network_session_closed_campaign_in_progress,
	_network_session_closed_film_in_progress,
	_network_session_closed_in_matchmaking,
	_network_session_closed_survival_in_progress
};

// non original names based on how they're used
enum e_session_game_start_status
{
	_start_status_none,
	_start_status_not_pre_game = 1,
	_start_status_not_leader = 2,
	_start_status_join_in_progress = 3,
	_start_status_loading = 4,
	_start_status_ready = 5,
	_start_status_countdown = 6,
	_start_status_countdown_unknown = 7,
	_start_status_error = 8,

	k_start_status_count
};

static const char* k_game_start_status_strings[k_start_status_count] = {
	"none",
	"not pre game",
	"not leader",
	"join in progress",
	"loading",
	"ready",
	"countdown",
	"countdown unknown",
	"error"
};

// non-original names based off strings in multiplayer_game_start_error_to_string
// this may be incorrect
enum e_session_game_start_error
{
	_start_error_none = 0,
	_start_error_no_map_selected = 1,
	_start_error_map_load_failure = 2,
	_start_error_map_load_precaching = 3,
	_start_error_invalid_film_selected = 4,
	_start_error_no_film_selected = 5,
	_start_error_too_many_teams = 6,
	_start_error_too_many_for_local_coop = 7,
	_start_error_too_many_for_net_coop = 8,
	_start_error_incompatible_for_net_coop = 9,
	_start_error_theater_too_many_players = 10,
	_start_error_theater_leader_must_be_host = 11,
	_start_error_theater_all_not_compatible = 12,
	_start_error_too_many_players_in_forge = 13,

	k_session_game_start_error_count
};

// update this whenever the enum updates
static const char* k_game_start_error_strings[k_session_game_start_error_count] = {
	"none",
	"no map selected",
	"map load failure",
	"map load precaching",
	"invalid film selected",
	"no film selected",
	"too many teams",
	"too many for local coop",
	"too many for net coop",
	"incompatible for net coop",
	"theater too many players",
	"theater leader must be host",
	"theater all not compatible",
	"too many players in forge"
};

struct s_network_session_privacy_mode
{
	e_network_game_privacy privacy_mode;
	e_network_session_closed_status closed_mode;
	long maximum_player_count;
	bool is_closed_by_user;
};
static_assert(sizeof(s_network_session_privacy_mode) == 0x10);

class c_generic_network_session_parameter_privacy_mode : public c_network_session_parameter_base
{
public:

	s_network_session_privacy_mode m_data;
	s_network_session_privacy_mode m_requested_data;
};

class c_network_session_parameter_ui_game_mode: public c_network_session_parameter_base
{
	e_gui_game_mode m_data;
	e_gui_game_mode m_requested_data;
};

class c_generic_network_session_parameter_dedicated_server_session_state: public c_network_session_parameter_base
{
	long m_data; // value between 0-10
	long m_requested_data;
};

struct s_network_session_parameter_map
{
	long campaign_id;
	long map_id;
	char map_name[128];
	byte campagin_arguments[120];
	ulong game_progression_bit_vector[64];
};

class c_network_session_parameter_map : public c_network_session_parameter_base
{
	s_network_session_parameter_map m_data;
	s_network_session_parameter_map m_requested_data;
};

struct s_network_session_parameter_initial_participants
{
	bool machine_options_exists;
	game_machine_options machine_options;
	bool player_options_exists;
	game_player_options player_options[k_network_maximum_players_per_session];
};

class c_network_session_parameter_initial_participants : public c_network_session_parameter_base
{
	s_network_session_parameter_initial_participants m_data;
	s_network_session_parameter_initial_participants m_requested_data;
	long m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	long m_next_request_chunk;
	long m_expected_update_chunk;
	long m_expected_change_chunk;
	char chunked_update_data[0x1F800];
	long chunked_update_data_size;
	long chunked_update_data_checksum;
	long chunked_update_data_chunk_count;
	char chunked_change_data[0x1F800];
	long chunked_change_data_size;
	long chunked_change_data_checksum;
	long chunked_change_data_chunk_count;
	long m_max_chunk_count;
	long : 32;
};

class c_generic_network_session_parameter_start_mode : public c_network_session_parameter_base
{
	e_desired_multiplayer_mode m_data;
	e_desired_multiplayer_mode m_requested_data;
};

struct s_network_session_parameter_game_start_status
{
	e_session_game_start_status game_start_status;
	e_session_game_start_error game_start_error;
	ushort player_error_mask;
	ushort map_load_progress;
};
static_assert(sizeof(s_network_session_parameter_game_start_status) == 0xC);

class c_network_session_parameter_game_start_status : public c_network_session_parameter_base
{
public:
	s_network_session_parameter_game_start_status m_data;
	s_network_session_parameter_game_start_status m_requested_data;
};

struct s_network_session_parameter_countdown_timer
{
	long : 32;
	long : 32;
};

class c_network_session_parameter_countdown_timer : public c_network_session_parameter_base
{
	s_network_session_parameter_countdown_timer m_data;
	s_network_session_parameter_countdown_timer m_requested_data;
};

struct s_network_session_parameter_voice_repeater
{
	long peer_index;
};
static_assert(sizeof(s_network_session_parameter_voice_repeater) == 0x4);

class c_network_session_parameter_voice_repeater : public c_network_session_parameter_base
{
	s_network_session_parameter_voice_repeater m_data;
	s_network_session_parameter_voice_repeater m_requested_data;
};

struct s_network_session_parameter_synchronous_out_of_sync
{
	short : 16;
};

class c_network_session_parameter_synchronous_out_of_sync : public c_network_session_parameter_base
{
	s_network_session_parameter_synchronous_out_of_sync m_data;
	s_network_session_parameter_synchronous_out_of_sync m_requested_data;
};

struct s_network_session_parameter_request_campaign_quit
{
	long campaign_quit_reason;
	long campaign_quitting_peer;
};
static_assert(sizeof(s_network_session_parameter_request_campaign_quit) == 0x8);

class c_network_session_parameter_request_campaign_quit : public c_network_session_parameter_base
{
	s_network_session_parameter_request_campaign_quit m_data;
	s_network_session_parameter_request_campaign_quit m_requested_data;
};

class c_network_session_parameter_game_simulation_protocol : public c_network_session_parameter_base
{
public:
	e_network_game_simulation_protocol m_data;
	e_network_game_simulation_protocol m_requested_data;
};

const char* multiplayer_game_start_error_to_string(e_session_game_start_error start_error);
const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status);