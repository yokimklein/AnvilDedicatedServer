#include "files.h"

s_file_reference* file_reference_create(s_file_reference* reference, short location)
{
    file_reference_agnostic_create(reference, location);
    reference->handle.handle = (void*)-1;
    return reference;
}

s_file_reference* file_reference_agnostic_create(s_file_reference* reference, short location)
{
    memset(reference, 0, sizeof(s_file_reference));
    reference->signature = 'filo';
    reference->location = location;
    return reference;
}

void file_reference_add_directory(s_file_reference* reference, wchar_t const* path)
{
    DECLFUNC(base_address(0xA5690), void, __fastcall, s_file_reference*, wchar_t const*)(reference, path);
}

void file_reference_set_name(s_file_reference* reference, wchar_t const* name)
{
    DECLFUNC(base_address(0xA5710), void, __fastcall, s_file_reference*, wchar_t const*)(reference, name);
}

s_file_reference* file_reference_create_from_path(s_file_reference* reference, wchar_t const* path, bool is_directory)
{
	file_reference_create(reference, -1);
    if (is_directory)
        file_reference_add_directory(reference, path);
    else
        file_reference_set_name(reference, path);
    return reference;
}