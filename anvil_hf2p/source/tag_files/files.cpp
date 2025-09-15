#include "files.h"
#include <text\unicode.h>

s_file_reference* file_reference_create(s_file_reference* reference, ushort location)
{
    file_reference_agnostic_create(reference, location);
    reference->handle.handle = (void*)NONE;
    return reference;
}

s_file_reference* file_reference_agnostic_create(s_file_reference* reference, ushort location)
{
    csmemset(reference, 0, sizeof(s_file_reference));
    reference->signature = FILE_REFERENCE_SIGNATURE;
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
    char_string_to_wchar_string(buffer, name);
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

file_reference_info* file_reference_get_info(s_file_reference* info)
{
    ASSERT(info);
    ASSERT(info->signature == FILE_REFERENCE_SIGNATURE);
    //ASSERT(VALID_BITS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
    ASSERT(info->location >= NONE && info->location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

    return info;
}

const file_reference_info* file_reference_get_info(const s_file_reference* info)
{
    ASSERT(info);
    ASSERT(info->signature == FILE_REFERENCE_SIGNATURE);
    //ASSERT(VALID_BITS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
    ASSERT(info->location >= NONE && info->location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

    return info;
}

void file_trim(s_file_reference* reference, long size)
{
    ulong error;
    constexpr ulong open_flags = FLAG(_file_open_flag_desired_access_read) | FLAG(_file_open_flag_desired_access_write);
    if (!file_open(reference, open_flags, &error))
    {
        return;
    }

    long eof = file_get_eof(reference);
    if (eof > size)
    {
        long remaining_bytes = size;
        long position = eof - size;
        long total_size = 0;
        char buffer[0x400];

        while (remaining_bytes > 0)
        {
            long buffer_size = MIN(remaining_bytes, 0x400);

            if (!file_set_position(reference, position, false))
            {
                break;
            }

            if (!file_read(reference, buffer_size, 0, buffer))
            {
                break;
            }

            if (!file_set_position(reference, total_size, false))
            {
                break;
            }

            if (!file_write(reference, buffer_size, buffer))
            {
                break;
            }

            position += buffer_size;
            total_size += buffer_size;
            remaining_bytes -= buffer_size;
        }
        file_set_eof(reference, size);
    }
    file_close(reference);
}

char* file_reference_get_name(const s_file_reference* reference, ulong flags, char* out_name, long name_length)
{
    wchar_t buffer[260]; // MAX_PATH
    file_reference_get_name(reference, flags, buffer, name_length);
    wchar_string_to_ascii_string(buffer, out_name, name_length, NULL);
    return out_name;
}

wchar_t* __fastcall file_reference_get_name(const s_file_reference* reference, ulong flags, wchar_t* out_name, long name_length)
{
    return DECLFUNC(0xA5790, wchar_t*, __fastcall, const s_file_reference*, ulong, wchar_t*, long)(reference, flags, out_name, name_length);
}

void directory_create_or_delete_contents(const char* directory)
{
    s_file_reference file_reference;
    file_reference_create(&file_reference, NONE); // $TODO: NONE may need to be unsigned here
    file_reference_add_directory(&file_reference, directory);
    if (file_exists(&file_reference))
    {
        s_find_file_data data;
        s_file_reference out_file;
        find_files_start(&data, 0, &file_reference);
        while (find_files_next(&data, &out_file, NULL))
        {
            file_delete(&out_file);
        }
        find_files_end(&data);
    }
    else
    {
        file_create(&file_reference);
    }
}