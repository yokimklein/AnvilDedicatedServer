#pragma once
#include "render\camera_fx_settings.h"

#define MAXIMUM_PLAYER_WINDOWS 4

struct s_render_game_state
{
public:
	//static void __cdecl dispose();
	//static void __cdecl dispose_from_old_map();
	//static void __cdecl initialize();
	//static void __cdecl initialize_for_new_map();

	struct s_player_window
	{
		c_camera_fx_settings m_camera_fx_settings;
		c_camera_fx_values m_camera_fx_values;
	};
	static_assert(sizeof(s_player_window) == 0x360);

	s_player_window player_window[MAXIMUM_PLAYER_WINDOWS];
};
static_assert(sizeof(s_render_game_state) == 0xD80);