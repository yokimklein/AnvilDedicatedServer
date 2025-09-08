#include "font_loading.h"
#include "text\font_fallback.h"
#include "cseries\cseries_events.h"
#include "text\font_cache.h"
#include "text\font_package_cache.h"

REFERENCE_DECLARE(0x3F863D4, s_font_globals, g_font_globals);

const s_font_header* font_get_header_internal(e_font_index internal_index)
{
	if (g_font_globals.font_package_header && internal_index >= 0 && internal_index < g_font_globals.font_package_header->font_count)
	{
		long header_offset = g_font_globals.font_package_header->fonts[internal_index].header_offset;
		return (s_font_header*)offset_pointer(g_font_globals.font_package_header, header_offset);
	}

	if (internal_index == _font_index_fallback)
	{
		return fallback_font_get_header();
	}

	return NULL;
}

e_font_index font_get_font_index(e_font_id font)
{
	e_font_index result = _font_index_none;
	if (g_font_globals.font_package_header && font >= 0 && font < 16)
	{
		result = (e_font_index)g_font_globals.font_package_header->font_mapping[font];
	}

	if (g_font_globals.emergency_mode || result == _font_index_none)
	{
		return _font_index_fallback;
	}

	return result;
}

const s_font_header* font_get_header(e_font_id font)
{
	e_font_index font_index = font_get_font_index(font);
	const s_font_header* result = font_get_header_internal(font_index);
	return result;
}

void font_load_idle(s_font_loading_state* loading_state, bool* out_failure_reported)
{
	if (loading_state->finished && loading_state->failed && !*out_failure_reported)
	{
		event(_event_critical, "fonts: font file '%s' failed to load, flushing fonts", loading_state->debug_filename);
		*out_failure_reported = true;
	}
}

bool font_in_emergency_mode()
{
	return g_font_globals.emergency_mode;
}

const s_font_package_file_header* font_get_package_header_internal()
{
	return g_font_globals.font_package_header;
}

void __fastcall font_initialize_emergency()
{
	if (!g_font_globals.initialized)
	{
		csmemset(&g_font_globals, 0, sizeof(g_font_globals));
		g_font_globals.language = _language_invalid;
		fallback_font_initialize();
		font_cache_new();
		font_package_cache_new();
		g_font_globals.initialized = true;
	}

	c_font_cache_mt_safe font_cache{};
	g_font_globals.emergency_mode = true;
}