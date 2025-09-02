#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_groups.h>

struct s_game_engine_player_traits
{

};

struct s_game_engine_base_variant_definition
{
	char data[0x58];
};
static_assert(sizeof(s_game_engine_base_variant_definition) == 0x58);

struct s_game_engine_slayer_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x18];
};
static_assert(sizeof(s_game_engine_slayer_variant_definition) == 0x70);

struct s_game_engine_ctf_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x18];
};
static_assert(sizeof(s_game_engine_ctf_variant_definition) == 0x70);

struct s_game_engine_oddball_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x18];
};
static_assert(sizeof(s_game_engine_oddball_variant_definition) == 0x70);

struct s_game_engine_king_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x18];
};
static_assert(sizeof(s_game_engine_king_variant_definition) == 0x70);

struct s_game_engine_sandbox_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x0C];
};
static_assert(sizeof(s_game_engine_sandbox_variant_definition) == 0x64);

struct s_game_engine_vip_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x24];
};
static_assert(sizeof(s_game_engine_vip_variant_definition) == 0x7C);

struct s_game_engine_juggernaut_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x1C];
};
static_assert(sizeof(s_game_engine_juggernaut_variant_definition) == 0x74);

struct s_game_engine_territories_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x14];
};
static_assert(sizeof(s_game_engine_territories_variant_definition) == 0x6C);

struct s_game_engine_assault_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x28];
};
static_assert(sizeof(s_game_engine_assault_variant_definition) == 0x80);

struct s_game_engine_infection_variant_definition : s_game_engine_base_variant_definition
{
	char data[0x24];
};
static_assert(sizeof(s_game_engine_infection_variant_definition) == 0x7C);

struct game_engine_settings_definition
{
	static tag const k_group_tag = GAME_ENGINE_SETTINGS_DEFINITION_TAG;

	dword_flags flags;
	c_typed_tag_block<s_game_engine_player_traits> player_traits;
	c_typed_tag_block<s_game_engine_slayer_variant_definition> slayer_variants;
	c_typed_tag_block<s_game_engine_oddball_variant_definition> oddball_variants;
	c_typed_tag_block<s_game_engine_ctf_variant_definition> ctf_variants;
	c_typed_tag_block<s_game_engine_assault_variant_definition> assault_variants;
	c_typed_tag_block<s_game_engine_infection_variant_definition> infection_variants;
	c_typed_tag_block<s_game_engine_king_variant_definition> koth_variants;
	c_typed_tag_block<s_game_engine_territories_variant_definition> territories_variants;
	c_typed_tag_block<s_game_engine_juggernaut_variant_definition> juggernaut_variants;
	c_typed_tag_block<s_game_engine_vip_variant_definition> vip_variants;
	c_typed_tag_block<s_game_engine_sandbox_variant_definition> sandbox_variants;
	long : 32;
};
static_assert(sizeof(game_engine_settings_definition) == 0x8C);

struct game_engine_settings_definition* game_engine_settings_try_and_get();