#pragma once
#include "math\real_math.h"
#include "math\integer_math.h"

struct rasterizer_vertex_debug
{
	real_point3d position;
	pixel32 color;
};
static_assert(sizeof(rasterizer_vertex_debug) == 0x10);