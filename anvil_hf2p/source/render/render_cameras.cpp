#include "render_cameras.h"
#include "cseries\cseries.h"
#include "math\matrix_math.h"

bool __fastcall render_camera_view_to_screen(const render_camera* camera, const render_projection* projection, const rectangle2d* window_display_bounds, const real_point3d* view_point, real_point2d* screen_point)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(view_point);
	ASSERT(screen_point);

	return INVOKE(0x2948D0, render_camera_view_to_screen, camera, projection, window_display_bounds, view_point, screen_point);
}

bool render_camera_world_to_window(const render_camera* camera, const render_projection* projection, const rectangle2d* window_display_bounds, const real_point3d* world_point, real_point2d* screen_point)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(window_display_bounds);
	ASSERT(world_point);
	ASSERT(screen_point);

	real_point3d point{};
	matrix4x3_transform_point(&projection->world_to_view, world_point, &point);
	return render_camera_view_to_screen(camera, projection, window_display_bounds, &point, screen_point);
}