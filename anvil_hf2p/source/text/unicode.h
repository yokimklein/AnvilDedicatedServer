#pragma once
#include "cseries\cseries.h"
#include <stdarg.h>

// Taken from https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/text/unicode.hpp

extern long ustrcmp(wchar_t const* string1, wchar_t const* string2);
extern ulong ustrlen(wchar_t const* string);
extern ulong ustrnlen(wchar_t const* string, ulong count);
//extern wchar_t const * ustrchr(wchar_t const *,wchar_t);
//extern int ustrcoll(wchar_t const *,wchar_t const *);
//extern unsigned int ustrcspn(wchar_t const *,wchar_t const *);
extern wchar_t* ustrnzcat(wchar_t* dest, wchar_t const* src, ulong count);
extern long ustrncmp(wchar_t const* string1, wchar_t const* string2, ulong count);
extern long ustrncpy(wchar_t* dest, wchar_t const* src, ulong count);
extern wchar_t* ustrnzcpy(wchar_t* dest, wchar_t const* src, ulong count);
//extern wchar_t const * ustrpbrk(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrrchr(wchar_t const *,wchar_t);
//extern unsigned int ustrspn(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrstr(wchar_t const *,wchar_t const *);
//extern wchar_t * ustrtok(wchar_t *,wchar_t const *);
//extern unsigned int ustrxfrm(wchar_t *,wchar_t const *,long);
//extern wchar_t * ustrnlwr(wchar_t *,long);
//extern wchar_t * ustrnupr(wchar_t *,long);
//extern int ustricmp(wchar_t const*, wchar_t const*);
extern long ustrnicmp(wchar_t const* string1, wchar_t const* string2, ulong count);
//extern int uisalpha(wchar_t);
//extern int uisupper(wchar_t);
//extern int uislower(wchar_t);
//extern int uisdigit(wchar_t);
//extern int uisxdigit(wchar_t);
//extern int uisspace(wchar_t);
//extern int uispunct(wchar_t);
//extern int uisalnum(wchar_t);
//extern int uisprint(wchar_t);
//extern int uisgraph(wchar_t);
//extern int uiscntrl(wchar_t);
//extern wchar_t utoupper(wchar_t);
//extern int utolower(wchar_t);
//extern wchar_t ufgetc(struct _iobuf *);
//extern wchar_t ufputc(wchar_t,struct _iobuf *);
//extern wchar_t uungetc(wchar_t,struct _iobuf *);
//extern wchar_t * ufgets(wchar_t *,int,struct _iobuf *);
//extern int ufputs(wchar_t const *,struct _iobuf *);
//extern wchar_t * ugets(wchar_t *);
//extern int uputs(wchar_t const *);
//extern int ufprintf(struct _iobuf *,wchar_t const *,...);
//extern int uprintf(wchar_t const *,...);
//extern int usnprintf(wchar_t *,long,wchar_t const *,...);
extern long usnzprintf(wchar_t* string, long size, wchar_t const* format, ...);
//extern int uvfprintf(struct _iobuf *,wchar_t const *,char *);
//extern int uvprintf(wchar_t const *,char *);
extern long uvsnzprintf(wchar_t* string, long size, wchar_t const* format, va_list list);
//extern struct _iobuf * ufdopen(int,wchar_t const *);
//extern struct _iobuf * ufopen(wchar_t const *,wchar_t const *);
//extern int ufclose(struct _iobuf *);
//extern struct _iobuf * ufreopen(wchar_t const *,wchar_t const *,struct _iobuf *);
//extern void uperror(wchar_t const *);
//extern int uremove(wchar_t const *);
//extern wchar_t * utmpnam(wchar_t *);
//extern long ustrtol(wchar_t const *,wchar_t * *,int);
//extern unsigned long ustrtoul(wchar_t const *,wchar_t * *,int);
//extern double ustrtod(wchar_t const *,wchar_t * *);
//extern wchar_t * ustrftime_tm(wchar_t *,long,wchar_t const *,struct tm const *);
//extern wchar_t * ustrftime(wchar_t *,long,wchar_t const *,__int64);
//extern int uatoi(wchar_t const *);

