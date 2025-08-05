#pragma once
#include <cseries\cseries.h>
#include <game\player_appearance.h>
#include <networking\transport\transport_security.h>

// e_multiplayer_team
// k_multiplayer_team_none = -1

enum e_game_team
{
	_game_team_none = -1,

	_campaign_team_default = 0,
	_campaign_team_player,
	_campaign_team_human,
	_campaign_team_covenant,
	_campaign_team_flood,
	_campaign_team_sentinel,
	_campaign_team_heretic,
	_campaign_team_prophet,
	_campaign_team_guilty,
	_campaign_team_unused9,
	_campaign_team_unused10,
	_campaign_team_unused11,
	_campaign_team_unused12,
	_campaign_team_unused13,
	_campaign_team_unused14,
	_campaign_team_unused15,

	k_campaign_team_count,

	_multiplayer_team_first = 0,
	_multiplayer_team_red = _multiplayer_team_first,
	_multiplayer_team_blue,
	_multiplayer_team_green,
	_multiplayer_team_yellow,
	_multiplayer_team_purple,
	_multiplayer_team_orange,
	_multiplayer_team_brown,
	_multiplayer_team_grey,

	k_multiplayer_team_count,

	_multiplayer_team_last = _multiplayer_team_grey,
	_multiplayer_team_none = -1,

	k_maximum_teams = 16
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
	s_transport_unique_identifier unique_identifiers;
};
static_assert(sizeof(s_machine_identifier) == 0x10);


struct s_player_identifier
{
	bool operator==(s_player_identifier other) const { return csmemcmp(this, &other, sizeof(*this)) == 0; };
	bool operator!=(s_player_identifier other) const { return csmemcmp(this, &other, sizeof(*this)) != 0; };

	byte identifier[8];
	//union
	//{
	//	dword parts[2];
	//	qword xuid;
	//};
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_player_configuration_from_client
{
	s_player_configuration_from_client()
	{
		csmemset(this, 0, sizeof(*this));
	};

	c_static_wchar_string<16> desired_name;
	c_enum<e_game_team, byte, _game_team_none, k_multiplayer_team_count> user_selected_team_index;
	c_enum<e_player_vote_selection, byte, _player_vote_none, k_player_vote_selection_count> vote_selection_index;
	byte selected_loadout_index;
	byte pad1; // active_weapon_loadout used to live here, likely now just padding
	bool player_is_griefer;
	byte pad2[3];
	dword_flags cheat_flags;
	dword_flags ban_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_configuration_from_host()
	{
		csmemset(this, 0, sizeof(*this));
		team_index = _multiplayer_team_none;
		assigned_team_index = _multiplayer_team_none;
	};

	qword user_xuid;
	c_static_wchar_string<16> player_name;
	e_game_team team_index;
	e_game_team assigned_team_index;
	s_player_appearance player_appearance;
	s_s3d_player_container s3d_player_container;
	s_s3d_player_customization s3d_player_customization;
};
static_assert(sizeof(s_player_configuration_from_host) == 0xB40);

struct s_player_configuration
{
	s_player_configuration()
	{
		client = {};
		host = {};
	}
	bool operator==(s_player_configuration other) const { return csmemcmp(this, &other, sizeof(*this)) == 0; };
	bool operator!=(s_player_configuration other) const { return csmemcmp(this, &other, sizeof(*this)) != 0; };

	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0xB70);