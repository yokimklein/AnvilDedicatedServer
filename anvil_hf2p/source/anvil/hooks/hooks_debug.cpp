#include "hooks_debug.h"
#include "anvil\hooks\hooks.h"
#include "cseries\cseries_events.h"
#include "render\views\render_view.h"
#include "profiler\profiler.h"
#include "interface\terminal.h"
#include "main\console.h"
#include "main\main.h"
#include "render\render_debug.h"
#include "hs\hs_function.h"
#include "text\font_fallback.h"
#include "text\font_loading.h"
#include "text\draw_string.h"
#include "camera\director.h"
#include "camera\debug_director.h"
#include "cseries\cseries_windows_debug_pc.h"
#include "cache\cache_files.h"
#include "shell\shell_windows.h"

void __cdecl main_game_reset_map_hook(s_hook_registers& registers)
{
#if defined(EVENTS_ENABLED)
    events_clear();
#endif
}

void __cdecl main_game_change_immediate_hook(s_hook_registers& registers)
{
#if defined(EVENTS_ENABLED)
    events_clear();
#endif
}

void __cdecl render_debug_window_render_hook(s_hook_registers& registers)
{
    long user_index = (long)registers.ecx;

#if defined(PLAY_ENABLED)
    render_debug_window_render(user_index);
#endif
}

void __cdecl shell_initialized_hook(s_hook_registers& registers)
{
#if defined(EVENTS_ENABLED)
    events_initialize();
#endif
}

void __cdecl main_loop_body_hook1(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	real shell_seconds_elapsed = *(real*)(registers.ebp - 0x18);

	PROFILER(update_console_terminal_and_debug_menu)
	{
		TAG_RESOURCES_GAME_LOCK();

		//if (main_globals.drop_cheat_tag)
		//{
		//	main_cheat_drop_tag_private();
		//}

		real seconds_elapsed = 0.0f;
		if (!main_time_halted())
		{
			seconds_elapsed = shell_seconds_elapsed;
		}

		terminal_update(seconds_elapsed);
		console_update(seconds_elapsed);
		//telnet_console_update();
		//xbox_connection_update();
		//remote_command_process();
		//debug_menu_update();
		//cinematic_debug_camera_control_update();
	}
#endif
}

void __cdecl main_loop_enter_hook1(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	console_initialize();
#endif
}

void __cdecl main_loop_enter_hook2(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	console_execute_initial_commands();
#endif
}

void __cdecl main_loop_exit_hook(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	console_dispose();
#endif
}

void __cdecl render_initialize_hook(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	render_debug_initialize();
#endif
}

#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
// ms30 moved input_update from the start of main_loop_body to after simulation_update, so it now runs after the
// terminal update (main_loop_body_hook1) and clears the input_suppressed flag the terminal sets while it is open.
// Without this, keys typed into the terminal (e.g. backspace) also reach the director and the game.
void __cdecl main_loop_body_input_update_hook(s_hook_registers& registers)
{
	if (terminal_gets_active())
	{
		input_suppress();
	}
}
#endif

void __cdecl main_loop_body_hook2(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	render_debug_reset_cache_to_game_tick_entires();
#endif
}

void __cdecl game_tick_hook1(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	render_debug_notify_game_tick_begin();
#endif
}

void __cdecl game_tick_hook2(s_hook_registers& registers)
{
#if defined(PLAY_ENABLED)
	render_debug_notify_game_tick_end();
#endif
}

#pragma runtime_checks("", off)
__declspec(safebuffers) void __cdecl main_loop_body_hook4(s_hook_registers& registers)
{
	bool halted = main_time_halted();
	__asm
	{
		cmp halted, 0
	}
}

__declspec(safebuffers) void __cdecl rumble_update_hook(s_hook_registers& registers)
{
	bool halted = main_time_halted();
	__asm
	{
		cmp halted, 0
	}
}
#pragma runtime_checks("", restore)

void __cdecl font_initialize_hook(s_hook_registers& registers)
{
	fallback_font_initialize();
}

void __cdecl font_initialize_emergency_hook(s_hook_registers& registers)
{
	fallback_font_initialize();
}

