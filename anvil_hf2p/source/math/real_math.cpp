#include "real_math.h"
#include <math.h>
#include <cseries\cseries.h>

const real_vector3d private_zero_vector[2] =
{
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }
};

const real_orientation private_identity_orientation =
{
	{ real_vector3d{ 0.0f, 0.0f, 0.0f }, 1.0f },
	{ 0.0f, 0.0f, 0.0f },
	1.0f
};

const real_matrix4x3 private_identity4x3 =
{
	1.0f,
	real_matrix3x3({ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }),
	{ 0.0f, 0.0f, 0.0f }
};

const real_matrix4x3 private_negative_identity4x3 =
{
	1.0f,
	real_matrix3x3({ -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }),
	{ 0.0f, 0.0f, 0.0f }
};

const real_rectangle3d private_null_rectangle =
{
	3.4028235e38f,
	-3.4028235e38f,
	3.4028235e38f,
	-3.4028235e38f,
	3.4028235e38f,
	-3.4028235e38f
};

const real_point2d* const global_origin2d = (const real_point2d* const)&private_zero_vector;

const real_point2d* const global_x_axis2d = (const real_point2d* const)&private_identity4x3.vectors.forward;
const real_point2d* const global_y_axis2d = (const real_point2d* const)&private_identity4x3.vectors.left;

const real_point2d* const global_negative_x_axis2d = (const real_point2d* const)&private_negative_identity4x3.vectors.forward;
const real_point2d* const global_negative_y_axis2d = (const real_point2d* const)&private_negative_identity4x3.vectors.left;

const real_vector2d* const global_zero_vector2d = (const real_vector2d* const)&private_zero_vector;
const real_vector2d* const global_forward2d = (const real_vector2d* const)&private_identity4x3.vectors.forward;
const real_vector2d* const global_left2d = (const real_vector2d* const)&private_identity4x3.vectors.left;

const real_vector2d* const global_backward2d = (const real_vector2d* const)&private_negative_identity4x3.vectors.forward;
const real_vector2d* const global_right2d = (const real_vector2d* const)&private_negative_identity4x3.vectors.left;

const real_point3d* const global_origin3d = (const real_point3d* const)&private_zero_vector;

const real_point3d* const global_x_axis3d = (const real_point3d* const)&private_identity4x3.vectors.forward;
const real_point3d* const global_y_axis3d = (const real_point3d* const)&private_identity4x3.vectors.left;
const real_point3d* const global_z_axis3d = (const real_point3d* const)&private_identity4x3.vectors.up;
const real_point3d* const global_negative_x_axis3d = (const real_point3d* const)&private_negative_identity4x3.vectors.forward;
const real_point3d* const global_negative_y_axis3d = (const real_point3d* const)&private_negative_identity4x3.vectors.left;
const real_point3d* const global_negative_z_axis3d = (const real_point3d* const)&private_negative_identity4x3.vectors.up;

const real_vector3d* const global_zero_vector3d = (const real_vector3d* const)&private_zero_vector;

const real_vector3d* const global_forward3d = (const real_vector3d* const)&private_identity4x3.vectors.forward;
const real_vector3d* const global_left3d = (const real_vector3d* const)&private_identity4x3.vectors.left;
const real_vector3d* const global_up3d = (const real_vector3d* const)&private_identity4x3.vectors.up;

const real_vector3d* const global_backward3d = (const real_vector3d* const)&private_negative_identity4x3.vectors.forward;
const real_vector3d* const global_right3d = (const real_vector3d* const)&private_negative_identity4x3.vectors.left;
const real_vector3d* const global_down3d = (const real_vector3d* const)&private_negative_identity4x3.vectors.up;

const real_vector4d* const global_zero_vector4d = (const real_vector4d* const)&private_zero_vector;
const real_euler_angles2d* const global_zero_angles2d = (const real_euler_angles2d* const)&private_zero_vector;
const real_euler_angles3d* const global_zero_angles3d = (const real_euler_angles3d* const)&private_zero_vector;
const real_quaternion* const global_identity_quaternion = (const real_quaternion* const)&private_identity_orientation.rotation;

const real_matrix4x3* const global_identity4x3 = &private_identity4x3;
const real_matrix4x3* const global_negative_identity4x3 = &private_negative_identity4x3;

const real_orientation* const global_identity_orientation = &private_identity_orientation;

const real_rectangle2d* const global_null_rectangle2d = (const real_rectangle2d* const)&private_null_rectangle;
const real_rectangle3d* const global_null_rectangle3d = &private_null_rectangle;

