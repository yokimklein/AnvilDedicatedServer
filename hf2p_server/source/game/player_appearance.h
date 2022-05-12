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

struct s_s3d_player_appearance_unknown1 // sizeof=0xA - ED PlayerCustomization?
{
	byte __unknown0;
	byte __unknown1;
	byte __unknown2;
	byte __unknown3;
	byte __unknown4;
	byte __unknown5;
	byte __unknown6[4];
};
static_assert(sizeof(s_s3d_player_appearance_unknown1) == 0xA);

struct s_s3d_player_appearance_unknown2
{
	uint32_t unknown[95];
};
static_assert(sizeof(s_s3d_player_appearance_unknown2) == 0x17C);

struct s_s3d_player_appearance
{
	byte unknown1; // index or count
	s_s3d_player_appearance_unknown1 unknown2[3]; // colors?
	byte align1F[1];
	s_s3d_player_appearance_unknown2 unknown3[3]; // armor tagblock indices?
};
static_assert(sizeof(s_s3d_player_appearance) == 0x494);