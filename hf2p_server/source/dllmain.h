#pragma once
#include "cseries\cseries.h"
#include <windows.h>

static dword module_base = (dword)GetModuleHandle(NULL);

bool game_is_dedicated_server();