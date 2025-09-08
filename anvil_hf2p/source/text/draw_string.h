#pragma once

#include "cseries/cseries.h"
#include "interface/user_interface_text.h"
#include "text/font_cache.h"

struct s_font_header;
struct s_font_character;

struct dynamic_screen_vertex;
struct s_widget_geometry;

enum e_font_id
{
	_font_id_fallback = -2,
	_invalid_font = -1,

	_terminal_font = 0,
	_body_text_font,
	_title_font,
	_super_large_font,
	_large_body_text_font,
	_split_screen_hud_message_font,
	_full_screen_hud_message_font,
	_english_body_text_font,
	_hud_number_font,
	_subtitle_font,
	_main_menu_font,

	k_public_font_id_count,
};

enum e_text_style
{
	_text_style_plain = -1,
	_text_style_bold,
	_text_style_italic,
	_text_style_condense,
	_text_style_underline,

	k_text_style_count,
};

enum e_text_justification
{
	_text_justification_left = 0,
	_text_justification_right,
	_text_justification_center,

	k_text_justification_count,
};

enum e_text_drop_shadow_style
{
	_shadow_none = 0,
	_shadow_drop,
	_shadow_outline,

	k_number_of_text_drop_shadow_styles,
	k_default_drop_shadow_style = _shadow_drop,
};

class c_draw_string
{
public:
	enum e_text_flags
	{
		_text_flag_wrap_horizontally_bit = 0,
		_text_flag_wrap_vertically_bit,
		_text_flag_center_vertically_bit,
		_text_flag_align_bottom_vertically_bit,
		_text_flag_draw_utf32_bit,
		_text_flag_precache_required_bit,
		_text_flag_draw_more_bit,

		k_text_flags
	};

	struct s_parse_string_state
	{
		long string_index;
		const e_utf32* string;
		long result;
		long previous_result;
		utf32 character;
		utf32 previous_character;
		e_font_id current_font;
		e_font_id previous_font;
		real_argb_color base_color;
		real_argb_color modifier_color;
		e_text_justification current_justification;
		e_text_justification previous_justification;
		bool can_line_break_before_current_character;
		bool pad[0x3];
	};
	static_assert(sizeof(s_parse_string_state) == 0x4C);

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* dtor)(c_draw_string* _this);
		bool(__thiscall* draw_utf32)(c_draw_string* _this, c_font_cache_base* font_cache, const e_utf32* string);
		bool(__thiscall* draw_wchar)(c_draw_string* _this, c_font_cache_base* font_cache, const wchar_t* string);
		bool(__thiscall* draw)(c_draw_string* _this, c_font_cache_base* font_cache, const char* string);
		bool(__thiscall* predict_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character);
		bool(__thiscall* precache_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, const s_font_character** font_character);
		void(__thiscall* draw_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, ulong a4, ulong a5, real a6, real a7, real a8, real a9, real a10, real a11);
	}*__vftable;

