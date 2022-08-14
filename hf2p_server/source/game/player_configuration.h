#pragma once
#include <cstdint>
#include <windows.h>
#include "player_appearance.h"
#include "players.h"

struct s_player_configuration_from_client
{
	wchar_t player_name[16];
	byte multiplayer_team;
	byte unknown_team; // swap with active_armor_loadout?
	byte active_armor_loadout;
	byte unknown_bool; // previously active_weapon_loadout, now some boolean?
	uint32_t hopper_access_flags;
	uint32_t cheating_flags;
	uint32_t user_experience_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
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
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0xB70);