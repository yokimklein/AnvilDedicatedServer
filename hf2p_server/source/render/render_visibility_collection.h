#pragma once
#include "..\cseries\cseries.h"

struct s_game_cluster_bit_vectors
{
	c_static_array<c_static_flags<256>, 16> flags;
};
static_assert(sizeof(s_game_cluster_bit_vectors) == 0x200);