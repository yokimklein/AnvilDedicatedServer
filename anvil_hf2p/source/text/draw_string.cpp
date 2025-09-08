#include "draw_string.h"
#include "math\color_math.h"
#include "text\font_loading.h"
#include "text\font_group.h"
#include "interface\interface_constants.h"
#include <string.h>

c_draw_string::c_draw_string() :
	__vftable(reinterpret_cast<decltype(__vftable)>(base_address(0xD80A58))),
	m_flags(),
	m_font_id(_terminal_font),
	m_styled_font_header(font_get_header(_terminal_font)),
	m_style(_text_style_plain),
	m_justification(_text_justification_left),
	m_drop_shadow_style(_shadow_drop),
	m_color(*global_real_argb_white),
	m_shadow_color(*global_real_argb_black),
	m_scale(1.0f),
	m_display_resolution_scale_adjustment(draw_string_get_glyph_scaling_for_display_settings()),
	m_height_adjust(0),
	m_tab_stop_count(0),
	m_tab_stops(),
	m_bounds(),
	m_text_bounds(),
	m_clip(),
	m_cursor(),
	m_permutation_proc(NULL),
	m_permutation_context(NULL),
	m_initial_indent(0),
	m_paragraph_indent(0),
	m_saved_parse_state(),
	m_saved_tab_stop_index(0),
	m_saved_line_count(0),
	m_saved_tab_stop_line_count(0),
	m_saved_maximum_tab_stop_line_count(0)
{
	//DECLFUNC(0x16C130, void, __thiscall, c_draw_string*)(this);
	rectangle2d display_bounds{};
	rectangle2d display_title_safe_pixel_bounds{};
	interface_get_current_display_settings(NULL, NULL, &display_bounds, &display_title_safe_pixel_bounds);
	c_draw_string::set_bounds(&display_bounds);
	csmemset(&m_saved_parse_state, 0, sizeof(m_saved_parse_state));
}

c_draw_string::~c_draw_string()
{
	//DECLFUNC(0x0, void, __thiscall, c_draw_string*)(this);
}

bool c_draw_string::draw_more(c_font_cache_base* font_cache, const char* string)
{
	return INVOKE_CLASS_MEMBER(0x16CBE0, c_draw_string, draw_more, font_cache, string);
}

short c_draw_string::get_line_height() const
{
	const s_font_header* header = font_get_header(m_font_id);
	real scale = m_display_resolution_scale_adjustment * m_scale;
	return (short)((real)font_get_line_height(header) * scale);
}

void c_draw_string::set_bounds(const real_rectangle2d* bounds, const real_rectangle2d* clip)
{
	set_bounds(bounds);
	if (clip)
	{
		if ((real)(clip->x1 - clip->x0) >= 0.0 && (real)(clip->y1 - clip->y0) >= 0.0)
		{
			m_clip = *clip;
		}
	}
	else
	{
		m_clip = m_bounds;
	}
}

void c_draw_string::set_bounds(const real_rectangle2d* bounds)
{
	if (bounds)
	{
		if ((bounds->x1 - bounds->x0) >= 0.0f && (bounds->y1 - bounds->y0) >= 0.0f)
		{
			m_bounds = *bounds;
		}
	}
	else
	{
		m_bounds = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
}

void c_draw_string::set_bounds(const rectangle2d* bounds)
{
	if (bounds)
	{
		if (bounds->x1 - bounds->x0 >= 0 && bounds->y1 - bounds->y0 >= 0)
		{
			m_bounds = { (real)bounds->x0, (real)bounds->x1, (real)bounds->y0, (real)bounds->y1 };
		}
	}
	else
	{
		m_bounds = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
}

void c_draw_string::set_color(ulong color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color({ .value = color }, &real_color));
}

void c_draw_string::set_color(pixel32 color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color(color, &real_color));
}

void c_draw_string::set_color(const real_argb_color* color)
{
	m_color = *color;
}