#pragma runtime_checks("", off)
__declspec(safebuffers) void __cdecl main_loop_body_hook3(s_hook_registers& registers)
{
	bool halted = main_time_halted();

	// return halted to cl
	registers.ecx = halted;
}

__declspec(safebuffers) void __cdecl c_draw_string__ctor_hook(s_hook_registers& registers)
{
	const s_font_header* font_header = font_get_header(_terminal_font);

	// return font_header to edx register
	registers.edx = reinterpret_cast<size_t>(font_header);
}

__declspec(safebuffers) void __cdecl c_draw_string__draw_internal_hook(s_hook_registers& registers)
{
	e_font_id font = (e_font_id)registers.esi;

	const s_font_header* font_header = font_get_header(font);

	// return font_header to esi register
	registers.esi = reinterpret_cast<size_t>(font_header);
}

__declspec(safebuffers) void __cdecl c_draw_string__parse_string_new_hook(s_hook_registers& registers)
{
	e_font_id font = (e_font_id)registers.ecx;

	const s_font_header* font_header = font_get_header(font);

	// return font_header to ecx register
	registers.ecx = reinterpret_cast<size_t>(font_header);
}

__declspec(safebuffers) void __cdecl font_cache_load_internal_hook(s_hook_registers& registers)
{
	e_font_id font_id = (e_font_id)registers.edx;
	e_font_index font_index = font_get_font_index(font_id);
	registers.eax = font_index; // return font_index value to eax register
}

__declspec(safebuffers) void __cdecl hardware_cache_load_character_hook(s_hook_registers& registers)
{
	e_font_id font_id = (e_font_id)registers.eax;
	e_font_index font_index = font_get_font_index(font_id);
	registers.edx = font_index; // return font_index value to edx register
}

__declspec(safebuffers) void __cdecl hardware_cache_predict_character_hook(s_hook_registers& registers)
{
	e_font_id font_id = (e_font_id)registers.esi;
	e_font_index font_index = font_get_font_index(font_id);
	registers.eax = font_index; // return font_index value to eax register
}
#pragma runtime_checks("", restore)

void __cdecl main_time_frame_rate_display_hook(s_hook_registers& registers)
{
	c_rasterizer_draw_string* display_pulse_rates_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0x10F0);
	c_rasterizer_draw_string* display_framerate_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0x8C8);

	display_pulse_rates_draw_string->set_font(_terminal_font);
	display_framerate_draw_string->set_font(_full_screen_hud_message_font);
}

void __cdecl director_render_hook(s_hook_registers& registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.esp + 0x9C4 - 0x868);

	rasterizer_draw_string->set_font(_large_body_text_font);
}

void __cdecl subtitle_render_hook(s_hook_registers& registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.esp + 0x1488 - 0x1238);

	rasterizer_draw_string->set_font(_subtitle_font);
}

void __cdecl game_engine_render_frame_watermarks_hook(s_hook_registers& registers)
{
	e_font_id font = (e_font_id)registers.ebx;
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0xFB0);

	rasterizer_draw_string->set_font(font);
}

void __cdecl render_fullscreen_text_hook(s_hook_registers& registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0xA50);

	rasterizer_draw_string->set_font(_font_id_fallback);
}

void __cdecl chud_get_string_width_hook(s_hook_registers& registers)
{
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x140);

	draw_string->set_font(_full_screen_hud_message_font);
}

void __cdecl c_user_interface_text__render_hook(s_hook_registers& registers)
{
	e_font_id font = (e_font_id)registers.eax;
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x870);

	draw_string->set_font(font);
}

void __cdecl c_user_interface_text__compute_text_bounds_hook(s_hook_registers& registers)
{
	e_font_id font = (e_font_id)registers.eax;
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x110);

	draw_string->set_font(font);
}

void __cdecl chud_build_text_geometry_hook(s_hook_registers& registers)
{
	e_font_id font = *(e_font_id*)(registers.ebp + 0x08);
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x140);

	draw_string->set_font(font);
}

void __cdecl font_loading_idle_hook(s_hook_registers& registers)
{
	font_load_idle(&g_font_globals.package_loading_state, &g_font_globals.fonts_unavailable);
}

