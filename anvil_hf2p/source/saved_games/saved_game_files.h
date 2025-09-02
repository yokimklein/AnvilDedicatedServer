#pragma once
#include <cseries\cseries.h>
#include <objects\objects.h>

// this probably belongs in saved_games\\content_catalogue.h
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

enum e_saved_game_file_type
{
	_saved_game_file_type_none = -1,
	_saved_game_file_type_personal,
	_saved_game_file_type_ctf,
	_saved_game_file_type_slayer,
	_saved_game_file_type_oddball,
	_saved_game_file_type_king,
	_saved_game_file_type_juggernaut,
	_saved_game_file_type_territories,
	_saved_game_file_type_assault,
	_saved_game_file_type_infection,
	_saved_game_file_type_vip,
	_saved_game_file_type_usermap,
	_saved_game_file_type_film,
	_saved_game_file_type_clip,
	_saved_game_file_type_screenshot,

	k_saved_game_file_type_count
};

struct s_content_item_metadata
{
	qword unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	c_enum<e_saved_game_file_type, long, _saved_game_file_type_none, k_saved_game_file_type_count> file_type;
	bool author_is_xuid_online;
	byte pad0[3];
	qword author_id;
	qword size_in_bytes;
	qword date;
	long length_seconds;
	long campaign_id;
	long map_id;
	long game_engine_type;
	long campaign_difficulty;
	byte campaign_insertion_point;
	bool campaign_survival_enabled;
	byte pad[1];
	qword game_id;
};
static_assert(sizeof(s_content_item_metadata) == 0xF8);