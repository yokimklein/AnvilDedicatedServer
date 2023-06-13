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