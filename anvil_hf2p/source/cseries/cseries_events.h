#pragma once
#include <cseries\cseries.h>
#include <memory\read_write_lock.h>
#include <math\real_math.h>

enum e_event_level
{
	_event_verbose = 0,
	_event_status,
	_event_message,
	_event_warning,
	_event_error,
	_event_critical,

	k_event_level_count,

	k_event_level_none = NONE,
};

enum e_category_properties_flags
{
	_category_properties_display_level_bit = 0,
	_category_properties_force_display_level_bit,
	_category_properties_log_level_bit,
	_category_properties_remote_log_level_bit, // datamine
	_category_properties_debugger_break_level_bit,
	_category_properties_halt_level_bit,
	_category_properties_event_listener_bit,

	k_category_properties_flags_count
};

enum
{
	k_error_message_buffer_maximum_size = 2048,
	k_spamming_event_display_timeout = 3000,
};

struct s_spamming_event
{
	ulong last_spam_time;
	long hit_count;
	char spam_text[2048];
	bool valid;
};
static_assert(sizeof(s_spamming_event) == 0x80C);

struct s_event_category_default_configuration
{
	const char* name;
	e_event_level initial_display_level;
	real_rgb_color initial_display_color;
	e_event_level initial_log_level;
	const char* log_name;
	void(__cdecl* log_format_func)(char*, long);
	e_event_level initial_remote_log_level;
};
static_assert(sizeof(s_event_category_default_configuration) == 0x24);

struct s_event_category
{
	short depth;
	char name[64];
	long event_log_index;
	e_event_level current_display_level;
	real_rgb_color current_display_color;
	ulong last_event_time;
	long possible_spam_event_count;
	e_event_level current_log_level;
	char log_name[256];
	void(__cdecl* log_format_func)(char*, long);
	e_event_level current_remote_log_level;
	e_event_level current_debugger_break_level;
	e_event_level current_halt_level;
	e_event_level current_force_display_level;
	ulong event_listeners;
	long parent_index;
	long first_child_index;
	long sibling_index;
};
static_assert(sizeof(s_event_category) == 0x188);

class c_event_listener_base
{
	char m_categories[128];
	long m_category_index;
	long m_event_listener_index;
};
static_assert(sizeof(c_event_listener_base) == 0x88);

class c_event_listener : 
	public c_event_listener_base
{
public:
	virtual void handle_event(e_event_level, const char*) = 0;
};
static_assert(sizeof(c_event_listener) == sizeof(c_event_listener_base) + sizeof(void*));

struct s_event_globals
{
	e_event_level current_display_level;
	e_event_level current_log_level;
	e_event_level current_remote_log_level;
	e_event_level current_minimum_level;
	e_event_level current_minimum_category_level;
	c_static_array<s_event_category, 1024> categories;
	long category_count;
	long console_suppression_old_time;
	long console_suppression_count;
	long console_suppression_old_line_check_time;
	short message_buffer_size;
	char message_buffer[k_error_message_buffer_maximum_size];
	long external_primary_event_log_index;
	long internal_primary_event_log_index;
	long internal_primary_full_event_log_index;
	long subfolder_internal_primary_event_log_index;
	long subfolder_internal_primary_full_event_log_index;
	long event_index;
	c_static_array<c_event_listener*, 8> event_listeners;
	c_static_array<s_spamming_event, 64> spamming_event_list;
	ulong last_console_response_event_time;
	bool enable_events;
	bool enable_spam_suppression;
	bool dump_to_stderr;
	bool disable_event_suppression;
	bool disable_event_log_trimming;
	bool disable_event_logging;
	bool suppress_console_display_and_show_spinner;
	long permitted_thread_bits;
};
static_assert(sizeof(s_event_globals) == 0x82B70);

class c_event
{
public:
	c_event(e_event_level event_level, long event_category_index, ulong event_response_suppress_flags);

	bool query();
	long generate(const char* format, ...);

protected:
	e_event_level m_event_level;
	long m_event_category_index;
	ulong m_event_response_suppress_flags;
};
static_assert(sizeof(c_event) == 0xC);

struct s_event_context
{
	char type[64];
	char description[128];
	bool display_to_console;
};
static_assert(sizeof(s_event_context) == 0xC1);

inline thread_local bool g_recursion_lock = false;
inline thread_local long g_event_context_stack_depth = 0;
inline thread_local long g_event_context_stack_failure_depth = 0;
inline thread_local s_event_context g_event_context_stack[32]{};

extern s_event_globals event_globals;
extern bool g_events_initialized;
extern bool g_events_initializing_cookie;
extern bool events_force_no_log;
extern c_read_write_lock g_event_read_write_lock;

extern const char* const k_event_level_names[k_event_level_count + 1];
extern const char* const k_event_level_severity_strings[k_event_level_count];
extern const char* const k_primary_event_log_filename;
extern const char* const k_primary_full_event_log_filename;

extern bool g_events_debug_render_enable;

struct s_file_reference;
extern s_file_reference* __cdecl create_report_file_reference(s_file_reference* info, const char* filename, bool use_sub_directory);
extern void events_clear();
extern void events_debug_render();
extern void events_dispose();
extern const char* events_get();
extern void events_initialize();
extern long event_interlocked_compare_exchange(long volatile* destination, long exchange, long comperand);
extern void event_logs_flush();
extern void events_suppress_output(bool suppress);

//#define USE_CONSOLE_FOR_EVENTS

#if defined(EVENTS_ENABLED)

#ifdef USE_CONSOLE_FOR_EVENTS
#define event(severity, ...) do { c_console::write_line(__VA_ARGS__); } while (false)
#define event_no_console(severity, ...) do { /* $TODO: implement me */ } while (false)
#else
#define event(severity, ...) \
do { \
	static long volatile x_event_category_index = NONE; \
	c_event local_event(severity, x_event_category_index, 0); \
	if (local_event.query()) \
	{ \
		long event_category_index = local_event.generate(__VA_ARGS__); \
		if (x_event_category_index == NONE) \
		{ \
			event_interlocked_compare_exchange(&x_event_category_index, event_category_index, NONE); \
		} \
	} \
} while (false)
#define event_no_console(severity, ...) do { /* $TODO: implement me */ } while (false)
#endif

#else
#define event(severity, ...) do { } while (false)
#define event_no_console(severity, ...) do { } while (false)
#endif

