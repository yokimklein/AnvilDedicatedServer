#pragma once

enum e_yield_reason
{
	_yield_for_unknown = 0,
	_yield_for_sound_cache,
	_yield_for_texture_cache,
	_yield_for_geometry_cache,
	_yield_for_font_cache
};

class c_synchronized_long;
extern void __fastcall internal_async_yield_until_done(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, long line);
extern void __fastcall async_yield_until_done_function(c_synchronized_long* done, bool(*yield_function)(c_synchronized_long*), bool idle, bool networking, bool spinner, e_yield_reason yield_reason);