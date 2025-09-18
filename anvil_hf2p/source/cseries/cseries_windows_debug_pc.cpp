#include "cseries_windows_debug_pc.h"
#include "cseries\cseries.h"
#include "main\main.h"
#include "cseries\cseries_events.h"
#include "cseries\cseries_event_logs.h"
#include "cseries\version.h"
#include "editor\editor_stubs.h"
#include "rasterizer\rasterizer.h"
#include "cseries\stack_walk_windows.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dbghelp.h>
#include <float.h>
#include <Psapi.h>

struct s_exception_type_info
{
	const char* exception_string;
	ulong exception_parameters[3];
};
static_assert(sizeof(s_exception_type_info) == 0x10);

struct s_exception_information
{
	c_synchronized_long exception_occurred;
	long thread_id;
	CONTEXT context_record;
	ulong exception_code;
	ulong exception_flags;
	void* exception_address;
	ulong number_parameters;
	union
	{
		s_exception_type_info exception_type_info;
		s_thread_assert_arguments thread_assert_arguments;
	};
};
static_assert(sizeof(s_exception_information) == 0x2F4);

bool g_set_always_a_debugger_present = false;
bool g_set_never_a_debugger_present = false;
REFERENCE_DECLARE(0x106DECC, ulong, g_exception_time);
//REFERENCE_DECLARE(0x0, PEXCEPTION_POINTERS, g_exception_pointers);
REFERENCE_DECLARE(0x40274B8, c_synchronized_long, g_exception_caching_in_progress);
REFERENCE_DECLARE(0x40274BC, s_exception_information, g_exception_information);

const char* const k_screenshot_file = "crash_report\\crash_screenshot.bmp";

