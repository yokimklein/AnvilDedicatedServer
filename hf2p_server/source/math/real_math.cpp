#include "real_math.h"
#include <math.h>
#include <cseries\cseries.h>

REFERENCE_DECLARE(0xD3A038, real_vector3d const, global_up3d);
REFERENCE_DECLARE(0xD39FD8, real_vector3d const, global_down3d);
REFERENCE_DECLARE(0xD3A0D4, real_point3d const, global_origin3d);

real_vector3d* __cdecl cross_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* out)
{
	out->n[0] = real(a->n[1] * b->n[2]) - real(a->n[2] * b->n[1]);
	out->n[1] = real(a->n[2] * b->n[0]) - real(a->n[0] * b->n[2]);
	out->n[2] = real(a->n[0] * b->n[1]) - real(a->n[1] * b->n[0]);

	return out;
}

real __cdecl dot_product3d(real_vector3d const* a, real_vector3d const* b)
{
	return ((a->n[0] * b->n[0]) + (a->n[1] * b->n[1])) + (a->n[2] * b->n[2]);
}

real_vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, real_vector3d* out)
{
	out->n[0] = b->n[0] - a->n[0];
	out->n[1] = b->n[1] - a->n[1];
	out->n[2] = b->n[2] - a->n[2];

	return out;
}

real_point3d* __cdecl point_from_line3d(real_point3d const* in_point, real_vector3d const* in_vector, real scale, real_point3d* out_point)
{
	*out_point = *in_point;
	out_point->n[0] += in_vector->n[0] * scale;
	out_point->n[1] += in_vector->n[1] * scale;
	out_point->n[2] += in_vector->n[2] * scale;

	return out_point;
}

real __cdecl magnitude_squared3d(real_vector3d const* a)
{
	return real(a->n[0] * a->n[0]) + real(a->n[1] * a->n[1]) + real(a->n[2] * a->n[2]);
}

real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b)
{
	real_vector3d temp{};
	return magnitude_squared3d(vector_from_points3d(a, b, &temp));
}

real __cdecl distance3d(real_point3d const* a, real_point3d const* b)
{
	return square_root(distance_squared3d(a, b));
}

real __cdecl square_root(real value)
{
	return sqrtf(value); // sqrt
}

real __cdecl magnitude3d(real_vector3d const* vector)
{
	return square_root(magnitude_squared3d(vector));
}

real_vector3d* __cdecl perpendicular3d(real_vector3d const* vector, real_vector3d* out_vector)
{
	real i = fabsf(vector->i);
	real j = fabsf(vector->j);
	real k = fabsf(vector->k);
	if (i > j || i > k)
	{
		if (j > k)
		{
			out_vector->i = vector->j;
			out_vector->j = -vector->i;
			out_vector->k = 0.0f;
		}
		else
		{
			out_vector->i = -vector->k;
			out_vector->j = 0.0f;
			out_vector->k = vector->i;
		}
	}
	else
	{
		out_vector->i = 0.0f;
		out_vector->j = vector->k;
		out_vector->k = -vector->j;
	}

	return out_vector;
}

real __cdecl normalize3d(real_vector3d* vector)
{
	real result = magnitude3d(vector);
	if (fabsf(result - 0.0f) < k_real_epsilon)
		result = 0.0f;
	else
		scale_vector3d(vector, 1.0f / result, vector);

	return result;
}

real_vector2d* __cdecl rotate_vector2d(real_vector2d const* vector, real a2, real a3, real_vector2d* out_vector)
{
	out_vector->i = (a3 * vector->i) - (a2 * vector->j);
	out_vector->j = (a2 * vector->i) + (a3 * vector->j);

	return out_vector;
}

real_vector3d* __cdecl negate_vector3d(real_vector3d const* in_vector, real_vector3d* out_vector)
{
	out_vector->i = -in_vector->i;
	out_vector->j = -in_vector->j;
	out_vector->k = -in_vector->k;

	return out_vector;
}

real_vector3d* __cdecl scale_vector3d(real_vector3d const* in_vector, real scale, real_vector3d* out_vector)
{
	out_vector->i = scale * in_vector->i;
	out_vector->j = scale * in_vector->j;
	out_vector->k = scale * in_vector->k;

	return out_vector;
}

real_vector3d* __cdecl add_vectors3d(real_vector3d const* in_vector_a, real_vector3d const* in_vector_b, real_vector3d* out_vector)
{
	out_vector->i = in_vector_a->i + in_vector_b->i;
	out_vector->j = in_vector_a->j + in_vector_b->j;
	out_vector->k = in_vector_a->k + in_vector_b->k;

	return out_vector;
}

real_point2d* __cdecl set_real_point2d(real_point2d* point, real x, real y)
{
	point->x = x;
	point->y = y;

	return point;
}

real_point3d* __cdecl set_real_point3d(real_point3d* point, real x, real y, real z)
{
	point->x = x;
	point->y = y;
	point->z = z;

	return point;
}

real_point2d* __cdecl point_from_line2d(real_point2d const* point, real_vector2d const* vector, real scale, real_point2d* out_point)
{
	*out_point = *point;
	out_point->x *= (vector->i * scale);
	out_point->y *= (vector->j * scale);

	return out_point;
};

real_vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, real_vector2d* out_vector)
{
	out_vector->i = point1->x - point0->x;
	out_vector->j = point1->y - point0->y;

	return out_vector;
};