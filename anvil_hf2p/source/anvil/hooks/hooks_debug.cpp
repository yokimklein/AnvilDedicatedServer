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

void __cdecl main_game_reset_map_hook(s_hook_registers registers)
{
#if defined(EVENTS_ENABLED)
    events_clear();
#endif
}

void __cdecl main_game_change_immediate_hook(s_hook_registers registers)
{
#if defined(EVENTS_ENABLED)
    events_clear();
#endif
}

void __cdecl render_debug_window_render_hook(s_hook_registers registers)
{
    long user_index = (long)registers.ecx;

#if defined(PLAY_ENABLED)
    render_debug_window_render(user_index);
#endif
}

void __cdecl shell_initialized_hook(s_hook_registers registers)
{
#if defined(EVENTS_ENABLED)
    events_initialize();
#endif
}

void __cdecl main_loop_body_hook1(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	real shell_seconds_elapsed = *(real*)(registers.ebp - 0x18);

	PROFILER(update_console_terminal_and_debug_menu)
	{
		LOCAL_TAG_RESOURCE_SCOPE_LOCK;

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

void __cdecl main_loop_enter_hook1(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	console_initialize();
#endif
}

void __cdecl main_loop_enter_hook2(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	console_execute_initial_commands();
#endif
}

void __cdecl main_loop_exit_hook(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	console_dispose();
#endif
}

void __cdecl render_initialize_hook(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	render_debug_initialize();
#endif
}

void __cdecl main_loop_body_hook2(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	render_debug_reset_cache_to_game_tick_entires();
#endif
}

void __cdecl game_tick_hook1(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	render_debug_notify_game_tick_begin();
#endif
}

void __cdecl game_tick_hook2(s_hook_registers registers)
{
#if defined(PLAY_ENABLED)
	render_debug_notify_game_tick_end();
#endif
}

void __cdecl main_loop_body_hook4(s_hook_registers registers)
{
	bool halted = main_time_halted();
	__asm
	{
		cmp halted, 0
	}
}

void __cdecl rumble_update_hook(s_hook_registers registers)
{
	bool halted = main_time_halted();
	__asm
	{
		cmp halted, 0
	}
}

void __cdecl font_initialize_hook(s_hook_registers registers)
{
	fallback_font_initialize();
}

void __cdecl font_initialize_emergency_hook(s_hook_registers registers)
{
	fallback_font_initialize();
}

#pragma runtime_checks("", off)
__declspec(safebuffers) void __cdecl main_loop_body_hook3(s_hook_registers registers)
{
	__asm
	{
		// preserve registers
		push edi
		push esi
		push edx
		push ebx
		push eax
	}

	bool halted = main_time_halted();

	__asm
	{
		// return halted to cl
		mov cl, halted
		// restore registers
		pop eax
		pop ebx
		pop edx
		pop esi
		pop edi
	}
}

__declspec(safebuffers) void __cdecl c_draw_string__ctor_hook(s_hook_registers registers)
{
	__asm
	{
		// preserve registers
		push edi
		push esi
		push ecx
		push ebx
		push eax
	}

	const s_font_header* font_header = font_get_header(_terminal_font);

	__asm
	{
		// return font_header to edx register
		mov edx, font_header
		// restore registers
		pop eax
		pop ebx
		pop ecx
		pop esi
		pop edi
	}
}

__declspec(safebuffers) void __cdecl c_draw_string__draw_internal_hook(s_hook_registers registers)
{
	__asm
	{
		// preserve registers
		push edi
		push edx
		push ecx
		push ebx
		push eax
	}
	e_font_id font = (e_font_id)registers.esi;

	const s_font_header* font_header = font_get_header(font);

	__asm
	{
		// return font_header to esi register
		mov esi, font_header
		// restore registers
		pop eax
		pop ebx
		pop ecx
		pop edx
		pop edi
	}
}

__declspec(safebuffers) void __cdecl c_draw_string__parse_string_new_hook(s_hook_registers registers)
{
	// preserve registers
	__asm
	{
		push edi
		push esi
		push edx
		push ebx
		push eax
	}
	e_font_id font = (e_font_id)registers.ecx;

	const s_font_header* font_header = font_get_header(font);

	__asm
	{
		// return font_header to ecx
		mov ecx, font_header
		// restore registers
		pop eax
		pop ebx
		pop edx
		pop esi
		pop edi
	}
}

__declspec(safebuffers) void __cdecl font_cache_load_internal_hook(s_hook_registers registers)
{
	e_font_id font_id = (e_font_id)registers.edx;
	__asm
	{
		// preserve registers
		push edi
		push esi
		push edx
		push ecx
		push ebx
	}

	e_font_index font_index = font_get_font_index(font_id);

	__asm
	{
		// return value
		mov eax, font_index
		// restore registers
		pop ebx
		pop ecx
		pop edx
		pop esi
		pop edi
	}
}

__declspec(safebuffers) void __cdecl hardware_cache_load_character_hook(s_hook_registers registers)
{
	e_font_id font_id = (e_font_id)registers.eax;
	__asm
	{
		// preserve registers
		push edi
		push esi
		push ecx
		push ebx
		push eax
	}

	e_font_index font_index = font_get_font_index(font_id);

	__asm
	{
		// return value
		mov edx, font_index
		// restore registers
		pop eax
		pop ebx
		pop ecx
		pop esi
		pop edi
	}
}

__declspec(safebuffers) void __cdecl hardware_cache_predict_character_hook(s_hook_registers registers)
{
	e_font_id font_id = (e_font_id)registers.esi;
	__asm
	{
		// preserve registers
		push edi
		push esi
		push edx
		push ecx
		push ebx
	}

	e_font_index font_index = font_get_font_index(font_id);

	__asm
	{
		// return value
		mov eax, font_index
		// restore registers
		pop ebx
		pop ecx
		pop edx
		pop esi
		pop edi
	}
}
#pragma runtime_checks("", restore)

void __cdecl main_time_frame_rate_display_hook(s_hook_registers registers)
{
	c_rasterizer_draw_string* display_pulse_rates_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0x10F0);
	c_rasterizer_draw_string* display_framerate_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0x8C8);

	display_pulse_rates_draw_string->set_font(_terminal_font);
	display_framerate_draw_string->set_font(_full_screen_hud_message_font);
}

