#include "unicode.h"
#include <cseries\cseries.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int ustrcmp(wchar_t const* string1, wchar_t const* string2)
{
    assert(string1 != NULL);
    assert(string2 != NULL);
    return wcscmp(string1, string2);
}

unsigned int ustrlen(wchar_t const* string)
{
    assert(string != NULL);
    return wcslen(string);
}

unsigned int ustrnlen(wchar_t const* string, long count)
{
    assert(string != NULL);
    return wcsnlen(string, count);
}

//wchar_t const * ustrchr(wchar_t const *,wchar_t)
//int ustrcoll(wchar_t const *,wchar_t const *)
//unsigned int ustrcspn(wchar_t const *,wchar_t const *)

wchar_t* ustrnzcat(wchar_t* dest, wchar_t const* src, long count)
{
    wcsncat_s(dest, count - 1, src, count);
    return dest;
}

int ustrncmp(wchar_t const* string1, wchar_t const* string2, long count)
{
    assert(string1 != NULL);
    assert(string2 != NULL);

    return wcsncmp(string1, string2, count);
}

int ustrncpy(wchar_t* dest, wchar_t const* src, long count)
{
    assert(dest != NULL);
    assert(src != NULL);

    for (long i = 0; i <= count; i++)
    {
        wchar_t chr = src[i];
        dest[i] = chr;
        if (chr == L'\0')
            break;
    }
    return 0;
    //return wcsncpy_s(dest, size, src, count);
    // originally used wcsncpy_s, but it would fill the buffer past \0 with 0xFE garbage markers, which caused the membership update checksum to fail
    // the above implementation is what saber used in ms23
}

wchar_t* ustrnzcpy(wchar_t* dest, wchar_t const* src, long count)
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(count > 0);

    ustrncpy(dest, src, count - 1);
    dest[count - 1] = 0;
    return dest;
}

//wchar_t const * ustrpbrk(wchar_t const *,wchar_t const *)
//wchar_t const * ustrrchr(wchar_t const *,wchar_t)
//unsigned int ustrspn(wchar_t const *,wchar_t const *)
//wchar_t const * ustrstr(wchar_t const *,wchar_t const *)
//wchar_t * ustrtok(wchar_t *,wchar_t const *)
//unsigned int ustrxfrm(wchar_t *,wchar_t const *,long)
//wchar_t * ustrnlwr(wchar_t *,long)
//wchar_t * ustrnupr(wchar_t *,long)

//int ustricmp(wchar_t const* a1, wchar_t const* a2)

//void ascii_string_to_wchar_string(char const* src, wchar_t* dest, long src_len, long* out_dest_len)

int ustrnicmp(wchar_t const* string1, wchar_t const* string2, long count)
{
    return _wcsnicmp(string1, string2, count);
}

//int uisalpha(wchar_t)
//int uisupper(wchar_t)
//int uislower(wchar_t)
//int uisdigit(wchar_t)
//int uisxdigit(wchar_t)
//int uisspace(wchar_t)
//int uispunct(wchar_t)
//int uisalnum(wchar_t)
//int uisprint(wchar_t)
//int uisgraph(wchar_t)
//int uiscntrl(wchar_t)
//wchar_t utoupper(wchar_t)
//int utolower(wchar_t)
//wchar_t ufgetc(struct _iobuf *)
//wchar_t ufputc(wchar_t,struct _iobuf *)
//wchar_t uungetc(wchar_t,struct _iobuf *)
//wchar_t * ufgets(wchar_t *,int,struct _iobuf *)
//int ufputs(wchar_t const *,struct _iobuf *)
//wchar_t * ugets(wchar_t *)
//int uputs(wchar_t const *)
//int ufprintf(struct _iobuf *,wchar_t const *,...)
//int uprintf(wchar_t const *,...)
//int usnprintf(wchar_t *,long,wchar_t const *,...)

int usnzprintf(wchar_t* string, long size, wchar_t const* format, ...)
{
    va_list list;
    va_start(list, format);

    int result = uvsnzprintf(string, size, format, list);

    va_end(list);
    return result;
}

//int uvfprintf(struct _iobuf *,wchar_t const *,char *)
//int uvprintf(wchar_t const *,char *)

int uvsnzprintf(wchar_t* string, long size, wchar_t const* format, va_list list)
{
    assert(string && format);
    assert(size > 0);

    long result = _vsnwprintf_s(string, size, size - 1, format, list);
    string[size - 1] = 0;

    return result;
}

//struct _iobuf * ufdopen(int,wchar_t const *)
//struct _iobuf * ufopen(wchar_t const *,wchar_t const *)
//int ufclose(struct _iobuf *)
//struct _iobuf * ufreopen(wchar_t const *,wchar_t const *,struct _iobuf *)
//void uperror(wchar_t const *)
//int uremove(wchar_t const *)
//wchar_t * utmpnam(wchar_t *)
//long ustrtol(wchar_t const *,wchar_t * *,int)
//unsigned long ustrtoul(wchar_t const *,wchar_t * *,int)
//double ustrtod(wchar_t const *,wchar_t * *)
//wchar_t * ustrftime_tm(wchar_t *,long,wchar_t const *,struct tm const *)
//wchar_t * ustrftime(wchar_t *,long,wchar_t const *,__int64)
//int uatoi(wchar_t const *)

void wchar_string_to_ascii_string(wchar_t const* src, char* dst, long source_length, long* destination_length)
{
    long new_length = 0;
    wchar_t current_char = L'\0';
    assert(((void*)src) != ((void*)dst));
    do
    {
        if (source_length <= 0)
            break;
        current_char = *(wchar_t*)src;
        if (source_length == 1)
            *dst = 0;
        else
            *dst = current_char > 0x7F ? '?' : (byte)current_char; // 0x7F is the size of the ascii table
        src = (const wchar_t*)((char*)src + 2);
        ++dst;
        --source_length;
        ++new_length;
    }
    while (current_char);
    if (destination_length)
        *destination_length = new_length;
}

//struct utf32 ascii_string_to_utf32_characters(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *)
//struct utf32 wchar_string_to_utf32_characters(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *)
//void ascii_string_to_utf32_string(char const *,struct s_escape_table const *,struct utf32 *,long,long *)
//void wchar_string_to_utf32_string(wchar_t const *,struct s_escape_table const *,struct utf32 *,long,long *)
//void ascii_string_to_wchar_string(char const *,wchar_t *,long,long *)
//long utf32_character_to_utf16_string(struct utf32,struct utf16 *,long)
//struct utf32 utf16_string_to_utf32_character(struct utf16 const *,struct utf16 const * *)
//bool utf32_in_list(struct utf32,struct s_utf32_range const *,long)
//bool utf32_isspace(struct utf32)
//bool utf32_is_id_start(struct utf32)
//bool utf32_is_id_continue(struct utf32)
//bool utf32_isalpha(struct utf32)
//bool utf32_isprint(struct utf32)
//bool utf32_ismonochrome(struct utf32)
//bool utf32_can_line_break(struct utf32,struct utf32)
//unsigned int utf32_strlen(struct utf32 const *)
//bool is_private_use_character(wchar_t)
//void utf8_string_to_wchar_string(struct utf8 const *,wchar_t *,long,long *)
//void wchar_string_to_utf8_string(wchar_t const *,struct utf8 *,long,long *)
//void string_to_utf32_string<char,8>(char const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *),long,long *)
//void string_to_utf32_string<wchar_t,8>(wchar_t const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *),long,long *)