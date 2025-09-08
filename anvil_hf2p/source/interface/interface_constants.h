#pragma once
#include <cseries\cseries.h>

extern void interface_get_current_display_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds);
extern real_vector2d interface_get_aspect_ratio_scaling();
extern void interface_scale_rectangle2d_for_xenon_scaler(rectangle2d* rectangle, const real_vector2d* aspect_ratio_scale);
extern void __fastcall calculate_aspect_ratio_scaling();

extern real_vector2d& g_aspect_ratio_scale;