#pragma runtime_checks("", off)
e_character_status __fastcall font_cache_retrieve_character_hook(ulong character_key, const s_font_character** out_character, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count> flags, const void** out_pixel_data)
{
	// $TODO: Something is broken with the rewritten function and causes it to fail to locate font characters
	//return font_cache_retrieve_character(character_key, flags, out_character, out_pixel_data);

	if (word(character_key >> 16) == (word)_font_index_fallback && fallback_font_get_character(e_utf32(character_key & 0xFFFF), out_character, out_pixel_data))
	{
		return _character_status_ready;
	}

	e_character_status result = _character_status_invalid;
	result = DECLFUNC(ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_HOOK, e_character_status, __fastcall, ulong, const s_font_character**, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count>, const void**)(character_key, out_character, flags, out_pixel_data);
	__asm { add esp, 8 }; // cleanup usercall
	return result;
}
#pragma runtime_checks("", restore)

void __cdecl shell_dispose_hook(s_hook_registers& registers)
{
	events_dispose();
}

void __fastcall c_draw_string__set_font_hook(c_draw_string* thisptr, void* unused, e_font_id font)
{
	thisptr->set_font(font);
}

void __cdecl director_update_hook(s_hook_registers& registers)
{
	long user_index = (long)registers.edi;

	survival_mode_update_flying_camera(user_index);
	control_toggling_of_debug_directors(user_index);
}

void __fastcall c_debug_director__update_hook(c_debug_director* thisptr, void* unused, real dt)
{
	thisptr->update_(dt);
}

