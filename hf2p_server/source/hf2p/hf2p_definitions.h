#pragma once
#include <tag_files\tag_groups.h>

struct s_armor_objects
{
	c_static_string<32> name;
	c_typed_tag_reference<ARMOR_TAG> armor;
	c_string_id variant;
};
static_assert(sizeof(s_armor_objects) == 0x34);

struct s_armor_gender
{
	c_typed_tag_block<s_armor_objects> armor_objects;
};
static_assert(sizeof(s_armor_gender) == 0xC);

struct s_armor_race
{
	c_typed_tag_block<s_armor_gender> genders;
};
static_assert(sizeof(s_armor_race) == 0xC);

struct hf2p_globals_definition
{
	static tag const k_group_tag = HF2P_GLOBALS_TAG;

	c_typed_tag_block<s_armor_race> race_armors;
};
static_assert(sizeof(hf2p_globals_definition) == 0xC);