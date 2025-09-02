#pragma once
#include <cseries\cseries.h>
#include <math\real_math.h>

struct s_dead_player_info
{
	long player_index;
	long time_of_death;
	real_point3d location;
};
static_assert(sizeof(s_dead_player_info) == 0x14);