#pragma once
#include "cseries\cseries.h"
#include <windows.h>

static const dword module_base = (dword)GetModuleHandle(NULL);
#define FUNCTION_DEF(ADDR, RET, CC, NAME, ...) static const auto NAME = (RET(CC*)(__VA_ARGS__))(module_base + ADDR)

bool game_is_dedicated_server();