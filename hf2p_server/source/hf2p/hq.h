#pragma once
#include <cseries\cseries.h>
#include <text\unicode.h>

extern c_static_string<128> g_tutorial_scenario_path;
extern c_static_wchar_string<128> g_tutorial_map_name;

void __cdecl hq_start_tutorial_level();