void __cdecl director_render_hook(s_hook_registers registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.esp + 0x9C4 - 0x868);

	rasterizer_draw_string->set_font(_large_body_text_font);
}

void __cdecl subtitle_render_hook(s_hook_registers registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.esp + 0x1488 - 0x1238);

	rasterizer_draw_string->set_font(_subtitle_font);
}

void __cdecl game_engine_render_frame_watermarks_hook(s_hook_registers registers)
{
	e_font_id font = (e_font_id)registers.ebx;
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0xFB0);

	rasterizer_draw_string->set_font(font);
}

void __cdecl render_fullscreen_text_hook(s_hook_registers registers)
{
	c_rasterizer_draw_string* rasterizer_draw_string = (c_rasterizer_draw_string*)(registers.ebp - 0xA50);

	rasterizer_draw_string->set_font(_font_id_fallback);
}

void __cdecl chud_get_string_width_hook(s_hook_registers registers)
{
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x140);

	draw_string->set_font(_full_screen_hud_message_font);
}

void __cdecl c_user_interface_text__render_hook(s_hook_registers registers)
{
	e_font_id font = (e_font_id)registers.eax;
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x870);

	draw_string->set_font(font);
}

void __cdecl c_user_interface_text__compute_text_bounds_hook(s_hook_registers registers)
{
	e_font_id font = (e_font_id)registers.eax;
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x110);

	draw_string->set_font(font);
}

