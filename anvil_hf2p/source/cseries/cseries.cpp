#include "cseries.h"

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/cseries.cpp

#include <memory\byte_swapping.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <text\unicode.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// TODO: remove this
//const size_t module_base = (size_t)GetModuleHandle(NULL);

inline size_t base_address_impl(size_t address)
{
    return (size_t)GetModuleHandle(NULL) + address;
}

// ms23 addresses
//REFERENCE_DECLARE(0x0189CD54, real_argb_color const* const, global_real_argb_white);
//REFERENCE_DECLARE(0x0189CD58, real_argb_color const* const, global_real_argb_grey);
//REFERENCE_DECLARE(0x0189CD5C, real_argb_color const* const, global_real_argb_black);
//REFERENCE_DECLARE(0x0189CD60, real_argb_color const* const, global_real_argb_red);
//REFERENCE_DECLARE(0x0189CD64, real_argb_color const* const, global_real_argb_green);
//REFERENCE_DECLARE(0x0189CD68, real_argb_color const* const, global_real_argb_blue);
//REFERENCE_DECLARE(0x0189CD6C, real_argb_color const* const, global_real_argb_yellow);
//REFERENCE_DECLARE(0x0189CD70, real_argb_color const* const, global_real_argb_cyan);
//REFERENCE_DECLARE(0x0189CD74, real_argb_color const* const, global_real_argb_magenta);
//REFERENCE_DECLARE(0x0189CD78, real_argb_color const* const, global_real_argb_pink);
//REFERENCE_DECLARE(0x0189CD7C, real_argb_color const* const, global_real_argb_lightblue);
//REFERENCE_DECLARE(0x0189CD80, real_argb_color const* const, global_real_argb_orange);
//REFERENCE_DECLARE(0x0189CD84, real_argb_color const* const, global_real_argb_purple);
//REFERENCE_DECLARE(0x0189CD88, real_argb_color const* const, global_real_argb_aqua);
//REFERENCE_DECLARE(0x0189CD8C, real_argb_color const* const, global_real_argb_darkgreen);
//REFERENCE_DECLARE(0x0189CD90, real_argb_color const* const, global_real_argb_salmon);
//REFERENCE_DECLARE(0x0189CD94, real_argb_color const* const, global_real_argb_violet);
//REFERENCE_DECLARE(0x0189CD98, real_rgb_color const* const, global_real_rgb_white);
//REFERENCE_DECLARE(0x0189CD9C, real_rgb_color const* const, global_real_rgb_grey);
//REFERENCE_DECLARE(0x0189CDA0, real_rgb_color const* const, global_real_rgb_black);
//REFERENCE_DECLARE(0x0189CDA4, real_rgb_color const* const, global_real_rgb_red);
//REFERENCE_DECLARE(0x0189CDA8, real_rgb_color const* const, global_real_rgb_green);
//REFERENCE_DECLARE(0x0189CDAC, real_rgb_color const* const, global_real_rgb_blue);
//REFERENCE_DECLARE(0x0189CDB0, real_rgb_color const* const, global_real_rgb_yellow);
//REFERENCE_DECLARE(0x0189CDB4, real_rgb_color const* const, global_real_rgb_cyan);
//REFERENCE_DECLARE(0x0189CDB8, real_rgb_color const* const, global_real_rgb_magenta);
//REFERENCE_DECLARE(0x0189CDBC, real_rgb_color const* const, global_real_rgb_pink);
//REFERENCE_DECLARE(0x0189CDC0, real_rgb_color const* const, global_real_rgb_lightblue);
//REFERENCE_DECLARE(0x0189CDC4, real_rgb_color const* const, global_real_rgb_orange);
//REFERENCE_DECLARE(0x0189CDC8, real_rgb_color const* const, global_real_rgb_purple);
//REFERENCE_DECLARE(0x0189CDCC, real_rgb_color const* const, global_real_rgb_aqua);
//REFERENCE_DECLARE(0x0189CDD0, real_rgb_color const* const, global_real_rgb_darkgreen);
//REFERENCE_DECLARE(0x0189CDD4, real_rgb_color const* const, global_real_rgb_salmon);
//REFERENCE_DECLARE(0x0189CDD8, real_rgb_color const* const, global_real_rgb_violet);

REFERENCE_DECLARE(0xE9B960, c_normal_allocation*, g_normal_allocation);

int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size) = memcmp;
void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size) = memcpy;
void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size) = memset;

#define MAXIMUM_STRING_SIZE 0x100000

long csstricmp(char const* s1, char const* s2)
{
    return _stricmp(s1, s2);
}

//long csstrnicmp(char const* s1, char const* s2, dword size)

long csstrcmp(char const* s1, char const* s2)
{
    return strcmp(s1, s2);
}

extern long csstrncmp(char const* s1, char const* s2, dword size)
{
    return strncmp(s1, s2, size);
}

//char* csstristr(char const* s1, char const* s2)

