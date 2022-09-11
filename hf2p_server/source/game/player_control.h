#pragma once
#include "..\cseries\cseries.h"

struct player_action
{
	byte unknown_data[0x80];
};
static_assert(sizeof(player_action) == 0x80);