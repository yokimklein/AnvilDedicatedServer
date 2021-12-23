#pragma once

enum e_gui_game_mode : long
{
	_gui_game_mode_none = -1,
	_gui_game_mode_campaign,
	//_gui_game_mode_matchmaking, // removed in later ho builds
	_gui_game_mode_multiplayer,
	_gui_game_mode_mapeditor,
	_gui_game_mode_theater,
	_gui_game_mode_survival,

	k_gui_game_mode_count
};