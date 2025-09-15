#pragma once

#include <cseries\cseries.h>
#include <tag_files\files_windows.h>

#define FILE_REFERENCE_SIGNATURE 'filo'
#define NUMBER_OF_FILE_REFERENCE_LOCATIONS 2

#define DATASTORE_MAX_DATA_SIZE 256
#define DATASTORE_MAX_FIELD_NAME_SIZE 256

enum e_file_reference_flags
{
	_file_reference_flag_is_file_name = 0,
	_file_reference_flag_open_for_write,
};

enum e_name
{
	_name_directory_bit = 0,
	_name_parent_directory_bit,
	_name_file_bit,
	_name_extension_bit,

	k_name_count
};

struct file_reference_info
{
	tag signature;
	ushort flags;
	short location;
	c_static_string<k_tag_long_string_length> path;
};
static_assert(sizeof(file_reference_info) == 0x108);

struct s_file_reference : file_reference_info
{
	s_file_handle handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);

s_file_reference* file_reference_create(s_file_reference* reference, ushort location);
s_file_reference* file_reference_agnostic_create(s_file_reference* reference, ushort location);
void __fastcall file_reference_add_directory(s_file_reference* reference, const char* path);
void __fastcall file_reference_add_directory(s_file_reference* reference, wchar_t const* path);
void __fastcall file_reference_set_name(s_file_reference* reference, const char* name);
void __fastcall file_reference_set_name(s_file_reference* reference, wchar_t const* name);
s_file_reference* file_reference_create_from_path(s_file_reference* reference, const char* path, bool is_directory);
s_file_reference* file_reference_create_from_path(s_file_reference* reference, wchar_t const* path, bool is_directory);
void file_path_add_name(wchar_t* path, long maximum_path_length, const wchar_t* name);
bool __fastcall file_create_parent_directories_if_not_present(const struct s_file_reference* info);
file_reference_info* file_reference_get_info(s_file_reference* info);
const file_reference_info* file_reference_get_info(const s_file_reference* info);
void file_trim(s_file_reference* reference, long size);
char* file_reference_get_name(const s_file_reference* reference, ulong flags, char* out_name, long name_length);
wchar_t* __fastcall file_reference_get_name(const s_file_reference* reference, ulong flags, wchar_t* out_name, long name_length);
void directory_create_or_delete_contents(const char* directory);