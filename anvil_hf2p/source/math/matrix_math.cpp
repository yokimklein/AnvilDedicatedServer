#include "matrix_math.h"
#include "cseries\cseries.h"
#include <math.h>

void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, real_vector3d const* forward, real_vector3d const* up)
{
	matrix->scale = 1.0f;
	matrix->vectors.forward = *forward;
	cross_product3d(up, forward, &matrix->vectors.left);
	matrix->vectors.up = *up;
	set_real_point3d(&matrix->position, 0.0f, 0.0f, 0.0f);
}

void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, real_vector3d const* forward, real_vector3d const* up)
{
	ASSERT(point);

	matrix4x3_rotation_from_vectors(matrix, forward, up);
	matrix->position = *point;
}

real_vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, real_vector3d const* in_vector, real_vector3d* out_vector)
{
	real forward = in_vector->n[0];
	real left = in_vector->n[1];
	real up = in_vector->n[2];

	if (matrix->scale != 1.0)
	{
		forward *= matrix->scale;
		left *= matrix->scale;
		up *= matrix->scale;
	}

	out_vector->n[0] = ((forward * matrix->vectors.forward.n[0]) + (left * matrix->vectors.left.n[0])) + (up * matrix->vectors.up.n[0]);
	out_vector->n[1] = ((forward * matrix->vectors.forward.n[1]) + (left * matrix->vectors.left.n[1])) + (up * matrix->vectors.up.n[1]);
	out_vector->n[2] = ((forward * matrix->vectors.forward.n[2]) + (left * matrix->vectors.left.n[2])) + (up * matrix->vectors.up.n[2]);

	return out_vector;
}

real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* in_point, real_point3d* out_point)
{
	real forward = in_point->n[0] * matrix->scale;
	real left = in_point->n[1] * matrix->scale;
	real up = in_point->n[2] * matrix->scale;

	out_point->n[0] = (((matrix->vectors.left.n[0] * left) + (matrix->vectors.forward.n[0] * forward)) + (matrix->vectors.up.n[0] * up)) + matrix->position.n[0];
	out_point->n[1] = (((matrix->vectors.left.n[1] * left) + (matrix->vectors.forward.n[1] * forward)) + (matrix->vectors.up.n[1] * up)) + matrix->position.n[1];
	out_point->n[2] = (((matrix->vectors.left.n[2] * left) + (matrix->vectors.forward.n[2] * forward)) + (matrix->vectors.up.n[2] * up)) + matrix->position.n[2];

	return out_point;
}

void __cdecl matrix4x3_transform_points(real_matrix4x3 const* matrix, long total_point_count, real_point3d const* const points, real_point3d* const out_points)
{
	for (long i = 0; i < total_point_count; ++i)
		matrix4x3_transform_point(matrix, &points[i], &out_points[i]);
}

