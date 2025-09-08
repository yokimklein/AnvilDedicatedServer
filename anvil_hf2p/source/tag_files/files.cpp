#include "files.h"
#include <text\unicode.h>

s_file_reference* file_reference_create(s_file_reference* reference, short location)
{
    file_reference_agnostic_create(reference, location);
    reference->handle.handle = (void*)NONE;
    return reference;
}

s_file_reference* file_reference_agnostic_create(s_file_reference* reference, short location)
{
    csmemset(reference, 0, sizeof(s_file_reference));
    reference->signature = 'filo';
    reference->location = location;
    return reference;
}

void __fastcall file_reference_add_directory(s_file_reference* reference, const char* path)
{
    wchar_t buffer[0x100];
    ascii_string_to_wchar_string(path, buffer, NUMBEROF(buffer), NULL);
    file_reference_add_directory(reference, buffer);
}

void __fastcall file_reference_add_directory(s_file_reference* reference, wchar_t const* path)
{
    DECLFUNC(0xA5690, void, __fastcall, s_file_reference*, wchar_t const*)(reference, path);
}

void __fastcall file_reference_set_name(s_file_reference* reference, const char* name)
{
    wchar_t buffer[0x100];
    ascii_string_to_wchar_string(name, buffer, NUMBEROF(buffer), NULL);
    file_reference_set_name(reference, buffer);
}

void __fastcall file_reference_set_name(s_file_reference* reference, wchar_t const* name)
{
    DECLFUNC(0xA5710, void, __fastcall, s_file_reference*, wchar_t const*)(reference, name);
}

s_file_reference* file_reference_create_from_path(s_file_reference* reference, const char* path, bool is_directory)
{
    file_reference_create(reference, NONE);
    if (is_directory)
    {
        file_reference_add_directory(reference, path);
    }
    else
    {
        file_reference_set_name(reference, path);
    }
    return reference;
}

s_file_reference* file_reference_create_from_path(s_file_reference* reference, wchar_t const* path, bool is_directory)
{
	file_reference_create(reference, NONE);
    if (is_directory)
    {
        file_reference_add_directory(reference, path);
    }
    else
    {
        file_reference_set_name(reference, path);
    }
    return reference;
}

void file_path_add_name(wchar_t* path, long maximum_path_length, const wchar_t* name)
{
    if (*name)
    {
        ulong path_len = ustrlen(path);
        wchar_t* buffer = &path[path_len];
        if (buffer != path && *(buffer - 1) != '\\')
        {
            *buffer++ = '\\';
            *buffer = '\0';
            ++path_len;
        }
        ustrnzcpy(buffer, name, maximum_path_length - path_len);
    }
}

bool __fastcall file_create_parent_directories_if_not_present(const s_file_reference* info)
{
    return INVOKE(0xA58E0, file_create_parent_directories_if_not_present, info);
}