void anvil_hooks_debug_apply()
{
	// events
	hook::insert(ADDRESS_MAIN_GAME_RESET_MAP_HOOK, ADDRESS_MAIN_GAME_RESET_MAP_HOOK_RETURN, main_game_reset_map_hook, _hook_execute_replaced_last);
	hook::insert(ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK, ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK_RETURN, main_game_change_immediate_hook, _hook_execute_replaced_last);
	hook::insert(ADDRESS_SHELL_INITIALIZED_HOOK, ADDRESS_SHELL_INITIALIZED_HOOK_RETURN, shell_initialized_hook, _hook_execute_replaced_last);

	// debug render
	hook::insert(ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK, ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK_RETURN, render_debug_window_render_hook, _hook_replace);
	hook::insert(ADDRESS_RENDER_INITIALIZE_HOOK, ADDRESS_RENDER_INITIALIZE_HOOK_RETURN, render_initialize_hook, _hook_execute_replaced_first);
	hook::insert(ADDRESS_MAIN_LOOP_BODY_HOOK2, ADDRESS_MAIN_LOOP_BODY_HOOK2_RETURN, main_loop_body_hook2, _hook_execute_replaced_last);
	hook::insert(ADDRESS_GAME_TICK_HOOK1, ADDRESS_GAME_TICK_HOOK1_RETURN, game_tick_hook1, _hook_execute_replaced_first);
	hook::insert(ADDRESS_GAME_TICK_HOOK2, ADDRESS_GAME_TICK_HOOK2_RETURN, game_tick_hook2, _hook_execute_replaced_first);

	// console/terminal
	hook::insert(ADDRESS_MAIN_LOOP_BODY_HOOK1, ADDRESS_MAIN_LOOP_BODY_HOOK1_RETURN, main_loop_body_hook1, _hook_execute_replaced_first);
#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
	hook::insert(ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK1, ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK1_RETURN, main_loop_body_input_update_hook, _hook_execute_replaced_first);
	hook::insert(ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK2, ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK2_RETURN, main_loop_body_input_update_hook, _hook_execute_replaced_first);
#endif
	hook::insert(ADDRESS_MAIN_LOOP_ENTER_HOOK1, ADDRESS_MAIN_LOOP_ENTER_HOOK1_RETURN, main_loop_enter_hook1, _hook_execute_replaced_last);
	hook::insert(ADDRESS_MAIN_LOOP_ENTER_HOOK2, ADDRESS_MAIN_LOOP_ENTER_HOOK2_RETURN, main_loop_enter_hook2, _hook_execute_replaced_last);
	hook::insert(ADDRESS_MAIN_LOOP_EXIT_HOOK, ADDRESS_MAIN_LOOP_EXIT_HOOK_RETURN, main_loop_exit_hook, _hook_execute_replaced_last);
	hook::call(ADDRESS_RENDER_DEBUG_FRAME_RENDER_CALL, render_debug_frame_render);

	// reimplement hs print
	patch::function(ADDRESS_PRINT_HS_PRINT_1_EVALUATE_SLOT, print_hs_print_1_evaluate);
	patch::function(ADDRESS_LOG_PRINT_HS_LOG_PRINT_1_EVALUATE_SLOT, log_print_hs_log_print_1_evaluate);

	// reimplement hs events_suppress_console_display
	patch::function(ADDRESS_EVENTS_SUPPRESS_DISPLAY_EVENTS_SUPPRESS_OUTPUT_1_EVALUATE_SLOT, events_suppress_display_events_suppress_output_1_evaluate);

	// main_time_halted for console pausing
	hook::insert(ADDRESS_MAIN_LOOP_BODY_HOOK3, ADDRESS_MAIN_LOOP_BODY_HOOK3_RETURN, main_loop_body_hook3, _hook_replace);
	hook::insert(ADDRESS_MAIN_LOOP_BODY_HOOK4, ADDRESS_MAIN_LOOP_BODY_HOOK4_RETURN, main_loop_body_hook4, _hook_replace);
	hook::insert(ADDRESS_RUMBLE_UPDATE_HOOK, ADDRESS_RUMBLE_UPDATE_HOOK_RETURN, rumble_update_hook, _hook_replace);

	// fallback font
	hook::insert(ADDRESS_FONT_INITIALIZE_HOOK, ADDRESS_FONT_INITIALIZE_HOOK_RETURN, font_initialize_hook, _hook_execute_replaced_last);
	
	// $TODO: HOOK ENTIRE FUNC
	//hook::insert(0x9F208, 0x9F20D, font_initialize_emergency_hook, _hook_execute_replaced_last);
	hook::function(ADDRESS_FONT_INITIALIZE_EMERGENCY, 0xAC, font_initialize_emergency);
	
	hook::insert(ADDRESS_FONT_LOADING_IDLE_HOOK, ADDRESS_FONT_LOADING_IDLE_HOOK_RETURN, font_loading_idle_hook, _hook_execute_replaced_last);
	patch::nop_region(ADDRESS_FONT_LOADING_IDLE_NOP, 10); // disable damaged_media_halt_and_display_error();

	//hook::function(0x16B870, 0x98, font_cache_retrieve_character_hook);
	hook::call(ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL, font_cache_retrieve_character_hook);
	hook::call(ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_2, font_cache_retrieve_character_hook);
	hook::call(ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_3, font_cache_retrieve_character_hook);
	hook::call(ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_4, font_cache_retrieve_character_hook);
	patch::nop_region(ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP, 3); // cleanup usercall
	patch::nop_region(ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_2, 3); // cleanup usercall
	patch::nop_region(ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_3, 3); // cleanup usercall
	patch::nop_region(ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_4, 3); // cleanup usercall
	// font_get_header inlines
	hook::insert(ADDRESS_C_DRAW_STRING_CTOR_HOOK, ADDRESS_C_DRAW_STRING_CTOR_HOOK_RETURN, c_draw_string__ctor_hook, _hook_replace);
	hook::insert(ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK, ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK_RETURN, c_draw_string__draw_internal_hook, _hook_replace);
	hook::insert(ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK, ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK_RETURN, c_draw_string__parse_string_new_hook, _hook_replace);
	// c_draw_string::set_font
	hook::function(ADDRESS_C_DRAW_STRING_SET_FONT, 0x65, c_draw_string__set_font_hook);
	// inlines
	hook::insert(ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK, ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK_RETURN, main_time_frame_rate_display_hook, _hook_replace);
	patch::nop_region(ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_NOP, 6); // remove leftover code from inline
	hook::insert(ADDRESS_DIRECTOR_RENDER_HOOK, ADDRESS_DIRECTOR_RENDER_HOOK_RETURN, director_render_hook, _hook_replace);
	patch::nop_region(ADDRESS_DIRECTOR_RENDER_NOP, 7); // remove leftover code from inline
	hook::insert(ADDRESS_SUBTITLE_RENDER_HOOK, ADDRESS_SUBTITLE_RENDER_HOOK_RETURN, subtitle_render_hook, _hook_replace);
	patch::nop_region(ADDRESS_SUBTITLE_RENDER_NOP, 7); // remove leftover code from inline
	hook::insert(ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK, ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK_RETURN, game_engine_render_frame_watermarks_hook, _hook_replace);
	patch::nop_region(ADDRESS_GAME_ENGINE_RENDER_WATERMARKS_NOP, 6); // remove leftover code from inline
	hook::insert(ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK, ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK_RETURN, render_fullscreen_text_hook, _hook_replace);
	patch::nop_region(ADDRESS_RENDER_FULLSCREEN_TEXT_NOP, 6); // remove leftover code from inline
	hook::insert(ADDRESS_CHUD_GET_STRING_WIDTH_HOOK, ADDRESS_CHUD_GET_STRING_WIDTH_HOOK_RETURN, chud_get_string_width_hook, _hook_replace);
	patch::nop_region(ADDRESS_CHUD_GET_STRING_WIDTH_NOP, 3); // remove leftover code from inline
	hook::insert(ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK, ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK_RETURN, c_user_interface_text__compute_text_bounds_hook, _hook_replace);
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP, 2); // remove leftover code from inline
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_2, 10); // remove leftover code from inline
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_3, 14); // remove leftover code from inline
	hook::insert(ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK, ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK_RETURN, c_user_interface_text__render_hook, _hook_replace);
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP, 7); // remove leftover code from inline
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_2, 16); // remove leftover code from inline
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_3, 9); // remove leftover code from inline
	patch::nop_region(ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_4, 6); // remove leftover code from inline
	hook::insert(ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK, ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK_RETURN, chud_build_text_geometry_hook, _hook_replace);
	patch::nop_region(ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP, 17); // remove leftover code from inline
	patch::nop_region(ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP_2, 6); // remove leftover code from inline
	
	// font_get_font_index
	hook::insert(ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK, ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK_RETURN, font_cache_load_internal_hook, _hook_replace);
	hook::insert(ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK, ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK_RETURN, hardware_cache_load_character_hook, _hook_replace);
	hook::insert(ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK, ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK_RETURN, hardware_cache_predict_character_hook, _hook_replace);

	// events_dispose in inlined cseries_dispose @ shell_dispose
	hook::insert(ADDRESS_SHELL_DISPOSE_HOOK, ADDRESS_SHELL_DISPOSE_HOOK_RETURN, shell_dispose_hook, _hook_execute_replaced_last);

	// director
	hook::insert(ADDRESS_DIRECTOR_UPDATE_HOOK, ADDRESS_DIRECTOR_UPDATE_HOOK_RETURN, director_update_hook, _hook_execute_replaced_last);
	//patch::bytes(0x1BE2AE, { _key_backspace }); // rebind camera mode swap from f12 to backspace to avoid breakpoint keybind
	patch::function(ADDRESS_C_DEBUG_DIRECTOR_UPDATE_HOOK_SLOT, c_debug_director__update_hook);

	// exceptions/asserts
	hook::function(ADDRESS_EXCEPTIONS_UPDATE, 0x190, exceptions_update);
	hook::function(ADDRESS_TOPLEVELEXCEPTIONFILTER, 0x5A, TopLevelExceptionFilter);

	// hook tag_get to store last tag index
	// The vast majority of tag_get instances have been inlined, so the 'last tag accessed' may not necessarily be accurate
	// There are only 265 calls to tag_get in ms29, vs 2652 in ms23
	// ms29's call has also optimised away the group_tag argument, so this will read invalid data
	//hook::function(0x83C70, 0x21, tag_get);

	// catch fire
	hook::function(ADDRESS_MAIN_HALT_AND_CATCH_FIRE, 0x140, main_halt_and_catch_fire);
}