char* csstrnzcpy(char* s1, char const* s2, dword size)
{
    ASSERT(s1 && s2);
    ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

    strncpy_s(s1, size, s2, size);
    s1[size - 1] = 0;

    size_t s2_size = strlen(s2);
    memset(s1 + s2_size, 0, size - s2_size);

    return s1;
}

char* csstrnzcat(char* s1, char const* s2, dword size)
{
    ASSERT(s1 && s2);
    ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

    dword len = csstrnlen(s1, size);
    return csstrnzcpy(s1 + len, s2, size - len);
}

dword csstrnlen(char const* s, dword size)
{
    ASSERT(s);
    ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

    return strnlen(s, size);
}

char* csstrnupr(char* s, dword size)
{
    ASSERT(s);
    ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

    for (dword i = 0; i < size; i++)
        s[i] = toupper(s[i]);

    return s;
}

char* csstrnlwr(char* s, dword size)
{
    ASSERT(s);
    ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

    for (dword i = 0; i < size; i++)
        s[i] = tolower(s[i]);

    return s;
}

char const* csstrstr(char const* s1, char const* s2)
{
    return strstr(s1, s2);
}

//char* csstrtok(char*, char const*, bool, struct csstrtok_data* data)

long cvsnzprintf(char* buffer, dword size, char const* format, va_list list)
{
    ASSERT(buffer);
    ASSERT(format);
    ASSERT(size > 0);

    long result = vsnprintf(buffer, size - 1, format, list);
    buffer[size - 1] = 0;

    size_t buf_size = strlen(buffer);
    memset(buffer + buf_size, 0, size - buf_size);

    return result;
}

char* csnzprintf(char* buffer, dword size, char const* format, ...)
{
    va_list list;
    va_start(list, format);

    cvsnzprintf(buffer, size, format, list);

    va_end(list);

    return buffer;
}

char* csnzappendf(char* buffer, dword size, char const* format, ...)
{
    dword current_length = strlen(buffer);
    ASSERT(current_length >= 0 && current_length < size);

    va_list list;
    va_start(list, format);

    cvsnzprintf(&buffer[current_length], size - current_length, format, list);

    va_end(list);

    return buffer;
}

bool string_is_not_empty(char const* s)
{
    return s && *s;
}

char* strncpy_debug(char* s1, dword size1, char const* s2, dword size2)
{
    ASSERT(s1 && s2);
    ASSERT(size1 >= 0 && size1 <= MAXIMUM_STRING_SIZE);
    ASSERT(size2 >= 0 && size2 <= MAXIMUM_STRING_SIZE);
    strncpy_s(s1, size1, s2, size2);
    return s1;
}

long strlen_debug(char const* s)
{
    ASSERT(s);
    long length = strlen(s);
    ASSERT(length >= 0 && length < MAXIMUM_STRING_SIZE);
    return length;
}

char* tag_to_string(tag _tag, char* buffer)
{
    *(tag*)buffer = _byteswap_ulong(_tag);
    buffer[4] = 0;

    return buffer;
}

//char const* c_string_id::get_string()
//{
//    return string_id_get_string_const(m_value);
//}
//
//char const* c_string_id::get_string() const
//{
//    return string_id_get_string_const(m_value);
//}

long __fastcall bit_vector_count_bits(const dword* bit_mask, int bit_count)
{
    return INVOKE(0xC39D0, bit_vector_count_bits, bit_mask, bit_count);
}

__int64 make_int64(__int64 a, __int64 b)
{
    return ((a << 0) | (b << 32));
}

long bit_count(long val)
{
    long result = 0;
    while (val != 0)
    {
        val >>= 1;
        result++;
    }
    return result;
}

long __fastcall index_from_mask(const dword* mask, long bit_count)
{
    return INVOKE(0xC3C10, index_from_mask, mask, bit_count);
}

void* offset_pointer(void* pointer, long offset)
{
    return pointer_from_address(address_from_pointer(pointer) + offset);
}

void const* offset_pointer(void const* pointer, long offset)
{
    return pointer_from_address(address_from_pointer(pointer) + offset);
}

unsigned int address_from_pointer(void const* pointer)
{
    return reinterpret_cast<unsigned int>(pointer);
}

void* pointer_from_address(unsigned int address)
{
    return reinterpret_cast<void*>(address);
}

unsigned int align_address(unsigned int address, long alignment_bits)
{
    return (address + (1 << alignment_bits) - 1) & ~((1 << alignment_bits) - 1);
}

void* align_pointer(void* pointer, long alignment_bits)
{
    return pointer_from_address(align_address(address_from_pointer(pointer), alignment_bits));
}

long pointer_distance(void const* pointer_a, void const* pointer_b)
{
    return static_cast<long>((char*)pointer_b - (char*)pointer_a);
}

long pointer_difference(void const* pointer_a, void const* pointer_b)
{
    return static_cast<long>((char*)pointer_b - (char*)pointer_a);
}

//template<long k_maximum_count>
//void c_static_string<k_maximum_count>::set_wchar(const wchar_t* src)
//{
//    wchar_string_to_ascii_string(src, m_string, k_maximum_count, NULL);
//}