short const global_projection3d_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _y, _z, _x }
	},
	{
		{ _x, _z, _y },
		{ _z, _x, _y }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

short const global_projection3d_inverse_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _z, _x, _y }
	},
	{
		{ _x, _z, _y },
		{ _y, _z, _x }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

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

real __cdecl interpolate_linear(real start_value, real end_value, real interpolation_factor)
{
	return start_value + ((end_value - start_value) * interpolation_factor);
}

real_point3d* __cdecl project_point2d(const real_point2d* p2d, const real_plane3d* plane, short projection, bool sign, real_point3d* p3d)
{
	short v5 = global_projection3d_mappings[projection][sign][_x];
	short v6 = global_projection3d_mappings[projection][sign][_y];

	real v7 = 0.0f;
	if (fabsf((plane->n.n[projection] - 0.0f)) >= k_real_epsilon)
	{
		v7 = ((plane->d - (plane->n.n[v5] * p2d->n[0])) - (plane->n.n[v6] * p2d->n[1])) / plane->n.n[projection];
	}

	ASSERT(projection >= _x && projection <= _z);
	p3d->n[v5] = p2d->n[0];
	p3d->n[v6] = p2d->n[1];
	p3d->n[projection] = v7;

	return p3d;
}

real_vector3d* __fastcall generate_up_vector3d(const real_vector3d* forward, real_vector3d* up)
{
	return INVOKE(0x84960, generate_up_vector3d, forward, up);
}

real dot_product4d_quaternion(const real_quaternion* a, const real_quaternion* b)
{
	return a->v.i * b->v.i + a->v.j * b->v.j + a->v.k * b->v.k + a->w * b->w;
}

long rectangle3d_build_edges(const real_rectangle3d* bounds, long maximum_edge_count, real_point3d(* const edges)[2])
{
	ASSERT(bounds);
	ASSERT(maximum_edge_count >= k_edges_per_cube_count);
	ASSERT(edges);

	long line_vertex_indices[k_edges_per_cube_count][2]
	{
		{ 0, 2 },
		{ 2, 3 },
		{ 3, 1 },
		{ 1, 0 },
		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },
		{ 4, 5 },
		{ 5, 7 },
		{ 7, 6 },
		{ 6, 4 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long edge_index = 0; edge_index < k_edges_per_cube_count; edge_index++)
	{
		ASSERT((line_vertex_indices[edge_index][0] >= 0) && (line_vertex_indices[edge_index][0] < k_vertices_per_cube_count));
		ASSERT((line_vertex_indices[edge_index][1] >= 0) && (line_vertex_indices[edge_index][1] < k_vertices_per_cube_count));

		edges[edge_index][0] = vertices[line_vertex_indices[edge_index][0]];
		edges[edge_index][1] = vertices[line_vertex_indices[edge_index][1]];
	}

	return k_edges_per_cube_count;
}

long rectangle3d_build_faces(const real_rectangle3d* bounds, long maximum_face_count, real_point3d(* const faces)[4])
{
	ASSERT(bounds);
	ASSERT(maximum_face_count >= k_faces_per_cube_count);
	ASSERT(faces);

	long face_vertex_indices[k_faces_per_cube_count][4]
	{
		{ 0, 2, 3, 1 },
		{ 0, 1, 5, 4 },
		{ 1, 3, 7, 5 },
		{ 2, 6, 7, 3 },
		{ 0, 4, 6, 2 },
		{ 4, 5, 7, 6 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long face_index = 0; face_index < k_faces_per_cube_count; face_index++)
	{
		for (long vertex_index = 0; vertex_index < k_vertices_per_cube_count; vertex_index++)
		{
			ASSERT((face_vertex_indices[face_index][vertex_index] >= 0) && (face_vertex_indices[face_index][vertex_index] < k_vertices_per_cube_count));

			*faces[face_vertex_indices[face_index][vertex_index]] = vertices[face_vertex_indices[face_index][vertex_index]];
		}
	}

	return k_faces_per_cube_count;
}

long rectangle3d_build_vertices(const real_rectangle3d* bounds, long maximum_vertex_count, real_point3d* const vertices)
{
	ASSERT(bounds);
	ASSERT(maximum_vertex_count >= k_vertices_per_cube_count);
	ASSERT(vertices);

	vertices[0].x = bounds->x0;
	vertices[0].y = bounds->y0;
	vertices[0].z = bounds->z0;
	vertices[1].x = bounds->x1;
	vertices[1].y = bounds->y0;
	vertices[1].z = bounds->z0;
	vertices[2].x = bounds->x0;
	vertices[2].y = bounds->y1;
	vertices[2].z = bounds->z0;
	vertices[3].x = bounds->x1;
	vertices[3].y = bounds->y1;
	vertices[3].z = bounds->z0;
	vertices[4].x = bounds->x0;
	vertices[4].y = bounds->y0;
	vertices[4].z = bounds->z1;
	vertices[5].x = bounds->x1;
	vertices[5].y = bounds->y0;
	vertices[5].z = bounds->z1;
	vertices[6].x = bounds->x0;
	vertices[6].y = bounds->y1;
	vertices[6].z = bounds->z1;
	vertices[7].x = bounds->x1;
	vertices[7].y = bounds->y1;
	vertices[7].z = bounds->z1;

	return k_vertices_per_cube_count;
}