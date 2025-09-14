#include "cseries.h"

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/cseries.cpp

#include <memory\byte_swapping.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <text\unicode.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline size_t base_address_impl(size_t address)
{
    return (size_t)GetModuleHandle(NULL) + address;
}

real_argb_color const private_real_argb_colors[17]
{
    { 1.0f,  1.0f,  1.0f,  1.0f },
    { 1.0f,  0.5f,  0.5f,  0.5f },
    { 1.0f,  0.0f,  0.0f,  0.0f },
    { 1.0f,  1.0f,  0.0f,  0.0f },
    { 1.0f,  0.0f,  1.0f,  0.0f },
    { 1.0f,  0.0f,  0.0f,  1.0f },
    { 1.0f,  0.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f,  0.0f },
    { 1.0f,  1.0f,  0.0f,  1.0f },
    { 1.0f,  1.0f, 0.41f,  0.7f },
    { 1.0f, 0.39f, 0.58f, 0.93f },
    { 1.0f,  1.0f,  0.5f,  0.0f },
    { 1.0f, 0.44f, 0.05f, 0.43f },
    { 1.0f,  0.5f,  1.0f, 0.83f },
    { 1.0f,  0.0f, 0.39f,  0.0f },
    { 1.0f,  1.0f, 0.63f, 0.48f },
    { 1.0f, 0.81f, 0.13f, 0.56f }
};

const real_argb_color* const global_real_argb_color_table = private_real_argb_colors;

const real_argb_color* const global_real_argb_white = &private_real_argb_colors[0];
const real_argb_color* const global_real_argb_grey = &private_real_argb_colors[1];
const real_argb_color* const global_real_argb_black = &private_real_argb_colors[2];
const real_argb_color* const global_real_argb_red = &private_real_argb_colors[3];
const real_argb_color* const global_real_argb_green = &private_real_argb_colors[4];
const real_argb_color* const global_real_argb_blue = &private_real_argb_colors[5];
const real_argb_color* const global_real_argb_yellow = &private_real_argb_colors[7];
const real_argb_color* const global_real_argb_cyan = &private_real_argb_colors[6];
const real_argb_color* const global_real_argb_magenta = &private_real_argb_colors[8];
const real_argb_color* const global_real_argb_pink = &private_real_argb_colors[9];
const real_argb_color* const global_real_argb_lightblue = &private_real_argb_colors[10];
const real_argb_color* const global_real_argb_orange = &private_real_argb_colors[11];
const real_argb_color* const global_real_argb_purple = &private_real_argb_colors[12];
const real_argb_color* const global_real_argb_aqua = &private_real_argb_colors[13];
const real_argb_color* const global_real_argb_darkgreen = &private_real_argb_colors[14];
const real_argb_color* const global_real_argb_salmon = &private_real_argb_colors[15];
const real_argb_color* const global_real_argb_violet = &private_real_argb_colors[16];

const real_rgb_color* const global_real_rgb_white = &private_real_argb_colors[0].rgb;
const real_rgb_color* const global_real_rgb_grey = &private_real_argb_colors[1].rgb;
const real_rgb_color* const global_real_rgb_black = &private_real_argb_colors[2].rgb;
const real_rgb_color* const global_real_rgb_red = &private_real_argb_colors[3].rgb;
const real_rgb_color* const global_real_rgb_green = &private_real_argb_colors[4].rgb;
const real_rgb_color* const global_real_rgb_blue = &private_real_argb_colors[5].rgb;
const real_rgb_color* const global_real_rgb_yellow = &private_real_argb_colors[7].rgb;
const real_rgb_color* const global_real_rgb_cyan = &private_real_argb_colors[6].rgb;
const real_rgb_color* const global_real_rgb_magenta = &private_real_argb_colors[8].rgb;
const real_rgb_color* const global_real_rgb_pink = &private_real_argb_colors[9].rgb;
const real_rgb_color* const global_real_rgb_lightblue = &private_real_argb_colors[10].rgb;
const real_rgb_color* const global_real_rgb_orange = &private_real_argb_colors[11].rgb;
const real_rgb_color* const global_real_rgb_purple = &private_real_argb_colors[12].rgb;
const real_rgb_color* const global_real_rgb_aqua = &private_real_argb_colors[13].rgb;
const real_rgb_color* const global_real_rgb_darkgreen = &private_real_argb_colors[14].rgb;
const real_rgb_color* const global_real_rgb_salmon = &private_real_argb_colors[15].rgb;
const real_rgb_color* const global_real_rgb_violet = &private_real_argb_colors[16].rgb;

