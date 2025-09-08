#pragma once
#include <windows.h>

struct s_windows_params
{
	HINSTANCE instance;
	HWND game_window_handle;
	HWND window_handle;
	int cmd_show;
	WNDPROC window_proc;
	CHAR class_name[64];
	CHAR window_name[64];
	LPSTR cmd_line;

	static bool editor_window_create;
	static HWND editor_window_handle;
	static WNDPROC editor_window_proc;
	static CHAR editor_class_name[64];
	static CHAR editor_window_name[64];
};
static_assert(sizeof(s_windows_params));

extern s_windows_params& g_windows_params;