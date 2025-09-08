#pragma once
#include "math\real_math.h"
#include "math\integer_math.h"

struct render_camera
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	bool mirrored;
	real vertical_field_of_view;
	real field_of_view_scale;
	rectangle2d window_pixel_bounds;
	rectangle2d window_title_safe_pixel_bounds;
	point2d window_final_location;
	rectangle2d render_pixel_bounds;
	rectangle2d render_title_safe_pixel_bounds;
	rectangle2d display_pixel_bounds;
	real z_near;
	real z_far;
	real_plane3d mirror_plane;
	bool enlarge_view;
	real_point2d enlarge_center;
	real enlarge_size_x;
	real enlarge_size_y;
};
static_assert(sizeof(render_camera) == 0x88);

struct s_oriented_bounding_box
{
	real_matrix4x3 m_transform;
	real_vector3d m_extents;
};
static_assert(sizeof(s_oriented_bounding_box) == 0x40);

struct render_projection
{
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;
	real_rectangle2d projection_bounds;
	real projection_matrix[4][4];
	real_vector2d world_to_screen_size;
};
static_assert(sizeof(render_projection) == 0xC0);

extern bool __fastcall render_camera_view_to_screen(const render_camera* camera, const render_projection* projection, const rectangle2d* window_display_bounds, const real_point3d* view_point, real_point2d* screen_point);
extern bool render_camera_world_to_window(const render_camera* camera, const render_projection* projection, const rectangle2d* window_display_bounds, const real_point3d* world_point, real_point2d* screen_point);