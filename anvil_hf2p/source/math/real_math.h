#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/real_math.hpp

#define PI 3.14159265359f
#define TWO_PI real(PI * 2)
#define RAD real(180 / PI)
#define DEG real(PI / 180)

typedef float real;
static_assert(sizeof(real) == 0x4);

typedef real real_fraction;
static_assert(sizeof(real_fraction) == 0x4);

#define DEFINE_REAL_CONSTANT(NAME) real const k##NAME = NAME

#define _pi PI
#define _test_real_epsilon  0.001f
#define _real_epsilon       0.0001f
#define _real_tiny_epsilon  0.000001f
#define _real_max           3.4028235e38f
#define _real_min          -3.4028235e38f

DEFINE_REAL_CONSTANT(_test_real_epsilon);
DEFINE_REAL_CONSTANT(_real_epsilon);
DEFINE_REAL_CONSTANT(_real_tiny_epsilon);
DEFINE_REAL_CONSTANT(_real_max);
DEFINE_REAL_CONSTANT(_real_min);

enum
{
	_x = 0,
	_y,
	_z,
	_w,

	k_2d_count = 2,
	k_3d_count = 3,
	k_4d_count = 4,

	k_faces_per_cube_count = 6,
	k_vertices_per_cube_count = 8,
	k_edges_per_cube_count = 12,
};

union real_bounds
{
	struct
	{
		real lower;
		real upper;
	};
	real n[2];
};
static_assert(sizeof(real_bounds) == 0x8);

union real_point2d
{
	struct
	{
		real x;
		real y;
	};
	real n[2];
};
static_assert(sizeof(real_point2d) == 0x8);

union real_point3d
{
	struct
	{
		real x;
		real y;
		real z;
	};
	real n[3];
};
static_assert(sizeof(real_point3d) == 0xC);

union real_vector2d
{
	struct
	{
		real i;
		real j;
	};
	real n[2];
};
static_assert(sizeof(real_vector2d) == 0x8);

union real_vector3d
{
	real_vector3d() {};
	real_vector3d(real i, real j, real k)
	{
		this->i = i;
		this->j = j;
		this->k = k;
	}

	struct
	{
		real i;
		real j;
		real k;
	};
	real n[3];

	real_vector3d operator-(const real_vector3d& other) const
	{
		return real_vector3d(this->i - other.i, this->j - other.j, this->k - other.k);
	}
};
static_assert(sizeof(real_vector3d) == 0xC);

union real_quaternion
{
	struct
	{
		real i;
		real j;
		real k;
		real w;
	};
	real n[4];
};
static_assert(sizeof(real_quaternion) == 0x10);

union real_plane2d
{
	struct
	{
		real_vector2d normal;
		real distance;
	};
	real n[3];
};
static_assert(sizeof(real_plane2d) == 0xC);

struct real_plane3d
{
	real_vector3d n;
	real d;
};
static_assert(sizeof(real_plane3d) == 0x10);

union real_rgb_color
{
	struct
	{
		real red;
		real green;
		real blue;
	};
	real n[3];
};
static_assert(sizeof(real_rgb_color) == 0xC);

union real_argb_color
{
	real n[4];

	struct
	{
		real alpha;
		real_rgb_color rgb;
	};

	struct
	{
		byte gap0[sizeof(real)];
		real red;
		real green;
		real blue;
	};
};
static_assert(sizeof(real_argb_color) == sizeof(real) * 4);

union real_hsv_color
{
	struct
	{
		real hue;
		real saturation;
		real value;
	};
	real n[3];
};
static_assert(sizeof(real_hsv_color) == 0xC);

union real_ahsv_color
{
	struct
	{
		real alpha;
		real hue;
		real saturation;
		real value;
	};
	real n[4];
};
static_assert(sizeof(real_ahsv_color) == 0x10);

typedef real angle;
static_assert(sizeof(angle) == 0x4);

union angle_bounds
{
	struct
	{
		angle lower;
		angle upper;
	};
	angle n[2];
};
static_assert(sizeof(angle_bounds) == 0x8);

union real_euler_angles2d
{
	struct
	{
		angle yaw;
		angle pitch;
	};
	angle n[2];
};
static_assert(sizeof(real_euler_angles2d) == 0x8);

union real_euler_angles3d
{
	struct
	{
		angle yaw;
		angle pitch;
		angle roll;
	};
	angle n[3];
};
static_assert(sizeof(real_euler_angles3d) == 0xC);

union real_matrix3x3
{
	struct
	{
		real_vector3d forward;
		real_vector3d left;
		real_vector3d up;
	};
	real n[9];
};
static_assert(sizeof(real_matrix3x3) == 0x24);

struct real_matrix4x3
{
	real_matrix4x3() {};

	real scale;

	union
	{
		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
			real_point3d position;
		};

		real n[k_3d_count][k_4d_count];
	};
};
static_assert(sizeof(real_matrix4x3) == 0x34);

struct real_rectangle3d
{
	real x0;
	real x1;
	real y0;
	real y1;
	real z0;
	real z1;
};
static_assert(sizeof(real_rectangle3d) == 0x18);

// These are supposed to be pointers, but the pointers were compiled out in MS29
extern real_vector3d const& global_up3d;
extern real_vector3d const& global_down3d;
extern real_point3d const& global_origin3d;

extern real_vector3d* __cdecl cross_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* out);
extern real __cdecl dot_product3d(real_vector3d const* a, real_vector3d const* b);
extern real_vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, real_vector3d* out);
extern real_point3d* __cdecl point_from_line3d(real_point3d const* in_point, real_vector3d const* in_vector, real scale, real_point3d* out_point);
extern real __cdecl magnitude_squared3d(real_vector3d const* a);
extern real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl distance3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl square_root(real value);
extern real __cdecl magnitude3d(real_vector3d const* vector);
extern real_vector3d* __cdecl perpendicular3d(real_vector3d const* vector, real_vector3d* out_vector);
extern real __cdecl normalize3d(real_vector3d* vector);
extern real_vector2d* __cdecl rotate_vector2d(real_vector2d const* vector, real a2, real a3, real_vector2d* out_vector);
extern real_vector3d* __cdecl scale_vector3d(real_vector3d const* in_vector, real scale, real_vector3d* out_vector);
extern real_vector3d* __cdecl add_vectors3d(real_vector3d const* in_vector_a, real_vector3d const* in_vector_b, real_vector3d* out_vector);
extern real_vector3d* __cdecl negate_vector3d(real_vector3d const* in_vector, real_vector3d* out_vector);
extern real_point2d* __cdecl set_real_point2d(real_point2d* point, real x, real y);
extern real_point3d* __cdecl set_real_point3d(real_point3d* point, real x, real y, real z);
extern real_point2d* __cdecl point_from_line2d(real_point2d const* point, real_vector2d const* vector, real scale, real_point2d* out_point);
extern real_vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, real_vector2d* out_vector);