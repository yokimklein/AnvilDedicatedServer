#pragma once
#include "..\dllmain.h"

static const bool* g_is_loading = (bool*)(module_base + 0x3F8E6A0);

bool main_game_change_in_progress();