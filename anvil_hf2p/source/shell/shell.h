#pragma once

// $TODO: Find a better home for these

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mainmenu,
	_game_mode_shared,

	k_game_mode_count
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_number_of_campaign_difficulty_levels
};

// e_game_skulls
enum e_campaign_skulls_primary
{
	_campaign_skull_iron = 0,
	_campaign_skull_black_eye,
	_campaign_skull_tough_luck,
	_campaign_skull_catch,
	_campaign_skull_fog,
	_campaign_skull_famine,
	_campaign_skull_thunderstorm,
	_campaign_skull_tilt,
	_campaign_skull_mythic,

	k_campaign_skulls_primary_count
};

enum e_campaign_skulls_secondary
{
	_campaign_skull_assassin = 0,
	_campaign_skull_blind,
	_campaign_skull_superman,
	_campaign_skull_birthday_party,
	_campaign_skull_daddy,
	_campaign_skull_third_person,
	_campaign_skull_directors_cut,

	k_campaign_skulls_secondary_count
};

// TODO: find a home
enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count
};

enum e_player_model_choice
{

};

enum e_shell_application_type
{
	_shell_application_tool = 0,
	_shell_application_editor,
	_shell_application_game,

	k_shell_application_count
};

enum e_shell_tool_type
{
	_shell_tool_invalid = 0,
	_shell_tool_interactive,
	_shell_tool_command_line,

	k_shell_tool_count
};

extern e_shell_application_type shell_application_type();
extern e_shell_tool_type shell_tool_type();
extern bool shell_application_is_paused();
const char* shell_get_version();