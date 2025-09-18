#include "main.h"
#include "cseries\cseries.h"
#include "multithreading\threads.h"
#include "game\game.h"
#include "main\console.h"
#include "shell\shell.h"
#include "cache\cache_file_tag_resource_runtime.h"
//#include "cache\cache_files_windows.h"
#include "cache\cache_files.h"
#include "cseries\stack_walk_windows.h"
#include "cseries\cseries_events.h"
#include "cseries\cseries_windows_debug_pc.h"
#include "text\font_cache.h"
#include "text\font_loading.h"
#include "networking\transport\transport.h"
#include "cseries\progress.h"
#include "online\online_lsp.h"
#include "rasterizer\rasterizer.h"
#include "main\main_render.h"
#include "cseries\async_xoverlapped.h"

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

REFERENCE_DECLARE_ARRAY(0x103E7C8, s_main_status_value, g_status_values, 32);
REFERENCE_DECLARE(0x3EE1E48, _main_globals, main_globals);

const char* const k_crash_info_output_filename = "crash_report\\crash_info.txt";

bool debug_console_pauses_game = true;
bool g_force_upload_even_if_untracked = true;

bool main_is_in_main_loop_pregame()
{
	return main_globals.main_loop_pregame_entered != 0;
}

long __cdecl _internal_halt_render_thread_and_lock_resources(const char* file_name, long line_number)
{
	return INVOKE(0x94CB0, _internal_halt_render_thread_and_lock_resources, file_name, line_number);
}

void __fastcall main_exit_game()
{
	main_globals.exit_game = true;
}

void main_loop_pregame_disable(bool disable)
{
	if (is_main_thread())
	{
		if (disable)
		{
			main_globals.main_loop_pregame_entered++;
		}
		else
		{
			main_globals.main_loop_pregame_entered--;
		}
	}
}

c_tag_resources_game_lock::c_tag_resources_game_lock()
{
	tag_resources_lock_game(m_resource_key);
}

c_tag_resources_game_lock::~c_tag_resources_game_lock()
{
	tag_resources_unlock_game(m_resource_key);
}

bool main_time_halted()
{
	bool time_halted = shell_application_is_paused();

	if ((debug_console_pauses_game && debugging_system_has_focus()) && (!game_in_progress() || !game_has_nonlocal_players() || game_is_authoritative_playback()))
	{
		time_halted = true;
	}

	if (main_globals.reset_zone_resources)
	{
		time_halted = true;
	}

	return time_halted;
}

void main_write_stack_to_crash_info_status_file(const char* crash_info, void* context)
{
	s_file_reference crash_info_output_file{};
	file_reference_create_from_path(&crash_info_output_file, k_crash_info_output_filename, false);
	file_create_parent_directories_if_not_present(&crash_info_output_file);

	if (file_exists(&crash_info_output_file))
	{
		file_delete(&crash_info_output_file);
	}

	ulong error = 0;
	if (file_create(&crash_info_output_file) && file_open(&crash_info_output_file, FLAG(_file_open_flag_desired_access_write), &error))
	{
		const char* string = "stack:\r\n";
		file_write(&crash_info_output_file, strlen(string), string);

		if (context)
		{
			stack_walk_with_context(&crash_info_output_file, 1, static_cast<_CONTEXT*>(context));
		}
		else
		{
			stack_walk_to_file(&crash_info_output_file, 3);
		}

		if (crash_info)
		{
			file_write(&crash_info_output_file, strlen(crash_info), crash_info);
		}

		main_status("system_milliseconds", "time %d", system_milliseconds());
		main_status_dump(&crash_info_output_file);

		// dump last accessed tag and resource owner
		c_static_string<1024> last_accessed;
		char tag_group[8]{};
		REFERENCE_DECLARE(0x0190E460, long, last_resource_owner);

		if (g_last_tag_accessed.index != NONE)
		{
			tag_to_string(g_last_tag_accessed.group_tag, tag_group);
			last_accessed.print_line("last accessed tag:\r\n    ['%s', %04X] '%s.%s'",
				tag_group,
				g_last_tag_accessed.index,
				g_last_tag_accessed.get_name(),
				g_last_tag_accessed.get_group_name());
			file_write(&crash_info_output_file, last_accessed.length(), last_accessed.get_string());
		}

		// $TODO: !!!
		//if (last_resource_owner != NONE && g_cache_file_globals.resource_gestalt != NULL)
		//{
		//	for (s_cache_file_tag_resource_data* resource : g_cache_file_globals.resource_gestalt->resources)
		//	{
		//		if (resource->runtime_data.owner_tag.index != last_resource_owner)
		//			continue;
		//
		//		tag_to_string(resource->runtime_data.owner_tag.group_tag, tag_group);
		//		last_accessed.print_line("last accessed resource owner:\r\n    ['%s', %04X] '%s.%s'",
		//			tag_group,
		//			resource->runtime_data.owner_tag.index,
		//			resource->runtime_data.owner_tag.get_name(),
		//			resource->runtime_data.owner_tag.get_group_name());
		//
		//		break;
		//	}
		//	file_write(&crash_info_output_file, last_accessed.length(), last_accessed.get_string());
		//}

		file_close(&crash_info_output_file);
	}
}

