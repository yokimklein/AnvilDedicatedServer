#pragma once
#include "cseries\cseries.h"
#include <windows.h>

static const dword module_base = (dword)GetModuleHandle(NULL); // TODO: move to cseries
#define FUNCTION_DEF(ADDR, RET, CC, NAME, ...) static const auto NAME = (RET(CC*)(__VA_ARGS__))(module_base + ADDR) // TODO: move to cseries

bool game_is_dedicated_server(); // TODO: move to game.h or server tools