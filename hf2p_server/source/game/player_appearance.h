#pragma once
#include <cstdint>
#include <windows.h>

struct s_player_appearance_unknown3
{
	uint16_t unknown0;
	uint16_t unknown2;
	uint16_t unknown4;
	uint16_t unknown6;
	uint16_t unknown8;
	uint16_t unknownA;
	uint16_t unknownC;
	byte unknownE;
	byte unknownF;
};
static_assert(sizeof(s_player_appearance_unknown3) == 0x10);

struct s_player_appearance_unknown2
{
	s_player_appearance_unknown3 unknown[50];
	long unknown_count; // max of 50
};
static_assert(sizeof(s_player_appearance_unknown2) == 0x324);

struct s_player_appearance_unknown1
{
	s_player_appearance_unknown2 unknown1[2];
	uint32_t unknown2[2]; // `unknown1` hashes? only set if `unknown1.unknown_count` != 0
};
static_assert(sizeof(s_player_appearance_unknown1) == 0x650);

struct s_player_appearance // lives in player_appearance.h?
{
	byte flags; // gender
	byte player_model_index;
	byte align2[2];
	s_player_appearance_unknown1 unknown_struct;
	wchar_t service_tag[5];
	byte padding[2];
};
static_assert(sizeof(s_player_appearance) == 0x660);

struct s_s3d_player_loadout
{
	byte flags; // gender
	byte armor;
	byte primary_weapon;
	byte secondary_weapon;
	byte grenade;
	byte support_pack;
	byte tactical_packs[4];
};
static_assert(sizeof(s_s3d_player_loadout) == 0xA);

struct s_s3d_player_armor
{
	uint32_t armor_indices[95];
};
static_assert(sizeof(s_s3d_player_armor) == 0x17C);

struct s_s3d_player_appearance
{
	byte unknown; // has loadouts? current loadout?
	s_s3d_player_loadout loadouts[3]; // 3 loadout sets
	byte align1F[1];
	s_s3d_player_armor armors[3]; // 3 armour sets
};
static_assert(sizeof(s_s3d_player_appearance) == 0x494);

struct s_s3d_player_customization
{
	byte unknown0;
	byte unknown1;
	byte nameplate;
	byte emblem;
	uint32_t colours[5]; // primary, secondary, visor, lights, holo? real_argb
	byte data[4];
};
static_assert(sizeof(s_s3d_player_customization) == 0x1C);