void c_draw_string::set_font(e_font_id font)
{
	if (font == _font_id_fallback)
	{
		m_font_id = _font_id_fallback;
		m_styled_font_header = font_get_header(_font_id_fallback);
	}
	else
	{
		if (font < _terminal_font)
		{
			font = _terminal_font;
		}
		if (font > _main_menu_font)
		{
			font = _main_menu_font;
		}
		m_font_id = font;
		m_styled_font_header = font_get_header(font);
	}
}

void c_draw_string::set_justification(e_text_justification justification)
{
	m_justification = justification;
}

void c_draw_string::set_shadow_color(pixel32 color)
{
	real_argb_color real_color{};
	set_shadow_color(pixel32_to_real_argb_color(color, &real_color));
}

void c_draw_string::set_shadow_color(const real_argb_color* color)
{
	ASSERT(color != NULL);

	real_argb_color use_color = *color;
	use_color.alpha = PIN(color->alpha, 0.0f, 1.0f);
	use_color.red = PIN(color->red, 0.0f, 1.0f);
	use_color.green = PIN(color->green, 0.0f, 1.0f);
	use_color.blue = PIN(color->blue, 0.0f, 1.0f);

	m_shadow_color = use_color;
}

void c_draw_string::set_style(e_text_style style)
{
	m_style = style;
}

void c_draw_string::set_tab_stops(const short* tab_stops, short count)
{
	ASSERT((tab_stops != NULL && count >= 0) || (tab_stops == NULL && count == 0));

	if (tab_stops)
	{
		short tab_stop_count = count;
		if (count > NUMBEROF(m_tab_stops))
		{
			tab_stop_count = NUMBEROF(m_tab_stops);
		}

		m_tab_stop_count = tab_stop_count;
		if (tab_stop_count > 0)
		{
			csmemcpy(m_tab_stops, tab_stops, sizeof(*m_tab_stops) * tab_stop_count);
		}
	}
	else
	{
		m_tab_stop_count = 0;
	}
}

c_simple_font_draw_string::c_simple_font_draw_string() :
	c_draw_string(),
	m_character_cache()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(base_address(0xD89148));
	DECLFUNC(0x2AB680, void, __thiscall, c_simple_font_draw_string*)(this);
}

c_rasterizer_draw_string::c_rasterizer_draw_string() :
	c_draw_string(),
	m_rotation_origin(),
	m_rotation(),
	m_sine_rotation(),
	m_cosine_rotation(),
	m_use_shader_system(),
	pad(),
	m_render_data()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(base_address(0xD85DC8));
}

c_simple_font_draw_string::s_character_group_render_data::s_character_group_render_data()
{
	reset();
	csmemset(characters, 0, sizeof(characters));
}

bool c_simple_font_draw_string::s_character_group_render_data::is_full()
{
	return count == 255;
}

void c_simple_font_draw_string::s_character_group_render_data::reset()
{
	color = 0;
	screen_x = 0;
	screen_y = 0;
	count = 0;
}

real __vectorcall draw_string_get_glyph_scaling_for_display_settings()
{
	return INVOKE(0x16DC30, draw_string_get_glyph_scaling_for_display_settings);
	//long aspect_ratio = interface_get_closest_authored_ui_aspect_ratio(); // interface_constants
	//
	//if (aspect_ratio == 0)
	//{
	//	return 1.0f;
	//}
	//
	//user_interface_shared_tag_globals = user_interface_shared_tag_globals_try_and_get(); // user_interface
	//if (!user_interface_shared_tag_globals)
	//{
	//	real glyph_scaling[2] = { 1.0f, 0.73f };
	//	return glyph_scaling[aspect_ratio];
	//}
	//
	//real width = user_interface_shared_tag_globals->aspect_ratio_content_scale_factor[aspect_ratio].i;
	//real height = user_interface_shared_tag_globals->aspect_ratio_content_scale_factor[aspect_ratio].j;
	//
	//if (width > height)
	//{
	//	return height;
	//}
	//else
	//{
	//	return width;
	//}
}