public:
	c_draw_string();
	~c_draw_string();

	bool draw(c_font_cache_base* font_cache, const e_utf32* string)
	{
		return __vftable->draw_utf32(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, const wchar_t* string)
	{
		return __vftable->draw_wchar(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, const char* string)
	{
		return __vftable->draw(this, font_cache, string);
	}
	bool predict_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character)
	{
		return __vftable->predict_character(this, font_cache, font_id, character);
	}
	bool precache_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, const s_font_character** font_character)
	{
		return __vftable->precache_character(this, font_cache, font_id, character, font_character);
	}
	void draw_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, ulong a4, ulong a5, real a6, real a7, real a8, real a9, real a10, real a11)
	{
		return __vftable->draw_character(this, font_cache, font_id, character, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	bool draw_more(c_font_cache_base* font_cache, const char* string);
	//draw_partial

	void get_cursor(point2d* cursor) const;
	void get_cursor(real_point2d* cursor) const;
	e_text_justification get_justification() const;
	short get_line_height() const;

	void set_align_bottom_vertically(bool align_bottom);
	void set_bounds(const real_rectangle2d* bounds, const real_rectangle2d* clip);
	void set_bounds(const real_rectangle2d* bounds);
	void set_bounds(const rectangle2d* bounds);
	void set_center_vertically(bool center_vertically);
	void set_color(ulong color);
	void set_color(pixel32 color);
	void set_color(const real_argb_color* color);
	void set_display_resolution_scale_adjustment(real scale);
	void set_drop_shadow_style(e_text_drop_shadow_style drop_shadow_style);
	void set_font(e_font_id font);
	void set_height_adjust(short height_adjust);
	void set_justification(e_text_justification justification);
	void set_paragraph_indent(short indent);
	void set_permutation_proc(bool(__cdecl* proc)(dynamic_screen_vertex*, void*), void* permutation_context);
	void set_precache_required(bool precache);
	void set_scale(real scale);
	void set_shadow_color(pixel32 color);
	void set_shadow_color(const real_argb_color* color);
	void set_style(e_text_style style);
	void set_tab_stops(const short* tab_stops, short count);
	void set_wrap_horizontally(bool wrap_horizontally);
	void text_bounds_draw_character(real screen_left, real screen_top, real bitmap_widthm, real bitmap_height);

protected:
	c_flags<e_text_flags, ulong, k_text_flags> m_flags;
	e_font_id m_font_id;
	const s_font_header* m_styled_font_header;
	e_text_style m_style;
	e_text_justification m_justification;
	e_text_drop_shadow_style m_drop_shadow_style;
	real_argb_color m_color;
	real_argb_color m_shadow_color;
	real m_scale;
	real m_display_resolution_scale_adjustment;
	short m_height_adjust;
	short m_tab_stop_count;
	short m_tab_stops[16];
	real_rectangle2d m_bounds;
	real_rectangle2d m_text_bounds;
	real_rectangle2d m_clip;
	real_point2d m_cursor;
	bool(__cdecl* m_permutation_proc)(dynamic_screen_vertex*, void*);
	void* m_permutation_context;
	short m_initial_indent;
	short m_paragraph_indent;
	c_draw_string::s_parse_string_state m_saved_parse_state;
	short m_saved_tab_stop_index;
	short m_saved_line_count;
	short m_saved_tab_stop_line_count;
	short m_saved_maximum_tab_stop_line_count;
};
static_assert(sizeof(c_draw_string) == 0x100);

class c_hardware_draw_string :
	public c_draw_string
{
public:
	c_hardware_draw_string();

protected:
	c_font_cache_mt_safe m_font_cache;
	long __unknown108;
	long __unknown10C;
	long __unknown110;
};
static_assert(sizeof(c_hardware_draw_string) == sizeof(c_draw_string) + 0x14);

class c_simple_font_draw_string :
	public c_draw_string
{
public:
	struct s_character_group_render_data
	{
		s_character_group_render_data();
		bool is_full();
		void reset();

	protected:
		ulong color;
		long screen_x;
		long screen_y;
		long count;
		char characters[256];
	};

public:
	c_simple_font_draw_string();

protected:
	s_character_group_render_data m_character_cache;
};
static_assert(sizeof(c_simple_font_draw_string) == sizeof(c_draw_string) + 0x110);

class c_rasterizer_draw_string :
	public c_draw_string
{
public:
	struct s_glyph_group_render_data
	{
		struct s_glyph_render_data
		{
			e_utf32 character;
			real screen_left;
			real screen_top;
			real bitmap_xoffset;
			real bitmap_yoffset;
			real bitmap_width;
			real bitmap_height;
		};
		static_assert(sizeof(s_glyph_render_data) == 0x1C);

		e_font_id font_id;
		ulong color;
		ulong shadow_color;
		s_glyph_render_data glyphs[64];
		long count;
	};
	static_assert(sizeof(s_glyph_group_render_data) == 0x710);

public:
	c_rasterizer_draw_string();
	void set_rotation(real angle_radians);
	void set_rotation_origin(const real_point2d* origin);

protected:
	real_point2d m_rotation_origin;
	real m_rotation;
	real m_sine_rotation;
	real m_cosine_rotation;
	bool m_use_shader_system;
	bool pad[0x3];
	s_glyph_group_render_data m_render_data;
};
static_assert(sizeof(c_rasterizer_draw_string) == sizeof(c_draw_string) + 0x728);

class c_chud_draw_string :
	public c_draw_string
{
public:
	c_chud_draw_string();

protected:
	s_widget_geometry* m_geometry;
	real m_character_scale;
	real_rectangle2d m_pixel_clip;
};
static_assert(sizeof(c_chud_draw_string) == sizeof(c_draw_string) + 0x18);

extern real __vectorcall draw_string_get_glyph_scaling_for_display_settings();