void __cdecl matrix4x3_multiply(real_matrix4x3 const* in_matrix0, real_matrix4x3 const* in_matrix1, real_matrix4x3* out_matrix)
{
	if (in_matrix0 == out_matrix)
		csmemcpy(out_matrix, in_matrix0, sizeof(real_matrix4x3));

	if (in_matrix1 == out_matrix)
		csmemcpy(out_matrix, in_matrix1, sizeof(real_matrix4x3));

	out_matrix->vectors.forward.n[0] = ((in_matrix0->vectors.forward.n[0] * in_matrix1->vectors.forward.n[0]) + (in_matrix0->vectors.left.n[0] * in_matrix1->vectors.forward.n[1])) + (in_matrix0->vectors.up.n[0] * in_matrix1->vectors.forward.n[2]);
	out_matrix->vectors.forward.n[1] = ((in_matrix0->vectors.forward.n[1] * in_matrix1->vectors.forward.n[0]) + (in_matrix0->vectors.left.n[1] * in_matrix1->vectors.forward.n[1])) + (in_matrix0->vectors.up.n[1] * in_matrix1->vectors.forward.n[2]);
	out_matrix->vectors.forward.n[2] = ((in_matrix0->vectors.forward.n[2] * in_matrix1->vectors.forward.n[0]) + (in_matrix0->vectors.left.n[2] * in_matrix1->vectors.forward.n[1])) + (in_matrix0->vectors.up.n[2] * in_matrix1->vectors.forward.n[2]);

	out_matrix->vectors.left.n[0] = ((in_matrix0->vectors.forward.n[0] * in_matrix1->vectors.left.n[0]) + (in_matrix0->vectors.left.n[0] * in_matrix1->vectors.left.n[1])) + (in_matrix0->vectors.up.n[0] * in_matrix1->vectors.left.n[2]);
	out_matrix->vectors.left.n[1] = ((in_matrix0->vectors.forward.n[1] * in_matrix1->vectors.left.n[0]) + (in_matrix0->vectors.left.n[1] * in_matrix1->vectors.left.n[1])) + (in_matrix0->vectors.up.n[1] * in_matrix1->vectors.left.n[2]);
	out_matrix->vectors.left.n[2] = ((in_matrix0->vectors.forward.n[2] * in_matrix1->vectors.left.n[0]) + (in_matrix0->vectors.left.n[2] * in_matrix1->vectors.left.n[1])) + (in_matrix0->vectors.up.n[2] * in_matrix1->vectors.left.n[2]);

	out_matrix->vectors.up.n[0] = ((in_matrix0->vectors.forward.n[0] * in_matrix1->vectors.up.n[0]) + (in_matrix0->vectors.left.n[0] * in_matrix1->vectors.up.n[1])) + (in_matrix0->vectors.up.n[0] * in_matrix1->vectors.up.n[2]);
	out_matrix->vectors.up.n[1] = ((in_matrix0->vectors.forward.n[1] * in_matrix1->vectors.up.n[0]) + (in_matrix0->vectors.left.n[1] * in_matrix1->vectors.up.n[1])) + (in_matrix0->vectors.up.n[1] * in_matrix1->vectors.up.n[2]);
	out_matrix->vectors.up.n[2] = ((in_matrix0->vectors.forward.n[2] * in_matrix1->vectors.up.n[0]) + (in_matrix0->vectors.left.n[2] * in_matrix1->vectors.up.n[1])) + (in_matrix0->vectors.up.n[2] * in_matrix1->vectors.up.n[2]);

	out_matrix->position.n[0] = in_matrix0->position.n[0] + (in_matrix0->scale * (((in_matrix0->vectors.forward.n[0] * in_matrix1->position.n[0]) + (in_matrix0->vectors.left.n[0] * in_matrix1->position.n[1])) + (in_matrix0->vectors.up.n[0] * in_matrix1->position.n[2])));
	out_matrix->position.n[1] = in_matrix0->position.n[1] + (in_matrix0->scale * (((in_matrix0->vectors.forward.n[1] * in_matrix1->position.n[0]) + (in_matrix0->vectors.left.n[1] * in_matrix1->position.n[1])) + (in_matrix0->vectors.up.n[1] * in_matrix1->position.n[2])));
	out_matrix->position.n[2] = in_matrix0->position.n[2] + (in_matrix0->scale * (((in_matrix0->vectors.forward.n[2] * in_matrix1->position.n[0]) + (in_matrix0->vectors.left.n[2] * in_matrix1->position.n[1])) + (in_matrix0->vectors.up.n[2] * in_matrix1->position.n[2])));

	out_matrix->scale = in_matrix0->scale * in_matrix1->scale;
}

