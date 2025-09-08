#include "interface_constants.h"
#include "rasterizer\rasterizer.h"

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
