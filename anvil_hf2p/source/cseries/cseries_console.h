#pragma once
#include <math\real_math.h>
#include <stdio.h>

class c_console
{
public:
	static void initialize(const char* window_title);
	static void dispose();
	static void toggle_window_visibility();
	static void clear();

	static void enable_ansi();

private:
	static bool console_allocated();

public:
	static void write(const real_argb_color* color, const char* format, ...);
	static void write_line(const real_argb_color* color, const char* format, ...);
	static void write(const real_argb_color* color, const wchar_t* format, ...);
	static void write_line(const real_argb_color* color, const wchar_t* format, ...);

	static void write_va(const real_argb_color* color, const char* format, va_list list);
	static void write_line_va(const real_argb_color* color, const char* format, va_list list);
	static void write_va(const real_argb_color* color, const wchar_t* format, va_list list);
	static void write_line_va(const real_argb_color* color, const wchar_t* format, va_list list);

protected:
	static bool m_initialized;
	static FILE* m_file;
};

extern void get_error_message(dword message_id, char(&message_buffer)[2048]);