void main_status_dump(s_file_reference* file)
{
	if (file)
	{
		file_write(file, 41, "fatal error detected, current status was:");
		file_write(file, 2, "\r\n");
	}

	for (long i = 0; i < NUMBEROF(g_status_values); i++)
	{
		if (csstrnlen(g_status_values[i].status_type, 0x100))
		{
			c_static_string<0x100> status;
			status.print("    %s: %s", g_status_values[i].status_type, g_status_values[i].status_data);
			if (file)
			{
				file_write(file, status.length(), status.get_string());
				file_write(file, 2, "\r\n");
			}
		}
	}
}

void main_halt_and_catch_fire()
{
	static c_static_string<0x6000> text{};
	static bool x_recursion_lock = false;
	static bool x_fatal_recursion_lock = false;
	//static bool x_just_upload_dammit = false;
	static bool x_return_to_application = false;

	main_loop_pregame_halt_and_catch_fire_push();
	if (x_recursion_lock)
	{
		if (!x_fatal_recursion_lock)
		{
			x_fatal_recursion_lock = true;
			event(_event_warning, "crash: recursion lock triggered!");
			system_exit();
		}

		event(_event_critical, "crash: ### CATASTROPHIC ERROR: halt_and_catch_fire: recursion lock triggered while exiting! (Someone probably smashed memory all to bits)");
		while (!is_debugger_present());
		abort();
	}

	main_loop_pregame_halt_and_catch_fire_push();
	release_locks_safe_for_crash_release();
	//crash_report_folder_create_if_not_present();
	bool crash_ui_enable = true;//network_configuration_is_crash_ui_enabled() || minidump_force_regular_minidump_with_ui;

	ulong lock_time = system_milliseconds();

	//bool upload_debug_started = false;
	//bool upload_debug_completed = false;
	//bool upload_debug_success = false;
	bool create_fake_minidump = false;

	x_recursion_lock = true;

	event(_event_warning, "lifecycle: CRASH");
	main_status("system_milliseconds", "time %d", system_milliseconds());
	main_status_dump(NULL);

	font_initialize_emergency();
	//online_process_debug_output_queue();
	transport_initialize();
	input_clear_all_rumblers();
	progress_set_default_callbacks(NULL);
	//saved_film_manager_close();
	c_online_lsp_manager::get()->go_into_crash_mode();
	//network_webstats_force_reset();
	//user_interface_networking_set_ui_upload_quota(NONE);
	console_close();

	//if (x_just_upload_dammit)
	//{
	//	main_crash_just_upload_dammit();
	//}

	//c_static_string<256> reason;
	//bool upload_to_server = game_state_debug_server_file_uploading_enabled(&reason);
	//if (upload_to_server)
	//{
	//	//c_datamine datamine(1, "crash", 0);
	//	//datamine.commit();
	//	//data_mine_upload();
	//	upload_debug_started = upload_debug_start();
	//	create_fake_minidump = false;
	//	if (g_fake_minidump_creation)
	//	{
	//		create_fake_minidump = true;
	//	}
	//	else if (!g_suppress_upload_debug && !g_suppress_keyboard_for_minidump)
	//	{
	//		//create_fake_minidump = online_guide_show_virtual_keyboard_ui(...) == 997;
	//	}
	//}

	while (!is_debugger_present())
	{
		RENDER_THREAD_LOCK;

		ulong this_loop_time = system_milliseconds();
		ulong time_delta = this_loop_time - lock_time;
		real shell_seconds_elapsed = time_delta / 1000.0f;
		lock_time = this_loop_time;

		if (crash_ui_enable)
		{
			c_rasterizer::begin_frame();
			c_rasterizer::setup_targets_simple();
		}

		text.set(events_get());

		//char upload_debug_output[1024]{};
		//if (upload_debug_started && upload_debug_get_output(upload_debug_output, 1024))
		//{
		//	text.append(upload_debug_output);
		//}

		//if (create_fake_minidump)
		//{
		//	if (g_fake_minidump_creation)
		//	{
		//		create_fake_minidump = false;
		//		c_static_string<256> description = "crash fast";
		//		crash_user_input_upload_now(description.get_string(), description.length() + 1);
		//	}
		//	else if()
		//	{
		//		create_fake_minidump = false;
		//		if (!)
		//		{
		//			long description_length = 0;
		//			char description[256]{};
		//			wchar_string_to_ascii_string(description_wide, description, 256, &description_length);
		//			if (description_length > 1)
		//				crash_user_input_upload_now(description, description_length);
		//		}
		//	}
		//}

		e_main_pregame_frame pregame_frame_type = _main_pregame_frame_crash_done;
		//e_main_pregame_frame pregame_frame_type = _main_pregame_frame_crash_uploading;
		//if (upload_to_server)
		//{
		//	if (upload_debug_completed && upload_debug_success)
		//	{
		//		pregame_frame_type = _main_pregame_frame_crash_done;
		//	}
		//	else if (!upload_debug_completed)
		//	{
		//		if (upload_debug_started)
		//		{
		//			if ((system_seconds() & 1) != 0)
		//			{
		//				text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE\r\nWE NEED THE CRASH LOGS!1!!");
		//			}
		//			else
		//			{
		//				text.append("\r\n\r\nEACH TIME YOU REBOOT A CRASH\r\n  A BABY KITTEN DIES!1!!");
		//			}
		//		}
		//	}
		//	else if ((system_seconds() & 1) != 0)
		//	{
		//		text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE!!!\r\nUPLOAD FAILED TO START!!!");
		//	}
		//	else
		//	{
		//		text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE!!!\r\nGET A TESTER!!!");
		//	}
		//}
		//else
		//{
		//	reason.copy_to(text.get_buffer(), text.element_count);
		//	upload_debug_completed = true;
		//	upload_debug_success = true;
		//}

		if (crash_ui_enable)
		{
			main_render_pregame(pregame_frame_type, text.get_string());
			c_rasterizer::end_frame();
		}

		overlapped_update();
		input_update();
		terminal_update(shell_seconds_elapsed);
		//online_lsp_update();
		//data_mine_update();
		//network_webstats_update();
		transport_global_update();
		//online_process_debug_output_queue();

		//if (upload_debug_started && !upload_debug_completed)
		//{
		//	upload_debug_completed = upload_debug_complete(&upload_debug_success);
		//}

		sleep(0);
	}

	// by setting the `emergency_mode` to `false` we enable fonts again?
	{
		c_font_cache_scope_lock scope_lock{};
		g_font_globals.emergency_mode = false;
	}

	//while (!x_return_to_application)
	//{
	//	__debugbreak();
	//}

	x_recursion_lock = false;

	main_loop_pregame_halt_and_catch_fire_pop();
}

void main_loop_pregame_halt_and_catch_fire_pop()
{
	if (is_main_thread())
	{
		main_globals.main_loop_pregame_entered--;
	}
}

void main_loop_pregame_halt_and_catch_fire_push()
{
	if (is_main_thread())
	{
		main_globals.main_loop_pregame_entered++;
	}
}

c_wait_for_render_thread::c_wait_for_render_thread(const char* file, long line)
	: m_flags(_internal_halt_render_thread_and_lock_resources(file, line))
{
}

c_wait_for_render_thread::~c_wait_for_render_thread()
{
	unlock_resources_and_resume_render_thread(m_flags);
}

void __fastcall unlock_resources_and_resume_render_thread(ulong flags)
{
	INVOKE(0x94E00, unlock_resources_and_resume_render_thread, flags);
}