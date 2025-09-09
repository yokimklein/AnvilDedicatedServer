#pragma once
#include "math\integer_math.h"
#include "math\real_math.h"

extern real_argb_color* pixel32_to_real_argb_color(pixel32 color, real_argb_color* real_color);
extern pixel32 real_argb_color_to_pixel32(const real_argb_color* real_color);

extern real value_degamma(real value);
extern real value_regamma(real value);
extern void color_degamma(const real_rgb_color* gamma_color, real_linear_rgb_color* linear_color);
extern void color_regamma(const real_linear_rgb_color* linear_color, real_rgb_color* gamma_color);

extern real_rgb_color interpolate_real_rgb_color(const real_rgb_color* a, const real_rgb_color* b, real t);