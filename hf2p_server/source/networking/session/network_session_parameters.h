#pragma once
#include <cseries\cseries.h>
#include <networking\session\network_session_parameters_session.h>
#include <networking\session\network_session_parameters_game.h>
#include <networking\session\network_session_parameters_game_variant.h>
#include <networking\session\network_session_parameters_map_variant.h>
#include <networking\session\network_session_parameters_saved_film_game_options.h>
#include <networking\session\network_session_parameters_matchmaking.h>
#include <networking\session\network_session_parameter_types.h>
#include <networking\session\network_session_parameters_ui.h>
#include <networking\messages\network_messages_session_parameters.h>
#include <networking\session\network_session_parameters_generic.h>
#include <shell\shell.h>
#include <simulation\simulation.h>
#include <networking\session\network_session_state.h>
#include <cseries\language.h>

enum e_network_rough_quality
{

};

#pragma pack(push, 4)
class c_network_session;
class c_network_observer;
class c_network_session_parameters
{
public:
	void check_to_send_updates();
	void check_to_send_change_requests();
	void update();
	bool handle_change_request(bool is_leader, s_network_message_parameters_request const* message);
	const char* get_session_description();

	c_network_session_parameter_session_mode session_mode;
	c_network_session_parameter_session_size session_size;
	c_generic_network_session_parameter<s_network_session_privacy_mode> privacy_mode;
	c_network_session_parameter_ui_game_mode ui_game_mode;
	c_generic_network_session_parameter<e_dedicated_server_session_state> dedicated_server_session_state; // value between 0-10
	c_generic_network_session_parameter<s_network_session_parameter_map> map;
	c_network_session_parameter_initial_participants initial_participants;
	c_generic_network_session_parameter<qword> game_instance;
	c_generic_network_session_parameter<dword> random_seed;
	c_generic_network_session_parameter<e_language> language;
	c_generic_network_session_parameter<long> determinism_version;
	c_generic_network_session_parameter<e_network_game_simulation_protocol> game_simulation_protocol;
	c_generic_network_session_parameter<e_campaign_difficulty_level> campaign_difficulty;
	c_generic_network_session_parameter<c_flags<e_campaign_skulls_primary, dword, k_campaign_skulls_primary_count>> campaign_active_skulls_primary;
	c_generic_network_session_parameter<c_flags<e_campaign_skulls_secondary, dword, k_campaign_skulls_secondary_count>> campaign_active_skulls_secondary;
	c_generic_network_session_parameter<short> campaign_metagame_scoring;
	c_generic_network_session_parameter<short> campaign_insertion_point;
	c_generic_network_session_parameter<s_network_ui_state> ui_state;
	c_generic_network_session_parameter<bool> end_game;
	c_generic_network_session_parameter<e_network_game_start_mode> start_mode;
	c_network_session_parameter_game_variant game_variant;
	c_network_session_parameter_map_variant map_variant;
	c_generic_network_session_parameter<s_saved_film_description> saved_film;
	c_network_session_parameter_saved_film_game_options saved_film_game_options;
	c_network_session_parameter_game_start_status game_start_status;
	c_network_session_parameter_countdown_timer countdown_timer;
	c_generic_network_session_parameter<long> voice_repeater; // peer_index
	c_network_session_parameter_requested_remote_join_data requested_remote_join_data;
	c_network_session_parameter_remote_join_data remote_join_data;
	c_generic_network_session_parameter<s_network_session_parameter_synchronous_out_of_sync> synchronous_out_of_sync;
	c_network_session_parameter_matchmaking_messaging matchmaking_messaging;
	c_generic_network_session_parameter<e_network_rough_quality> host_frame_quality;
	c_generic_network_session_parameter<s_network_session_parameter_request_campaign_quit> request_campaign_quit;
	c_generic_network_session_parameter<s_network_session_parameter_leader_properties> leader_properties;
	c_network_session_parameter_lobby_vote_set lobby_vote_set;
	long : 32;
	c_network_session* session;
	c_network_observer* observer;
	c_network_session_parameter_base* parameters[k_network_session_parameter_type_count];
	ulong flags;
	ulong initial_parameters_update_mask;
};
static_assert(sizeof(c_network_session_parameters) == 0xCA114);
#pragma pack(pop)