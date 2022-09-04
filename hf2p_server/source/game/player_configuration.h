#pragma once
#include <cstdint>
#include <windows.h>
#include "player_appearance.h"
#include "players.h"

struct s_player_configuration_from_client
{
	s_player_configuration_from_client() :
		player_name(),
		multiplayer_team(-1),
		unknown_byte1(),
		active_armor_loadout(-1),
		unknown_bool1(),
		unknown_bool2(),
		pad(), // we have to initialise these so leftover garbage bytes don't screw with the membership update checksum
		cheating_flags(),
		user_experience_flags()
	{
	};

	wchar_t player_name[16];
	byte multiplayer_team;
	byte unknown_byte1; // active_weapon_loadout?
	byte active_armor_loadout;
	bool unknown_bool1;
	bool unknown_bool2;
	byte pad[3];
	uint32_t cheating_flags; // old h3 name that likely changed
	uint32_t user_experience_flags; // old h3 name that likely changed
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_configuration_from_host() :
		player_xuid(),
		player_name(),
		player_team(-1),
		player_assigned_team(-1),
		player_appearance(),
		s3d_player_appearance(),
		s3d_player_customization()
	{
	};

	s_player_identifier player_xuid;
	wchar_t player_name[16];
	long player_team;
	long player_assigned_team;
	s_player_appearance player_appearance;
	s_s3d_player_appearance s3d_player_appearance;
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