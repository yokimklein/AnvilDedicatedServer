#pragma once

#include "cseries/cseries.h"
#include "interface/terminal.h"
#include "hs/hs_enum.h"

enum e_status_line_flags
{
	_status_line_blink_bit,
	_status_line_inhibit_drawing_bit,
	_status_line_left_justify_bit,
	_status_line_draw_once_bit,

	k_status_line_count
};

class c_draw_string;

struct s_console_globals
{
	bool active;
	bool status_render;
	real open_timeout_seconds;
	terminal_gets_state input_state;
	c_static_string<255> previous_commands[16];
	short previous_command_count;
	short newest_previous_command_index;
	short selected_previous_command_index;
};
//static_assert(sizeof(s_console_globals) == 0x1204);

struct s_console_global
{
	const char* name;

	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;
	void* pointer;
};
static_assert(sizeof(s_console_global) == 0xC);

struct s_status_line
{
	s_status_line();

	c_static_string<256> text;
	real_rgb_color color;
	real alpha;
	c_flags<e_status_line_flags, ulong, k_status_line_count> flags;
	bool* in_use;
	const char* identifier;
	s_status_line* prev;
	s_status_line* next;
};
static_assert(sizeof(s_status_line) == 0x124);

struct s_status_string
{
	s_status_string();

	c_static_string<256> format_string;
	long time_created;
	s_status_line line;
};
static_assert(sizeof(s_status_string) == 0x228);

enum e_text_justification;
struct s_string_cache
{
	s_string_cache();

	c_static_string<4096> string;
	real alpha;
	real_rgb_color color;
	e_text_justification justification;
};
static_assert(sizeof(s_string_cache) == 0x1014);

extern s_console_globals console_globals;

extern s_console_global const k_console_globals[];
extern long const k_console_global_count;

extern bool console_dump_to_debug_display;

extern s_status_line* g_status_line_head;
extern s_status_line* g_status_line_tail;

extern s_status_string g_status_strings[20];

class c_draw_string;
class c_font_cache_base;

enum e_status_line_flags;
enum e_text_justification;

extern void __cdecl console_clear();
extern void __cdecl console_close();
extern void __cdecl console_complete();
extern void __cdecl console_dispose();
extern void __cdecl console_execute_initial_commands();
extern void __cdecl console_initialize();
extern bool __cdecl console_is_active();
extern bool __cdecl console_is_empty();
extern void __cdecl console_open(bool debug_menu);
extern void __cdecl console_printf(const char* format, ...);
extern void __cdecl console_printf_color(const real_argb_color* color, const char* format, ...);
extern bool __cdecl console_process_command(const char* command, bool interactive);
extern void __cdecl console_update(real shell_seconds_elapsed);
extern void __cdecl console_warning(const char* format, ...);

extern bool __cdecl debugging_system_has_focus();

extern void status_line_add_single(s_status_line* status_line);
extern void status_line_draw();
extern void status_line_dump();
extern void status_line_remove_single(s_status_line* status_line);
extern bool status_line_visible(const s_status_line* status_line);

extern void status_lines_clear_text(s_status_line* status_lines, long count);
extern void status_lines_disable(const char* identifier);
extern void status_lines_dispose(s_status_line* status_lines, long count);
extern void status_lines_enable(const char* identifier);
extern void status_lines_initialize(s_status_line* status_lines, bool* flag, long count);
extern void status_lines_initialize_simple(s_status_line* status_lines, bool* flag, const char* identifier, long count);
extern void status_lines_set_flags(s_status_line* status_lines, e_status_line_flags flag, bool enable, long count);

extern void status_printf(const char* format, ...);
extern void status_printf_va(const char* format, char* list);

extern void status_string_internal(const char* status, const char* message);
extern void status_strings(const char* status, const char* strings);

extern bool string_cache_add(s_string_cache* cache, const char* string, real alpha, const real_rgb_color& color, e_text_justification justification);
extern void string_cache_render(s_string_cache* string_cache, c_draw_string* draw_string, c_font_cache_base* font_cache);