extern void wchar_string_to_ascii_string(wchar_t const* src, char* dst, long source_length, long* destination_length);
//extern struct utf32 ascii_string_to_utf32_characters(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *);
//extern struct utf32 wchar_string_to_utf32_characters(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *);
//extern void ascii_string_to_utf32_string(char const *,struct s_escape_table const *,struct utf32 *,long,long *);
//extern void wchar_string_to_utf32_string(wchar_t const *,struct s_escape_table const *,struct utf32 *,long,long *);
extern void ascii_string_to_wchar_string(char const* src, wchar_t* dest, long src_len, long* out_dest_len);
extern void char_string_to_wchar_string(wchar_t* dest, const char* src);
//extern long utf32_character_to_utf16_string(struct utf32,struct utf16 *,long);
//extern struct utf32 utf16_string_to_utf32_character(struct utf16 const *,struct utf16 const * *);
//extern bool utf32_in_list(struct utf32,struct s_utf32_range const *,long);
//extern bool utf32_isspace(struct utf32);
//extern bool utf32_is_id_start(struct utf32);
//extern bool utf32_is_id_continue(struct utf32);
//extern bool utf32_isalpha(struct utf32);
//extern bool utf32_isprint(struct utf32);
//extern bool utf32_ismonochrome(struct utf32);
//extern bool utf32_can_line_break(struct utf32,struct utf32);
//extern unsigned int utf32_strlen(struct utf32 const *);
//extern bool is_private_use_character(wchar_t);
//extern void utf8_string_to_wchar_string(struct utf8 const *,wchar_t *,long,long *);
//extern void wchar_string_to_utf8_string(wchar_t const *,struct utf8 *,long,long *);
//extern void string_to_utf32_string<char,8>(char const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *),long,long *);
//extern void string_to_utf32_string<wchar_t,8>(wchar_t const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *),long,long *);

template<long k_buffer_size>
struct c_static_wchar_string
{
public:
	c_static_wchar_string() :
		m_string{}
	{
		clear();
	}

	c_static_wchar_string(wchar_t const* s) :
		c_static_wchar_string()
	{
		clear();
		set(s);
	}

	void set(wchar_t const* s)
	{
		ustrnzcpy(m_string, s, k_buffer_size);
	}

	void set_length(long length)
	{
		if (VALID_COUNT(length, k_buffer_size - 1))
		{
			m_string[length] = 0;
		}
	}

	void clear()
	{
		csmemset(m_string, 0, sizeof(m_string));
	}

	bool is_empty() const
	{
		return !m_string[0];
	}

	wchar_t const* get_string() const
	{
		return m_string;
	}

	wchar_t* get_buffer()
	{
		return m_string;
	}

	char const* get_offset(long offset) const
	{
		if (VALID_INDEX(offset, length()))
		{
			return &m_string[offset];
		}

		return "";
	}

	long length() const
	{
		return ustrnlen(m_string, k_buffer_size);
	}

	void append(wchar_t const* s)
	{
		ustrnzcat(m_string, s, k_buffer_size);
	}

	void append_line(wchar_t const* s = nullptr)
	{
		if (s != nullptr)
		{
			ustrnzcat(m_string, s, k_buffer_size);
		}
		ustrnzcat(m_string, L"\r\n", k_buffer_size);
	}

	wchar_t const* print(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		uvsnzprintf(m_string, k_buffer_size, format, list);

		va_end(list);

		return m_string;
	}

	wchar_t const* print_line(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		uvsnzprintf(m_string, k_buffer_size, format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	wchar_t const* print_va(wchar_t const* format, va_list list)
	{
		uvsnzprintf(m_string, k_buffer_size, format, list);

		return m_string;
	}

	wchar_t const* append_print(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		append_print_va(format, list);

		va_end(list);
		return m_string;
	}

	wchar_t const* append_print_line(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		wchar_t const* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	wchar_t const* append_print_va(wchar_t const* format, va_list list)
	{
		long current_length = length();

		//ASSERT(format);
		//ASSERT(current_length >= 0 && current_length < k_buffer_size);

		uvsnzprintf(m_string + current_length, k_buffer_size - current_length, format, list);

		return m_string;
	}

	bool is_equal(wchar_t const* s, bool case_sensitive) const
	{
		if (case_sensitive)
		{
			return ustrncmp(m_string, s, k_buffer_size) == 0;
		}

		return ustrnicmp(m_string, s, k_buffer_size) == 0;
	}

	bool is_equal(wchar_t const* s) const
	{
		return is_equal(s, true);
	}

	wchar_t* copy_to(wchar_t* s, unsigned int size)const
	{
		if (size > k_buffer_size)
		{
			size = k_buffer_size;
		}

		return ustrnzcpy(s, m_string, size);
	}

protected:
	wchar_t m_string[k_buffer_size];
};