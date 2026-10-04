#pragma once
#include "cseries\cseries.h"

struct s_tag_persistent_identifier
{
	ulong data[4];
};
static_assert(sizeof(s_tag_persistent_identifier) == 0x10);
