#include "interface_constants.h"
#include "rasterizer\rasterizer.h"
#include "render\views\render_view.h"
#include "interface\user_interface_controller.h"

REFERENCE_DECLARE(0xEB0CAC, real_vector2d, g_aspect_ratio_scale);

void interface_get_current_display_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds)
{
    if (out_render_bounds)
    {
        c_rasterizer::get_fullscreen_render_pixel_bounds((rectangle2d*)out_render_bounds);
    }

    if (out_render_title_safe_bounds)
    {
        c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(out_render_title_safe_bounds);
    }

    if (out_display_bounds)
    {
        c_rasterizer::get_display_pixel_bounds(out_display_bounds);
    }

    if (out_display_title_safe_bounds)
    {
        c_rasterizer::get_display_title_safe_pixel_bounds(out_display_title_safe_bounds);
    }
}

real_vector2d interface_get_aspect_ratio_scaling()
{
    calculate_aspect_ratio_scaling();
    return g_aspect_ratio_scale;
}

void interface_scale_rectangle2d_for_xenon_scaler(rectangle2d* rectangle, const real_vector2d* aspect_ratio_scale)
{
    rectangle->y0 = (short)((real)rectangle->y0 * aspect_ratio_scale->j);
    rectangle->x0 = (short)((real)rectangle->x0 * aspect_ratio_scale->i);
    rectangle->y1 = (short)((real)rectangle->y1 * aspect_ratio_scale->j);
    rectangle->x1 = (short)((real)rectangle->x1 * aspect_ratio_scale->i);
}

void __fastcall calculate_aspect_ratio_scaling()
{
    INVOKE(0x3C5E80, calculate_aspect_ratio_scaling);
}

void interface_get_current_display_or_window_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds)
{
    if (c_player_view::x_current_player_view)
    {
        interface_get_current_window_settings(out_render_bounds, out_render_title_safe_bounds, out_display_bounds, out_display_title_safe_bounds);
    }
    else
    {
        interface_get_current_display_settings(out_render_bounds, out_render_title_safe_bounds, out_display_bounds, out_display_title_safe_bounds);
    }
}

void interface_get_current_window_settings(rectangle2d* out_render_window_bounds, rectangle2d* out_render_window_title_safe_bounds, rectangle2d* out_display_window_bounds, rectangle2d* out_display_window_title_safe_bounds)
{
    const c_player_view* top = c_player_view::x_current_player_view;
    ASSERT(top);
    const render_camera* rasterizer_camera = top->get_rasterizer_camera();
    if (out_render_window_bounds)
    {
        *out_render_window_bounds = rasterizer_camera->window_pixel_bounds;
    }
    if (out_render_window_title_safe_bounds)
    {
        *out_render_window_title_safe_bounds = rasterizer_camera->window_title_safe_pixel_bounds;
    }
    if (out_display_window_bounds || out_display_window_title_safe_bounds)
    {
        calculate_aspect_ratio_scaling();
        real_vector2d aspect_ratio_scale = { 1.0f / g_aspect_ratio_scale.i, 1.0f / g_aspect_ratio_scale.j };
        if (out_display_window_bounds)
        {
            *out_display_window_bounds = rasterizer_camera->window_pixel_bounds;
            interface_scale_rectangle2d_for_xenon_scaler(out_display_window_bounds, &aspect_ratio_scale);
        }
        if (out_display_window_title_safe_bounds)
        {
            *out_display_window_title_safe_bounds = rasterizer_camera->window_title_safe_pixel_bounds;
            interface_scale_rectangle2d_for_xenon_scaler(out_display_window_title_safe_bounds, &aspect_ratio_scale);
        }
    }
}

bool interface_is_split_screen()
{
    return user_interface_controller_get_signed_in_controller_count() > 1;
}
