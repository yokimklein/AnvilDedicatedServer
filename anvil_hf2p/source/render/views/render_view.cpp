#include "render_view.h"
#include "cseries\cseries.h"
#include "rasterizer\rasterizer.h"
#include "render\render_debug.h"
#include <interface\terminal.h>
#include <main\console.h>
#include <main\main_time.h>

REFERENCE_DECLARE(0xEAC158, long, c_view::g_view_stack_top);
REFERENCE_DECLARE_ARRAY(0x233FD70, c_view*, c_view::g_view_stack, 4);

REFERENCE_DECLARE(0x233FD80, c_player_view*, c_player_view::x_current_player_view);


// $TODO:
bool render_resources_disabled()
{
	return false;
}

void render_debug_window_render(long user_index)
{
	// $TODO: profiler
	//c_rasterizer_profile_scope _window_debug(_rasterizer_profile_element_debug, L"window_debug");

	c_rasterizer::restore_last_viewport();
	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
	if (c_view::top())
	{
		render_debug_begin(false, false, false);
		render_debug_structure_draw();
		render_debug_clients(user_index);
		render_debug_end(true, false, false);
	}
}

void __fastcall render_debug_frame_render()
{
	if (render_resources_disabled())
	{
		return;
	}

#if defined(DEBUG_ENABLED)
	rectangle2d screen_pixel_bounds{};
	c_rasterizer::get_fullscreen_render_pixel_bounds(&screen_pixel_bounds);

	rectangle2d safe_frame_pixel_bounds{};
	c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&safe_frame_pixel_bounds);

	//c_rasterizer_profile_scope _frame_debug(_rasterizer_profile_element_debug, L"frame_debug");
	render_debug_begin(true, true, true);

	terminal_draw(&screen_pixel_bounds, &safe_frame_pixel_bounds);
	status_line_draw();

	//cinematic_status_draw();
	//render_debug_campaign_metagame();
#endif

	main_time_frame_rate_display();

#if defined(DEBUG_ENABLED)
	//render_debug_scripting();
	//render_debug_cluster_blend_info();
	//profile_render(&screen_pixel_bounds, &safe_frame_pixel_bounds);
	//render_synchronization_stats();
	//player_control_debug_render();
	//weapons_debug_render();
	//render_debug_debug_menu();
	//game_time_render_debug();
	//overlapped_render();
	//controllers_render();
	//font_cache_debug_render();
	//async_display_current_task_status();

	render_debug_end(false, false, false);
#endif
}

c_view* c_view::top()
{
	if (g_view_stack_top < 0)
	{
		return NULL;
	}
	else
	{
		return g_view_stack[g_view_stack_top];
	}
}

const render_camera* c_view::get_render_camera() const
{
	return &m_render_camera;
}

render_camera* c_view::get_render_camera_modifiable()
{
	return &m_render_camera;
}

const render_camera* c_view::get_rasterizer_camera() const
{
	return &m_rasterizer_camera;
}

render_camera* c_view::get_rasterizer_camera_modifiable()
{
	return &m_rasterizer_camera;
}

const render_projection* c_view::get_rasterizer_projection() const
{
	return &m_rasterizer_projection;
}

render_projection* c_view::get_rasterizer_projection_modifiable()
{
	return &m_rasterizer_projection;
}

const render_projection* c_view::get_render_projection() const
{
	return &m_render_projection;
}

render_projection* c_view::get_render_projection_modifiable()
{
	return &m_render_projection;
}