long __fastcall exceptions_update()
{
	if (!has_cached_exception())
	{
		return 0;
	}

	main_loop_pregame_disable(true);

	ulong code = g_exception_information.exception_code;
	const char* exception_code_string = exception_code_get_string(g_exception_information.exception_code);
	if (!g_catch_exceptions)
	{
		g_exception_information.exception_occurred = false;
		main_loop_pregame_disable(false);
		return 0;
	}

	if (code == 'stk')
	{
		if (version_is_tracked_build() || g_force_upload_even_if_untracked)
		{
			rasterizer_dump_display_to_bmp(k_screenshot_file);
			release_locks_safe_for_crash_release();
		}

		g_exception_information.exception_occurred = false;
		main_loop_pregame_disable(false);
		return -1;
	}

	c_static_string<1156> crash_info;
	const char* thread_name = get_thread_name_from_thread_id(g_exception_information.thread_id);
	long thread_id = g_exception_information.thread_id;

	_clearfp();
	event_logs_flush();
	release_locks_safe_for_crash_release();

	event(_event_message, "crash: ");
	event(_event_message, "crash: FATAL ERROR ENCOUNTERED");
	event(_event_message, "crash: ");
	event(_event_message, "crash: ");

	ulong exception_address = (ulong)g_exception_information.exception_address;
	const char* symbol_name = symbol_name_from_address(exception_address, NULL);
	event(_event_message, "crash: %s",
		version_get_full_string());

	crash_info.print("version:\r\n%s\r\n",
		version_get_full_string());

	event(_event_message, "crash:   thread information: thread_name: %s thread_id: %08lx",
		thread_name,
		thread_id);
	crash_info.append_print("thread information:\r\n thread_name: %s, thread_id: %08lx\r\n",
		thread_name,
		thread_id);

	if (code == 'asrt' && g_exception_information.number_parameters >= 4)
	{
		const char* exception_string = g_exception_information.thread_assert_arguments.statement;
		const char* file = g_exception_information.thread_assert_arguments.file;
		long line = g_exception_information.thread_assert_arguments.line;
		bool assertion_failed = g_exception_information.thread_assert_arguments.fatal;

		event(_event_message, "crash: %s at %s,#%d",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		crash_info.append_print("halt:\r\n%s at %s,#%d\r\n",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		if (exception_string)
		{
			event(_event_message, "crash:   %s",
				exception_string);
			crash_info.append_print("halt information:\r\n  %s\r\n",
				exception_string);
		}
	}
	else
	{
		if (exception_code_string)
		{
			event(_event_message, "crash: ### RUNTIME ERROR: %s at %08lX",
				exception_code_string,
				exception_address);
			event(_event_message, "crash:   (%s)",
				symbol_name);

			crash_info.append_print("halt:\r\n### RUNTIME ERROR: %s at %08lX\r\n",
				exception_code_string,
				exception_address);
			crash_info.append_print("halt information:\r\n  (%s)\r\n",
				symbol_name);
		}
		else
		{
			event(_event_message, "crash: ### RUNTIME ERROR: UNKNOWN EXCEPTION %08lX at %08lX",
				exception_code_string,
				exception_address);
			event(_event_message, "crash:   (%s)",
				symbol_name);

			crash_info.append_print("halt:\r\n### RUNTIME ERROR: UNKNOWN EXCEPTION %08lX at %08lX\r\n",
				code,
				exception_address);
			crash_info.append_print("halt information:\r\n  (%s)\r\n",
				symbol_name);
		}

		if (code == EXCEPTION_ACCESS_VIOLATION)
		{
			if (g_exception_information.number_parameters >= 2)
			{
				event(_event_message, "crash:   tried to %s address %08lx",
					g_exception_information.exception_type_info.exception_string ? "write" : "read",
					g_exception_information.exception_type_info.exception_parameters[0]);

				crash_info.append_print("crash:   tried to %s address %08lx\r\n",
					g_exception_information.exception_type_info.exception_string ? "write" : "read",
					g_exception_information.exception_type_info.exception_parameters[0]);
			}
		}
		else if (code == 0xC06D007E && g_exception_information.number_parameters) // VCPPEXCEPTION_MODULE_NOT_FOUND
		{
			event(_event_message, "crash:   tried to load %s",
				*((const char**)g_exception_information.exception_type_info.exception_string + 3));
			crash_info.append_print("  tried to load %s\r\n",
				*((const char**)g_exception_information.exception_type_info.exception_string + 3));
		}
	}

	main_write_stack_to_crash_info_status_file(crash_info.get_string(), &g_exception_information.context_record);
	editor_save_progress();
	//call_fatal_error_callbacks();
	if (version_is_tracked_build() || g_force_upload_even_if_untracked)
	{
		rasterizer_dump_display_to_bmp(k_screenshot_file);
		release_locks_safe_for_crash_release();
	}

	if (is_debugger_present())
	{
		__debugbreak();
	}
	else
	{
		main_halt_and_catch_fire();
	}

	long result = !is_debugger_present() ? 1 : -1;

	g_exception_information.exception_occurred = false;
	main_loop_pregame_disable(false);

	return result;
}

bool has_cached_exception()
{
	return g_exception_information.exception_occurred.peek() != 0;
}

const char* __fastcall exception_code_get_string(ulong code)
{
	return INVOKE(0x167E80, exception_code_get_string, code);
}

bool is_debugger_present()
{
	if (g_set_always_a_debugger_present)
	{
		return true;
	}

	if (g_set_never_a_debugger_present)
	{
		return false;
	}

	return IsDebuggerPresent();
}

void cache_exception_information(_EXCEPTION_POINTERS* exception_pointers)
{
	if (!g_exception_caching_in_progress.set(1) && !has_cached_exception())
	{
		build_exception_information(exception_pointers, &g_exception_information);
		build_exception_pointers(exception_pointers); // new since ms23
		g_exception_information.exception_occurred = true;
		g_exception_time = system_milliseconds();
	}
}

void crashdump_from_exception()
{
	INVOKE(0x2B4650, crashdump_from_exception);
}

long generic_exception_filter(ulong exception_code, _EXCEPTION_POINTERS* exception_pointers)
{
	cache_exception_information(exception_pointers);
	return exceptions_update();
}

void build_exception_information(_EXCEPTION_POINTERS* exception_pointers, s_exception_information* exception_information)
{
	if (exception_information)
	{
		csmemset(exception_information, 0, sizeof(s_exception_information));
		if (exception_pointers)
		{
			exception_information->thread_id = GetCurrentThreadId();
			csmemcpy(&exception_information->context_record, exception_pointers->ContextRecord, sizeof(exception_information->context_record));
			exception_information->exception_code = exception_pointers->ExceptionRecord->ExceptionCode;
			exception_information->exception_flags = exception_pointers->ExceptionRecord->ExceptionFlags;
			exception_information->exception_address = exception_pointers->ExceptionRecord->ExceptionAddress;
			if (exception_pointers->ExceptionRecord != (PEXCEPTION_RECORD)-0x14)
			{
				exception_information->number_parameters = min((sizeof(s_exception_type_info) / sizeof(ULONG_PTR)), exception_pointers->ExceptionRecord->NumberParameters);
				csmemcpy(&exception_information->exception_type_info, exception_pointers->ExceptionRecord->ExceptionInformation, (exception_information->number_parameters * sizeof(ULONG_PTR)));
			}
			exception_information->exception_occurred = true;
		}
	}
}

void __fastcall build_exception_pointers(_EXCEPTION_POINTERS* exception_pointers)
{
	// $TODO: new since ms23, need to look into this further
	INVOKE(0x167BD0, build_exception_pointers, exception_pointers);
}
