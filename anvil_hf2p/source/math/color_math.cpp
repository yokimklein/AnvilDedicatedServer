#include "color_math.h"
#include "cseries\cseries.h"
#include <math.h>

real_argb_color* pixel32_to_real_argb_color(pixel32 color, real_argb_color* real_color)
{
	real_color->alpha = ((real)color.alpha + 0.0f) / 255.0f;
	real_color->red = ((real)color.red + 0.0f) / 255.0f;
	real_color->green = ((real)color.green + 0.0f) / 255.0f;
	real_color->blue = ((real)color.blue + 0.0f) / 255.0f;
	return real_color;
}

pixel32 real_argb_color_to_pixel32(const real_argb_color* real_color)
{
	return
	{
		static_cast<byte>(real_color->rgb.blue * 255.0),
		static_cast<byte>(real_color->rgb.green * 255.0),
		static_cast<byte>(real_color->rgb.red * 255.0),
		static_cast<byte>(real_color->alpha * 255.0)
	};
}

real value_degamma(real value)
{
	real v3 = value;
	if (value <= 0.0f)
		v3 = 0.0f;

	real v4 = value;
	if (value <= 0.0f)
		v4 = 0.0f;

	real v5 = v4;
	if (v3 >= 1.0f)
		v5 = 1.0f;

	return powf(v5, 2.2f);
}

real value_regamma(real value)
{
	real v3 = value;
	if (value <= 0.0f)
		v3 = 0.0f;

	real v4 = value;
	if (value <= 0.0f)
		v4 = 0.0f;

	real v5 = v4;
	if (v3 >= 1.0f)
		v5 = 1.0f;

	return powf(v5, 0.45454544f);
}

void color_degamma(const real_rgb_color* gamma_color, real_linear_rgb_color* linear_color)
{
	ASSERT(gamma_color && gamma_color->red >= 0.f && gamma_color->green >= 0.f && gamma_color->blue >= 0.f);

	linear_color->red = value_degamma(gamma_color->red);
	linear_color->green = value_degamma(gamma_color->green);
	linear_color->blue = value_degamma(gamma_color->blue);
}

void color_regamma(const real_linear_rgb_color* linear_color, real_rgb_color* gamma_color)
{
	ASSERT(linear_color && linear_color->red >= 0.f && linear_color->green >= 0.f && linear_color->blue >= 0.f);

	gamma_color->red = value_regamma(linear_color->red);
	gamma_color->green = value_regamma(linear_color->green);
	gamma_color->blue = value_regamma(linear_color->blue);
}

real_rgb_color interpolate_real_rgb_color(const real_rgb_color* a, const real_rgb_color* b, real t)
{
	real_rgb_color result;
	result.red = interpolate_linear(a->red, b->red, t);
	result.green = interpolate_linear(a->green, b->green, t);
	result.blue = interpolate_linear(a->blue, b->blue, t);
	return result;
}