REFERENCE_DECLARE(0xE9B960, c_normal_allocation*, g_normal_allocation);

int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size) = memcmp;
void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size) = memcpy;
void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size) = memset;

#define MAXIMUM_STRING_SIZE 0x100000

long csstricmp(char const* s1, char const* s2)
{
    return _stricmp(s1, s2);
}

long csstrnicmp(char const* s1, char const* s2, dword size)
{
    return _strnicmp(s1, s2, size);
}

long csstrcmp(char const* s1, char const* s2)
{
    return strcmp(s1, s2);
}

extern long csstrncmp(char const* s1, char const* s2, dword size)
{
    return strncmp(s1, s2, size);
}

char* csstristr(char const* s1, char const* s2)
{
    char v5 = *s2;
    const char* v8 = s2 + 1;
    if (*s2)
    {
        ulong v4 = strlen_debug(v8);
        long v3;
        do
        {
            char v6 = *s1++;
            if (!v6)
            {
                return NULL;
            }
            v3 = tolower(v6);
        }
        while (v3 != tolower(v5) || csstrnicmp(s1, v8, v4));
        --s1;
    }
    return (char*)s1;
}

char* csstrnzcpy(char* s1, char const* s2, dword size)
{
    ASSERT(s1 && s2);
    ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

    size_t dst_len = 0;
    for (const char* src_cur_char = s2; dst_len < size; ++dst_len)
    {
        if (!*src_cur_char++)
            break;
    }

    strncpy_debug(s1, s2, size);

    if (dst_len >= size)
    {
        s1[size - 1] = 0;
    }
    else
    {
        memset(&s1[dst_len], 0, size - dst_len);
    }

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

char* csstrtok(char* a1, char const* delimiters, bool a3, csstrtok_data* data)
{
    ASSERT(data);
    ASSERT(delimiters);

    csstrtok_data csstrtok_data;

    if (a1)
    {
        csstrtok_data.unknown3 = a1;
    }
    else
    {
        csstrtok_data.unknown3 = data->unknown1;
    }
    csstrtok_data.unknown1 = csstrtok_data.unknown3;
    if (a3)
    {
        if (csstrtok_data.unknown3)
        {
            csstrtok_data.unknown1 = &csstrtok_data.unknown3[strspn(csstrtok_data.unknown3, delimiters)];
            if (!*csstrtok_data.unknown1)
            {
                csstrtok_data.unknown1 = 0;
            }
        }
    }
    csstrtok_data.unknown2 = csstrtok_data.unknown1;
    if (csstrtok_data.unknown1)
    {
        csstrtok_data.unknown1 = strpbrk(csstrtok_data.unknown1, delimiters);
        if (csstrtok_data.unknown1)
        {
            *csstrtok_data.unknown1++ = 0;
        }
    }
    data->unknown1 = csstrtok_data.unknown1;
    return csstrtok_data.unknown2;
}

long cvsnzprintf(char* buffer, dword size, char const* format, va_list list)
{
    ASSERT(buffer);
    ASSERT(format);
    ASSERT(size > 0);

    long result = _vsnprintf_s(buffer, size, size - 1, format, list);
    buffer[size - 1] = 0;

    //size_t buf_size = strlen(buffer);
    //memset(buffer + buf_size, 0, size - buf_size);

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

void string_terminate_at_first_delimiter(char* s, const char* delimiter)
{
    s[strcspn(s, delimiter)] = 0;
}

char* strncpy_debug(char* s1, char const* s2, dword size)
{
    ASSERT(s1 && s2);
    ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);
    strncpy_s(s1, size, s2, -1);
    return s1;
}

long strlen_debug(char const* s)
{
    ASSERT(s);
    long length = strlen(s);
    ASSERT(length >= 0 && length < MAXIMUM_STRING_SIZE);
    return length;
}

bool ascii_isupper(char C)
{
    return C >= 'A' && C <= 'Z';
}

void ascii_strnlwr(char* string, long count)
{
    ASSERT(string != NULL || count == 0);
    ASSERT(count >= 0 && count < MAXIMUM_STRING_SIZE);

    for (long i = 0; i < count && string[i]; i++)
    {
        if (ascii_isupper(string[i]))
        {
            string[i] += ' ';
        }
    }
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

c_string_builder::c_string_builder() :
    c_static_string<1024>()
{
}

c_string_builder::c_string_builder(const char* format, ...) :
    c_static_string<1024>()
{
    va_list list;
    va_start(list, format);

    print_va(format, list);

    va_end(list);
}

c_string_builder::~c_string_builder()
{

}

c_allocation_base::c_allocation_base()
{
}