void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* out_matrix)
{
	real negative_x = -matrix->position.x;
	real negative_y = -matrix->position.y;
	real negative_z = -matrix->position.z;

	if (matrix->scale == 1.0f)
	{
		out_matrix->scale = 1.0f;
	}
	else
	{
		out_matrix->scale = 1.0f / (matrix->scale < 0.0f ? fmaxf(matrix->scale, -_real_epsilon) : fmaxf(matrix->scale, _real_epsilon));

		negative_x *= out_matrix->scale;
		negative_y *= out_matrix->scale;
		negative_z *= out_matrix->scale;
	}

	out_matrix->vectors.forward.i = matrix->vectors.forward.i;
	out_matrix->vectors.left.j = matrix->vectors.left.j;
	out_matrix->vectors.up.k = matrix->vectors.up.k;

	out_matrix->vectors.left.i = matrix->vectors.forward.j;
	out_matrix->vectors.forward.j = matrix->vectors.left.i;
	out_matrix->vectors.up.i = matrix->vectors.forward.k;

	out_matrix->vectors.forward.k = matrix->vectors.up.i;
	out_matrix->vectors.up.j = matrix->vectors.left.k;
	out_matrix->vectors.left.k = matrix->vectors.up.j;

	out_matrix->position.x = ((negative_x * out_matrix->vectors.forward.i) + (negative_y * out_matrix->vectors.left.i)) + (negative_z * out_matrix->vectors.up.i);
	out_matrix->position.y = ((negative_x * out_matrix->vectors.forward.j) + (negative_y * out_matrix->vectors.left.j)) + (negative_z * out_matrix->vectors.up.j);
	out_matrix->position.z = ((negative_x * out_matrix->vectors.forward.k) + (negative_y * out_matrix->vectors.left.k)) + (negative_z * out_matrix->vectors.up.k);
}

real_vector3d* __cdecl matrix4x3_transform_normal(real_matrix4x3 const* matrix, real_vector3d const* vector, real_vector3d* out_vector)
{
	out_vector->i = ((vector->i * matrix->vectors.forward.i) + (vector->j * matrix->vectors.left.i)) + (vector->k * matrix->vectors.up.i);
	out_vector->j = ((vector->i * matrix->vectors.forward.j) + (vector->j * matrix->vectors.left.j)) + (vector->k * matrix->vectors.up.j);
	out_vector->k = ((vector->i * matrix->vectors.forward.k) + (vector->j * matrix->vectors.left.k)) + (vector->k * matrix->vectors.up.k);

	return out_vector;
}

real_plane3d* __cdecl matrix4x3_transform_plane(real_matrix4x3 const* matrix, real_plane3d const* plane, real_plane3d* out_plane)
{
	matrix4x3_transform_normal(matrix, &plane->n, &out_plane->n);
	out_plane->d = (matrix->scale * plane->d) + dot_product3d((real_vector3d*)&matrix->position, &out_plane->n);

	return out_plane;
}

real_matrix3x3* matrix3x3_rotation_from_quaternion(real_matrix3x3* matrix, const real_quaternion* quaternion)
{
	const real dot_product = dot_product4d_quaternion(quaternion, quaternion);

	const real scalar = (dot_product > k_real_epsilon ? 2.0f / dot_product : 0.0f);

	real_vector3d scaled_vector;
	scale_vector3d(&quaternion->v, scalar, &scaled_vector);

	real_vector3d w_scaled_vector;
	scale_vector3d(&scaled_vector, quaternion->w, &w_scaled_vector);

	real_vector3d i_scaled_vector;
	scale_vector3d(&scaled_vector, quaternion->i, &i_scaled_vector);

	real j_scaled_vector_j = quaternion->j * scaled_vector.j;
	real j_scaled_vector_k = quaternion->j * scaled_vector.k;
	real k_scaled_vector_k = quaternion->k * scaled_vector.k;

	matrix->forward.i = 1.0f - (k_scaled_vector_k + j_scaled_vector_j);
	matrix->left.i = i_scaled_vector.j - w_scaled_vector.k;
	matrix->up.i = i_scaled_vector.k + w_scaled_vector.j;
	matrix->forward.j = i_scaled_vector.j + w_scaled_vector.k;
	matrix->left.j = 1.0f - (k_scaled_vector_k + i_scaled_vector.i);
	matrix->up.j = j_scaled_vector_k - w_scaled_vector.i;
	matrix->forward.k = i_scaled_vector.k - w_scaled_vector.j;
	matrix->left.k = j_scaled_vector_k + w_scaled_vector.i;
	matrix->up.k = 1.0f - (j_scaled_vector_j + i_scaled_vector.i);
	return matrix;
}

void matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, const real_point3d* point, const real_quaternion* quaternion)
{
	matrix3x3_rotation_from_quaternion(&matrix->vectors, quaternion);
	matrix->scale = 1.0f;
	matrix->position = *point;
	return;
}
