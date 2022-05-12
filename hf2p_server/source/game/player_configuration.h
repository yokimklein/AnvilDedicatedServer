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
	byte active_weapon_loadout;
	uint32_t hopper_access_flags;
	uint32_t cheating_flags;
	uint32_t user_experience_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host_unknown
{
	byte unknown0;
	byte unknown1;
	byte unknown2;
	byte unknown3;
	uint32_t unknown4[5]; // colours? primary, secondary, visor, lights, holo?
	byte data[4];
};
static_assert(sizeof(s_player_configuration_from_host_unknown) == 0x1C);

struct s_player_configuration_from_host
{
	s_player_identifier player_xuid;
	wchar_t player_name[16];
	long player_team;
	long player_assigned_team;
	s_player_appearance player_appearance;
	s_s3d_player_appearance saber_appearance;
	s_player_configuration_from_host_unknown unknown_data; // PlayerCustomization from ED? contains colour and armour info
};
static_assert(sizeof(s_player_configuration_from_host) == 0xB40);

struct s_player_configuration
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0xB70);