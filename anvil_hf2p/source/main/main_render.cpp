#include "main_render.h"
#include "game\game.h"
#include "text\font_cache.h"
#include "text\draw_string.h"
#include "config\version.h"
#include "interface\interface_constants.h"
#include "hf2p\hf2p.h"
#include "cache\cache_files.h"
#include "rasterizer\rasterizer.h"

void __fastcall game_engine_render_frame_watermarks(bool pregame)
{
	if (version_get_build_number() == -1/* || global_preferences_get_hide_watermark()*/)
	{
		return;
	}

	rectangle2d display_bounds;
	interface_get_current_display_or_window_settings(NULL, NULL, NULL, &display_bounds);
	real_rectangle2d bounds{};
	bounds.x0 = (real)display_bounds.x0;
	bounds.x1 = (real)display_bounds.x1;
	bounds.y0 = (real)display_bounds.y0;
	bounds.y1 = (real)display_bounds.y1;
	c_static_wchar_string<256> cache_version;
	cache_version.print(L"%hs", cache_file_get_build_number());
	c_static_wchar_string<256> session_string;
	s_transport_secure_address session_id;
	session_string.print(L"%hs", transport_secure_address_get_string(get_session_secure_address(&session_id)));

	c_static_wchar_string<256> game_session_string;
	s_transport_secure_identifier game_session_id = *base_address<s_transport_secure_identifier*>(0x4A2CBD8); // $TODO: map global
	if (!hf2p_session_invalid(&game_session_id))
	{
		game_session_string.print(L"%hs", transport_secure_identifier_get_string(&game_session_id));
	}

	const char* build_version = version_get_build_string();
	c_static_wchar_string<512> build_info;
	build_info.print(L"%s|n%s|n%hs %hs %hs|n%s", session_string.get_string(), game_session_string.get_string(), build_version, version_get_build_date(), version_get_build_time(), cache_version.get_string());

	bool multiplayer_in_progress = false;
	bool is_splitscreen = false;
	if (game_options_valid() && game_is_multiplayer() && game_in_progress())
	{
		multiplayer_in_progress = true;
	}
	if (interface_is_split_screen())
	{
		is_splitscreen = true;
	}

	real_argb_color color = { 0.7f, 1.0f, 1.0f, 1.0f };
	e_font_id font = !is_splitscreen ? _full_screen_hud_message_font : _split_screen_hud_message_font;
	long scale_offset = 0;
	if (multiplayer_in_progress)
	{
		scale_offset = is_splitscreen + 1;
	}
	bool is_widescreen = rasterizer_get_is_widescreen();
	long scale_index = 3 * (is_widescreen + scale_offset + 2 * is_widescreen);
	c_rasterizer_draw_string draw_string;
	draw_string.set_font(font);
	REFERENCE_DECLARE_ARRAY(0xD81498, real, g_watermark_scales, 18);
	draw_string.set_scale(g_watermark_scales[scale_index]);
	draw_string.set_justification(_text_justification_right);
	draw_string.set_color(&color);
	draw_string.set_drop_shadow_style(_shadow_outline);
	long line_height = draw_string.get_line_height();
	bounds.y0 = (real)display_bounds.y1 - ((real)line_height * g_watermark_scales[scale_index + 1]);
	draw_string.set_bounds(&bounds);
	c_font_cache_mt_safe font_cache;
	draw_string.draw(&font_cache, build_info.get_string());
}

void __fastcall game_engine_render_frame_watermarks_anvil(bool pregame)
{
	rectangle2d display_bounds;
	rectangle2d display_title_safe_bounds;
	interface_get_current_display_or_window_settings(NULL, NULL, &display_bounds, &display_title_safe_bounds);

	rectangle2d bounds = display_title_safe_bounds;
	real_bounds scale = { 1.0f, 1.0f };

	c_static_wchar_string<512> watermark;
	watermark.print(L"cache %hs|n", cache_file_get_build_number());
	s_transport_secure_address session_id;
	watermark.append_print(L"%hs|n", transport_secure_address_get_string(get_session_secure_address(&session_id)));
	s_transport_secure_identifier game_session_id = *base_address<s_transport_secure_identifier*>(0x4A2CBD8); // $TODO: map global
	short lines = 3;
	if (!hf2p_session_invalid(&game_session_id))
	{
		watermark.append_print(L"%hs|n", transport_secure_identifier_get_string(&game_session_id));
		lines++;
	}
	watermark.append_print(L"%hs %hs %hs %hs",
		version_get_build_string(),
		version_get_target_configuration(),
		version_get_build_date(),
		version_get_build_time());

	bool multiplayer_in_progress = false;
	bool is_splitscreen = false;
	if (game_options_valid() && game_is_multiplayer() && game_in_progress())
	{
		multiplayer_in_progress = true;
	}
	if (interface_is_split_screen())
	{
		is_splitscreen = true;
	}

	real_argb_color color = { 0.4f, 1.0f, 1.0f, 1.0f };
	long scale_offset = 0;
	if (multiplayer_in_progress)
	{
		scale_offset = is_splitscreen + 1;
	}
	bool is_widescreen = rasterizer_get_is_widescreen();
	long scale_index = 3 * (is_widescreen + scale_offset + 2 * is_widescreen);
	c_rasterizer_draw_string draw_string;
	draw_string.set_font(_terminal_font);
	REFERENCE_DECLARE_ARRAY(0xD81498, real, g_watermark_scales, 18);
	draw_string.set_scale(g_watermark_scales[scale_index] * scale.lower);
	draw_string.set_justification(_text_justification_right);
	draw_string.set_color(&color);
	draw_string.set_drop_shadow_style(_shadow_outline);
	short line_height = draw_string.get_line_height();
	bounds.y0 = bounds.y1 - (line_height * lines) - 4;
	draw_string.set_bounds(&bounds);
	c_font_cache_mt_safe font_cache;
	draw_string.draw(&font_cache, watermark.get_string());
}