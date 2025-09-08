#include "render\views\render_view.h"

void c_player_view::get_player_render_camera_orientation(real_matrix4x3* camera)
{
	ASSERT(camera);
	ASSERT(c_player_view::x_current_player_view, "cannot get the render camera when not in the player view render loop");

	const render_camera* rasterizer_camera = c_player_view::x_current_player_view->get_rasterizer_camera();

	camera->position = rasterizer_camera->position;
	camera->vectors.forward = rasterizer_camera->forward;
	camera->vectors.up = rasterizer_camera->up;
	cross_product3d(&camera->vectors.up, &camera->vectors.forward, &camera->vectors.left);
	camera->scale = 1.0f;
}