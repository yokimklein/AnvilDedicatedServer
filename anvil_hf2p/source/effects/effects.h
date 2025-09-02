#pragma once
#include <cseries\cseries.h>
#include <math\real_math.h>

struct s_effect_vector
{
	real_point3d point;
	real_vector3d direction;
	c_string_id name;
};
static_assert(sizeof(s_effect_vector) == 0x1C);