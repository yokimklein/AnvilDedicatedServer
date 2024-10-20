#pragma once
#include <cseries\cseries.h>
#include <game\game_progression.h>
#include <game\game_options.h>
#include <networking\session\network_session_parameters_base.h>
#include <networking\session\network_session_parameters_generic.h>
#include <networking\session\network_session_parameters_chunked.h>

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
	_network_session_closed_survival_in_progress,

	k_network_session_closed_status_count
};

enum e_session_game_start_status
{
	_session_game_start_status_none = 0,
	_session_game_start_status_not_pregame,
	_session_game_start_status_changing_settings_member,
	_session_game_start_status_join_in_progress,
	_session_game_start_status_loading,
	_session_game_start_status_ready_leader,
	_session_game_start_status_voting,
	_session_game_start_status_countdown,
	_session_game_start_status_error,

	k_session_game_start_status_count
};

// this may be incorrect
enum e_session_game_start_error
{
	_session_game_start_error_none = 0,
	_session_game_start_error_no_map_selected = 1,
	_session_game_start_error_map_load_failure = 2,
	_session_game_start_error_map_load_precaching = 3,
	_session_game_start_error_invalid_film_selected = 4,
	_session_game_start_error_no_film_selected = 5,
	_session_game_start_error_too_many_teams = 6,
	_session_game_start_error_too_many_for_local_coop = 7,
	_session_game_start_error_too_many_for_net_coop = 8,
	_session_game_start_error_incompatible_for_net_coop = 9,
	_session_game_start_error_theater_too_many_players = 10,
	_session_game_start_error_theater_leader_must_be_host = 11,
	_session_game_start_error_theater_all_not_compatible = 12,
	_session_game_start_error_too_many_players_in_forge = 13,

	k_session_game_start_error_count
};

enum e_network_game_start_mode
{
	_network_game_start_mode_campaign,
	_network_game_start_mode_custom_game,
	_network_game_start_mode_matchmaking,
	_network_game_start_mode_film,

	k_number_of_network_game_start_modes
};

enum e_network_game_countdown_delayed_reason
{
	_network_game_countdown_delayed_reason_none, // none may actually be -1, new HO reason taking place of 0?
	_network_game_countdown_delayed_reason_voting,
	_network_game_countdown_delayed_reason_start, // user_interface? TODO: find out what this actually is, none does not work to start after voting

	k_network_game_countdown_delayed_reason_count
};

struct s_network_session_privacy_mode
{
	c_enum<e_network_game_privacy, long, _network_game_privacy_open_to_public, k_network_game_privacy_count> privacy_mode;
	c_enum<e_network_session_closed_status, long, _network_session_closed_none, k_network_session_closed_status_count> closed_mode;
	long maximum_player_count;
	bool is_closed_by_user;
};
static_assert(sizeof(s_network_session_privacy_mode) == 0x10);

struct s_network_session_parameter_map
{
	long campaign_id;
	long map_id;
	c_static_string<128> scenario_path;
	s_campaign_armaments campaign_armaments;
	s_campaign_game_progression campaign_progression;
	s_hub_progression hub_progression;
};
static_assert(sizeof(s_network_session_parameter_map) == 0x200);

struct s_network_session_parameter_initial_participants
{
	bool machine_options_exists;
	game_machine_options machine_options;
	bool player_options_exists;
	game_player_options player_options[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_network_session_parameter_initial_participants) == 0xBA30);

class c_network_session_parameter_initial_participants : public c_network_session_parameter_chunked<s_network_session_parameter_initial_participants, 0x1F800>
{
};
static_assert(sizeof(c_network_session_parameter_initial_participants) == 0x56500);

struct s_network_session_parameter_game_start_status
{
	c_enum<e_session_game_start_status, long, _session_game_start_status_none, k_session_game_start_status_count> game_start_status;
	c_enum<e_session_game_start_error, long, _session_game_start_error_none, k_session_game_start_error_count> game_start_error;
	word player_error_mask;
	short map_load_progress;
};
static_assert(sizeof(s_network_session_parameter_game_start_status) == 0xC);

class c_network_session_parameter_game_start_status : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_game_start_status>
{
public:
	s_network_session_parameter_game_start_status* get();
	bool set(s_network_session_parameter_game_start_status* start_status);
};
static_assert(sizeof(c_network_session_parameter_game_start_status) == 0x48);

struct s_network_session_parameter_countdown_timer
{
	c_enum<e_network_game_countdown_delayed_reason, long, _network_game_countdown_delayed_reason_none, k_network_game_countdown_delayed_reason_count> delayed_reason;
	long countdown_timer;
};
static_assert(sizeof(s_network_session_parameter_countdown_timer) == 0x8);

class c_network_session_parameter_countdown_timer : public c_network_session_parameter_base, c_generic_network_session_parameter_data<s_network_session_parameter_countdown_timer>
{
public:
	bool set(e_network_game_countdown_delayed_reason delayed_reason, long countdown_timer);
	long get_countdown_timer();
	e_network_game_countdown_delayed_reason get_delayed_reason();
};
static_assert(sizeof(c_network_session_parameter_countdown_timer) == 0x40);

struct s_network_session_parameter_synchronous_out_of_sync
{
	bool synchronous_out_of_sync;
	bool synchronous_determinism_failure;
};
static_assert(sizeof(s_network_session_parameter_synchronous_out_of_sync) == 0x2);

struct s_network_session_parameter_request_campaign_quit
{
	long campaign_quit_reason;
	long campaign_quitting_peer;
};
static_assert(sizeof(s_network_session_parameter_request_campaign_quit) == 0x8);

const char* multiplayer_game_start_error_to_string(e_session_game_start_error start_error);
const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status);