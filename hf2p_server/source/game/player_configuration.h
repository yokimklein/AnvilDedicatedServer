#pragma once
#include "..\cseries\cseries.h"
#include "player_appearance.h"

// e_multiplayer_team
// k_multiplayer_team_none = -1
// k_multiplayer_max_team_game_and_ffa_game_team_count = 16

enum e_game_team
{
	_game_team_none = -1,
	_game_team_red,
	_game_team_blue,
	_game_team_green,
	_game_team_orange,
	_game_team_purple,
	_game_team_gold,
	_game_team_brown,
	_game_team_pink,

	k_maximum_teams
	//_game_team_observer = 16, // also 8? -1 seems to behave as a spectator mode?
};

enum e_player_vote_selection
{
	_player_vote_none,
	_player_vote_option1,
	_player_vote_option2,

	k_player_vote_selection_count
};

struct s_machine_identifier
{
	long data1;
	long data2;
	long data3;
	long data4;
};

struct s_player_identifier
{
	qword data;
	//dword ip_addr;
	//word port;
	//word_flags flags;
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_player_configuration_from_client
{
	s_player_configuration_from_client() :
		name(),
		user_selected_team_index(),
		vote_selection_index(),
		active_armor_loadout(),
		active_weapon_loadout(),
		player_is_griefer(),
		pad(), // we have to initialise these so leftover garbage bytes don't screw with the membership update checksum
		cheat_flags(),
		ban_flags()
	{
		user_selected_team_index = _game_team_none;
		active_armor_loadout = -1;
	};

	wchar_t name[16];
	c_enum<e_game_team, byte, k_maximum_teams> user_selected_team_index;
	c_enum<e_player_vote_selection, byte, k_player_vote_selection_count> vote_selection_index;
	char active_armor_loadout;
	char active_weapon_loadout; // this might now be unused or something under a different name?
	bool player_is_griefer;
	byte pad[3];
	dword_flags cheat_flags;
	dword_flags ban_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_configuration_from_host() :
		player_xuid(),
		player_name(),
		team_index(),
		user_selected_team_index(),
		player_appearance(),
		s3d_player_container(),
		s3d_player_customization()
	{
		team_index = _game_team_none;
		user_selected_team_index = _game_team_none;
	};

	s_player_identifier player_xuid;
	wchar_t player_name[16];
	c_enum<e_game_team, long, k_maximum_teams> team_index;
	c_enum<e_game_team, long, k_maximum_teams> user_selected_team_index;
	s_player_appearance player_appearance;
	s_s3d_player_container s3d_player_container;
	s_s3d_player_customization s3d_player_customization;
};
static_assert(sizeof(s_player_configuration_from_host) == 0xB40);

struct s_player_configuration
{
	s_player_configuration() : client(), host() {};

	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0xB70);