#pragma once
#include <cseries\cseries.h>
#include <objects\damage.h>

enum e_game_progression_level
{
	_game_progression_level_none = 0,
	_game_progression_level_normal,
	_game_progression_level_hub_and_level_is_hub,
	_game_progression_level_spoke_and_level_is_spoke,

	k_game_progression_level_count
};

struct s_campaign_armaments_weapon
{
	c_enum<e_damage_reporting_type, short, _damage_reporting_type_unknown, k_damage_reporting_type_count> damage_reporting_type;
	short rounds_loaded_maximum;
	short runtime_rounds_inventory_maximum;
	short rounds_loaded_amount;
};
static_assert(sizeof(s_campaign_armaments_weapon) == 0x8);

struct s_campaign_armaments_player
{
	bool valid;
	byte : 8;

	s_campaign_armaments_weapon primary_weapon;
	s_campaign_armaments_weapon backpack_weapon;
	s_campaign_armaments_weapon secondary_weapon;
	c_static_array<byte, 4> grenade_counts;
};
static_assert(sizeof(s_campaign_armaments_player) == 0x1E);

struct s_campaign_armaments
{
	c_static_array<s_campaign_armaments_player, 4> players;
};
static_assert(sizeof(s_campaign_armaments) == 0x78);

struct s_campaign_game_progression
{
	c_static_array<dword, 32> integer_names;
};
static_assert(sizeof(s_campaign_game_progression) == 0x80);

struct s_hub_progression
{
	s_campaign_armaments hub_armaments;
	long hub_return_to_insertion_point;
	bool hub_progression_valid;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_hub_progression) == 0x80);