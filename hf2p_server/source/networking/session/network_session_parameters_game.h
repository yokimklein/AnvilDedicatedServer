#pragma once
#include <cstdint>
#include <windows.h>
#include "network_session_parameters_base.h"
#include "network_session_membership.h"
#include "..\..\game\game_options.h"
#include "..\..\game\players.h"
#include "..\..\simulation\simulation.h"
#include "..\..\interface\gui_pregame_setup_manager.h"

// non-original names, may be incorrect
enum e_network_game_privacy
{
	_network_game_privacy_open_to_public,
	_network_game_privacy_open_to_friends,
	_network_game_privacy_invite_only,
	_network_game_privacy_closed,

	//_network_game_privacy_full?
	//_network_game_privacy_offline?
	//_network_game_privacy_system_link?

	k_network_game_privacy_count
};

enum e_network_session_closed
{

};

struct s_network_session_privacy_mode
{
	e_network_game_privacy privacy_mode;
	e_network_session_closed closed_mode;
	long maximum_player_count;
	bool is_closed_by_user;
};
static_assert(sizeof(s_network_session_privacy_mode) == 0x10);

struct c_generic_network_session_parameter_privacy_mode : c_network_session_parameter_base
{
	s_network_session_privacy_mode m_data;
	s_network_session_privacy_mode m_requested_data;
};

struct c_network_session_parameter_ui_game_mode: c_network_session_parameter_base
{
	e_gui_game_mode m_data;
	e_gui_game_mode m_requested_data;
};

struct c_generic_network_session_parameter_dedicated_server_session_state: c_network_session_parameter_base
{
	int m_data;
	int m_requested_data;
};

struct s_network_session_parameter_map
{
	int campaign_id;
	int map_id;
	char map_name[128];
	byte campagin_arguments[120];
	uint32_t game_progression_bit_vector[64];
};

struct c_network_session_parameter_map : c_network_session_parameter_base
{
	s_network_session_parameter_map m_data;
	s_network_session_parameter_map m_requested_data;
};

struct s_network_session_parameter_initial_participants
{
	bool machine_options_exists;
	s_game_machine_options machine_options;
	bool player_options_exists;
	s_game_player_options player_options[k_network_maximum_players_per_session];
};

struct c_network_session_parameter_initial_participants : c_network_session_parameter_base
{
	s_network_session_parameter_initial_participants m_data;
	s_network_session_parameter_initial_participants m_requested_data;
	int m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	int m_next_request_chunk;
	int m_expected_update_chunk;
	int m_expected_change_chunk;
	char chunked_update_data[0x1F800];
	int chunked_update_data_size;
	int chunked_update_data_checksum;
	int chunked_update_data_chunk_count;
	char chunked_change_data[0x1F800];
	int chunked_change_data_size;
	int chunked_change_data_checksum;
	int chunked_change_data_chunk_count;
	int m_max_chunk_count;
	long : 32;
};

struct s_network_session_parameter_game_start_status
{
	long game_start_status;
	long game_start_error;
	unsigned short player_error_mask;
	unsigned short map_load_progress;
};
static_assert(sizeof(s_network_session_parameter_game_start_status) == 0xC);

struct c_network_session_parameter_game_start_status : c_network_session_parameter_base
{
	s_network_session_parameter_game_start_status m_data;
	s_network_session_parameter_game_start_status m_requested_data;
};

struct s_network_session_parameter_countdown_timer
{
	long : 32;
	long : 32;
};

struct c_network_session_parameter_countdown_timer : c_network_session_parameter_base
{
	s_network_session_parameter_countdown_timer m_data;
	s_network_session_parameter_countdown_timer m_requested_data;
};

struct s_network_session_parameter_voice_repeater
{
	long peer_index;
};
static_assert(sizeof(s_network_session_parameter_voice_repeater) == 0x4);

struct c_network_session_parameter_voice_repeater : c_network_session_parameter_base
{
	s_network_session_parameter_voice_repeater m_data;
	s_network_session_parameter_voice_repeater m_requested_data;
};

struct s_network_session_parameter_synchronous_out_of_sync
{
	short : 16;
};

struct c_network_session_parameter_synchronous_out_of_sync : c_network_session_parameter_base
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

struct c_network_session_parameter_request_campaign_quit : c_network_session_parameter_base
{
	s_network_session_parameter_request_campaign_quit m_data;
	s_network_session_parameter_request_campaign_quit m_requested_data;
};