#pragma once
#include <cstdint>
#include <windows.h>
#include "game_engine_player_traits.h"
#include "..\memory\bitstream.h"

enum e_map_id : long
{
	_mainmenu = 0,
	_zanzibar = 30,
	_s3d_turf = 31,
	_deadlock = 310,
	_guardian = 320,
	_riverworld = 340,
	_chill = 380,
	_cyberdyne = 390,
	_bunkerworld = 410,
	_s3d_reactor = 700,
	_s3d_edge = 703,
	_s3d_avalanche = 705

	/*
	_s3d_waterfall = 706
	s3d_beachhead
	s3d_cold_storage
	s3d_dry_dock
	s3d_lockout
	s3d_tutorial
	s3d_powerhouse
	s3d_sky_bridgenew
	*/
};

enum e_engine_variant : long
{
	_engine_variant_base,
	_engine_variant_ctf,
	_engine_variant_slayer,
	_engine_variant_oddball,
	_engine_variant_king,
	_engine_variant_sandbox,
	_engine_variant_vip,
	_engine_variant_juggernaut,
	_engine_variant_territories,
	_engine_variant_assault,
	_engine_variant_infection,
};

enum e_content_item
{
	_content_item_usermap,
	_content_item_personal,
	_content_item_ctf,
	_content_item_slayer,
	_content_item_oddball,
	_content_item_koth,
	_content_item_juggernaut,
	_content_item_territories,
	_content_item_assault,
	_content_item_vip,
	_content_item_infection,
	_content_item_film,
	_content_item_clip,
	_content_item_screenshot,

	k_content_item_count
};

struct s_content_item_metadata
{
	uint64_t m_id;
	wchar_t m_name[16];
	char m_description[128];
	char m_author[16];
	e_content_item m_content_type;
	bool m_user_is_online;
	uint64_t m_user_id;
	uint64_t m_size;
	uint64_t m_timestamp;
	uint32_t m_unknown;
	uint32_t m_campaign_id;
	uint32_t m_map_id;
	uint32_t m_engine;
	uint32_t m_campaign_difficulty;
	uint8_t m_campaign_insertion;
	bool m_survival;
	uint64_t m_game_instance;
};
static_assert(sizeof(s_content_item_metadata) == 0xF8);

struct c_game_engine_variant_general_settings
{
	byte m_flags;
	byte m_time_limit;
	byte m_number_of_rounds;
	byte m_early_victory_win_count;
};
static_assert(sizeof(c_game_engine_variant_general_settings) == 0x4);

struct c_game_engine_variant_respawn_settings
{
	byte m_flags;
	byte m_lives_per_round;
	byte m_shared_team_lives;
	byte m_respawn_time;
	byte m_suicide_penalty;
	byte m_betrayal_penalty;
	byte m_unknown_penalty;
	byte m_respawn_time_growth;
	byte m_respawn_trait_duration;
	short unknown9;
	c_player_traits m_respawn_trait_profile;
};
static_assert(sizeof(c_game_engine_variant_respawn_settings) == 0x28);

struct c_game_engine_variant_social_settings
{
	short m_flags;
	short m_team_changing;
};
static_assert(sizeof(c_game_engine_variant_social_settings) == 0x4);

struct c_game_engine_variant_map_overrides
{
	long m_flags;
	c_player_traits m_base_traits;
	short m_weapon_set;
	short m_vehicle_set;
	c_player_traits m_red_traits;
	c_player_traits m_blue_traits;
	c_player_traits m_yellow_traits;
	byte m_red_traits_duration;
	byte m_blue_traits_duration;
	byte m_yellow_traits_duration;
	byte unknwon7B;
};
static_assert(sizeof(c_game_engine_variant_map_overrides) == 0x7C);

struct c_game_engine_variant_vtbl;
struct c_game_engine_base_variant
{
	c_game_engine_variant_vtbl* vftable;
	long* SHA1_C;
	char m_variant_backend_name[32];
	s_content_item_metadata m_content_header;
	c_game_engine_variant_general_settings m_general_settings;
	c_game_engine_variant_respawn_settings m_respawn_settings;
	c_game_engine_variant_social_settings m_social_settings;
	c_game_engine_variant_map_overrides m_map_overrides;
	uint16_t m_flags;
	int16_t m_team_scoring;
};
static_assert(sizeof(c_game_engine_base_variant) == 0x1D0);

struct c_game_engine_variant : c_game_engine_base_variant
{
	uint8_t m_specific_data[144];
};
static_assert(sizeof(c_game_engine_variant) == 0x260);

struct c_game_variant
{
	e_engine_variant m_game_engine_index;
	byte storage[0x260]; // TODO - c_game_engine_variant union with different engine/gamemode types
};
static_assert(sizeof(c_game_variant) == 0x264);

struct c_game_engine_variant_vtbl
{
	long(__cdecl* get_name)();
	long(__thiscall* get_description)(c_game_engine_variant*);
	long(__thiscall* make_default)(c_game_engine_variant*);
	void(__thiscall* build_settings)(c_game_engine_variant*);
	void* backend_encode;
	void(__thiscall* backend_decode)(c_game_engine_variant*, c_bitstream*);
	long(__cdecl* func06)();
	long(__thiscall* score_to_win)(c_game_engine_variant*);
	long(__thiscall* func08)(c_game_engine_variant*);
	void(__stdcall* get_engine)(c_game_engine_variant*, e_engine_variant, c_game_engine_variant**);
	long(__stdcall* func10)(long, long, long);
};