void __cdecl chud_build_text_geometry_hook(s_hook_registers registers)
{
	e_font_id font = *(e_font_id*)(registers.ebp + 0x08);
	c_draw_string* draw_string = (c_draw_string*)(registers.ebp - 0x140);

	draw_string->set_font(font);
}

void __cdecl font_loading_idle_hook(s_hook_registers registers)
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
	result = DECLFUNC(0x16B870, e_character_status, __fastcall, ulong, const s_font_character**, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count>, const void**)(character_key, out_character, flags, out_pixel_data);
	__asm { add esp, 8 }; // cleanup usercall
	return result;
}
#pragma runtime_checks("", restore)

void __cdecl shell_dispose_hook(s_hook_registers registers)
{
	events_dispose();
}

void __fastcall c_draw_string__set_font_hook(c_draw_string* thisptr, void* unused, e_font_id font)
{
	thisptr->set_font(font);
}

void __cdecl director_update_hook(s_hook_registers registers)
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
	hook::insert(0xA9333, 0xA9338, main_game_reset_map_hook, _hook_execute_replaced_last);
	hook::insert(0xA9603, 0xA9608, main_game_change_immediate_hook, _hook_execute_replaced_last);
	hook::insert(0x1045, 0x104D, shell_initialized_hook, _hook_execute_replaced_last);

	// debug render
	hook::insert(0x271938, 0x27193D, render_debug_window_render_hook, _hook_replace);
	hook::insert(0x268BEB, 0x268BF0, render_initialize_hook, _hook_execute_replaced_first);
	hook::insert(0x957EE, 0x957F3, main_loop_body_hook2, _hook_execute_replaced_last);
	hook::insert(0xB1A1B, 0xB1A22, game_tick_hook1, _hook_execute_replaced_first);
	hook::insert(0xB1C53, 0xB1C5B, game_tick_hook2, _hook_execute_replaced_first);

	// console/terminal
	hook::insert(0x95883, 0x95888, main_loop_body_hook1, _hook_execute_replaced_first);
	hook::insert(0x952A8, 0x952AD, main_loop_enter_hook1, _hook_execute_replaced_last);
	hook::insert(0x95346, 0x9534B, main_loop_enter_hook2, _hook_execute_replaced_last);
	hook::insert(0x95DB1, 0x95DBB, main_loop_exit_hook, _hook_execute_replaced_last);
	hook::call(0x1644A1, render_debug_frame_render);

	// reimplement hs print
	patch::function(0xD4A234, print_hs_print_1_evaluate);
	patch::function(0xD4C028, log_print_hs_log_print_1_evaluate);

	// reimplement hs events_suppress_console_display
	patch::function(0xD4C678, events_suppress_display_events_suppress_output_1_evaluate);

	// main_time_halted for console pausing
	hook::insert(0x958FD, 0x95903, main_loop_body_hook3, _hook_replace_no_preserve);
	hook::insert(0x95A9D, 0x95AA4, main_loop_body_hook4, _hook_replace);
	hook::insert(0x164856, 0x16485D, rumble_update_hook, _hook_replace);

	// fallback font
	hook::insert(0x9F2B7, 0x9F2BC, font_initialize_hook, _hook_execute_replaced_last);
	
	// $TODO: HOOK ENTIRE FUNC
	//hook::insert(0x9F208, 0x9F20D, font_initialize_emergency_hook, _hook_execute_replaced_last);
	hook::function(0x9F1E0, 0xAC, font_initialize_emergency);
	
	hook::insert(0x9F661, 0x9F666, font_loading_idle_hook, _hook_execute_replaced_last);
	patch::nop_region(0x9F6C3, 10); // disable damaged_media_halt_and_display_error();

	//hook::function(0x16B870, 0x98, font_cache_retrieve_character_hook);
	hook::call(0x16B6BF, font_cache_retrieve_character_hook);
	hook::call(0x16B747, font_cache_retrieve_character_hook);
	hook::call(0x16B768, font_cache_retrieve_character_hook);
	hook::call(0x16B783, font_cache_retrieve_character_hook);
	patch::nop_region(0x16B6C7, 3); // cleanup usercall
	patch::nop_region(0x16B74E, 3); // cleanup usercall
	patch::nop_region(0x16B76F, 3); // cleanup usercall
	patch::nop_region(0x16B788, 3); // cleanup usercall
	// font_get_header inlines
	hook::insert(0x16C152, 0x16C17A, c_draw_string__ctor_hook, _hook_replace_no_preserve);
	hook::insert(0x16CEF8, 0x16CF25, c_draw_string__draw_internal_hook, _hook_replace_no_preserve);
	hook::insert(0x16D429, 0x16D455, c_draw_string__parse_string_new_hook, _hook_replace_no_preserve);
	// c_draw_string::set_font
	hook::function(0x16C760, 0x65, c_draw_string__set_font_hook);
	// inlines
	hook::insert(0x9FE99, 0x9FF03, main_time_frame_rate_display_hook, _hook_replace);
	patch::nop_region(0x9FF09, 6); // remove leftover code from inline
	hook::insert(0xE2BF2, 0xE2C22, director_render_hook, _hook_replace);
	patch::nop_region(0xE2C2E, 7); // remove leftover code from inline
	hook::insert(0x175F4F, 0x175F7F, subtitle_render_hook, _hook_replace);
	patch::nop_region(0x175F8E, 7); // remove leftover code from inline
	hook::insert(0x1B0EDD, 0x1B0F30, game_engine_render_frame_watermarks_hook, _hook_replace);
	patch::nop_region(0x1B0F48, 6); // remove leftover code from inline
	hook::insert(0x269BBC, 0x269BEB, render_fullscreen_text_hook, _hook_replace);
	patch::nop_region(0x269BEE, 6); // remove leftover code from inline
	hook::insert(0x3CE494, 0x3CE4BC, chud_get_string_width_hook, _hook_replace);
	patch::nop_region(0x3CE482, 3); // remove leftover code from inline
	hook::insert(0x3DA0CA, 0x3DA0FC, c_user_interface_text__compute_text_bounds_hook, _hook_replace);
	patch::nop_region(0x3DA09C, 2); // remove leftover code from inline
	patch::nop_region(0x3DA0A4, 10); // remove leftover code from inline
	patch::nop_region(0x3DA0B4, 14); // remove leftover code from inline
	hook::insert(0x3D9CF9, 0x3D9D25, c_user_interface_text__render_hook, _hook_replace);
	patch::nop_region(0x3D9C6C, 7); // remove leftover code from inline
	patch::nop_region(0x3D9C81, 16); // remove leftover code from inline
	patch::nop_region(0x3D9CA4, 9); // remove leftover code from inline
	patch::nop_region(0x3D9D43, 6); // remove leftover code from inline
	hook::insert(0x3E0EA6, 0x3E0EE4, chud_build_text_geometry_hook, _hook_replace);
	patch::nop_region(0x3E0E91, 17); // remove leftover code from inline
	patch::nop_region(0x3E0EFF, 6); // remove leftover code from inline
	
	// font_get_font_index
	hook::insert(0x16B675, 0x16B689, font_cache_load_internal_hook, _hook_replace_no_preserve);
	hook::insert(0x25F3E8, 0x25F3FE, hardware_cache_load_character_hook, _hook_replace_no_preserve);
	hook::insert(0x25F2E3, 0x25F300, hardware_cache_predict_character_hook, _hook_replace_no_preserve);

	// events_dispose in inlined cseries_dispose @ shell_dispose
	hook::insert(0x123A, 0x123F, shell_dispose_hook, _hook_execute_replaced_last);

	// director
	hook::insert(0xE280D, 0xE2813, director_update_hook, _hook_execute_replaced_last);
	//patch::bytes(0x1BE2AE, { _key_backspace }); // rebind camera mode swap from f12 to backspace to avoid breakpoint keybind
	patch::function(0xD81A24, c_debug_director__update_hook);
}
