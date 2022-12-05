#pragma once
#include "..\cseries\cseries.h"
#include "..\objects\objects.h"

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

struct s_saved_game_item_metadata
{
	qword m_id;
	wchar_t m_name[16];
	char m_description[128];
	char m_author[16];
	e_content_item m_content_type;
	bool m_user_is_online;
	qword m_user_id;
	qword m_size;
	qword m_timestamp;
	long : 32;
	long m_campaign_id;
	long m_map_id;
	long m_engine;
	ulong m_campaign_difficulty;
	byte m_campaign_insertion;
	bool m_survival;
	qword m_game_instance;
};
static_assert(sizeof(s_saved_game_item_metadata) == 0xF8);