#pragma once

#include <cseries\cseries.h>
#include <tag_files\files_windows.h>

struct s_file_reference
{
	dword signature;
	word_flags flags;
	short location;
	long_string path;
	s_file_handle handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);
typedef s_file_reference file_reference_info;

s_file_reference* file_reference_create(s_file_reference* reference, short location);
s_file_reference* file_reference_agnostic_create(s_file_reference* reference, short location);
void __fastcall file_reference_add_directory(s_file_reference* reference, wchar_t const* path);
void __fastcall file_reference_set_name(s_file_reference* reference, wchar_t const* name);
s_file_reference* file_reference_create_from_path(s_file_reference* reference, wchar_t const* path, bool is_directory);