#pragma once
#include "cseries\cseries.h"
#include "text\unicode.h"

enum
{
	_find_files_recursive_bit = 0,
	_find_files_enumerate_directories_bit,
	_find_files_enumerate_files_with_directories_bit,

	NUMBER_OF_FIND_FILES_FLAGS,
};

enum e_file_open_flags
{
	_file_open_flag_desired_access_read = 0,
	_file_open_flag_desired_access_write,
	_file_open_flag_set_file_end_and_close,
	_file_open_flag_share_mode_read,
	_file_open_flag_print_error,
	_file_open_flag_flags_and_attributes_write,
	_file_open_flag_flags_and_attributes_delete_on_close,
	_file_open_flag_flags_and_attributes_random_access,
	_file_open_flag_flags_and_attributes_sequecial_scan,
};

enum e_file_open_error
{
	_file_open_error_none = 0,
	_file_open_error_file_not_found,
	_file_open_error_access_denied,
	_file_open_error_path_not_found,
	_file_open_error_invalid_drive,
	_file_open_error_sharing_violation,
	_file_open_error_unknown,

	k_file_open_error_count
};

enum : ulong
{
	k_file_attribute_readonly = 0x00000001,
	k_file_attribute_hidden = 0x00000002,
	k_file_attribute_system = 0x00000004,
	k_file_attribute_directory = 0x00000010,
	k_file_attribute_archive = 0x00000020,
	k_file_attribute_device = 0x00000040,
	k_file_attribute_normal = 0x00000080,
	k_file_attribute_temporary = 0x00000100,
	k_file_attribute_sparse_file = 0x00000200,
	k_file_attribute_reparse_point = 0x00000400,
	k_file_attribute_compressed = 0x00000800,
	k_file_attribute_offline = 0x00001000,
	k_file_attribute_not_content_indexed = 0x00002000,
	k_file_attribute_encrypted = 0x00004000,
	k_file_attribute_integrity_stream = 0x00008000,
	k_file_attribute_virtual = 0x00010000,
	k_file_attribute_no_scrub_data = 0x00020000,
	k_file_attribute_ea = 0x00040000,
	k_file_attribute_pinned = 0x00080000,
	k_file_attribute_unpinned = 0x00100000,
	k_file_attribute_recall_on_open = 0x00040000,
	k_file_attribute_recall_on_data_access = 0x00400000,
};

struct s_file_last_modification_date
{
	ulong low;
	ulong hi;
};
static_assert(sizeof(s_file_last_modification_date) == 0x8);

struct s_file_handle
{
	void* handle;
};
static_assert(sizeof(s_file_handle) == 0x4);

struct s_indirect_file
{
	// map_file_index?
	void* handle;
};
static_assert(sizeof(s_indirect_file) == 0x4);

struct s_find_file_data
{
	struct s_find_file_state
	{
		s_file_handle handles[16];

		struct // WIN32_FIND_DATAW
		{
			ulong dwFileAttributes;
			s_file_last_modification_date ftCreationTime;
			s_file_last_modification_date ftLastAccessTime;
			s_file_last_modification_date ftLastWriteTime;
			ulong nFileSizeHigh;
			ulong nFileSizeLow;
			ulong dwReserved0;
			ulong dwReserved1;
			wchar_t  cFileName[260];
			wchar_t  cAlternateFileName[14];
#if defined(_MAC)
			ulong dwFileType;
			ulong dwCreatorType;
			uns16  wFinderFlags;
#endif
		} find_data;
	};

	ulong flags;

	short depth;
	short location;

	c_static_wchar_string<256> path;
	c_static_wchar_string<256> search_spec;

	s_find_file_state active_find_file_state;

	ulong __unknown698;
};

struct s_file_reference;

extern void suppress_file_errors(bool suppress);
extern bool file_errors_suppressed();
extern bool file_close(s_file_reference* reference);
extern bool file_copy_to(const s_file_reference* reference, s_file_reference* other, bool fail_if_not_exists);
extern bool file_create(s_file_reference* reference);
extern void file_error(const char* file_function, const s_file_reference* reference_a, const s_file_reference* reference_b, bool suppress_error);
extern bool file_exists(const s_file_reference* reference);
extern bool file_open(s_file_reference* reference, ulong open_flags, ulong* error);
extern bool __fastcall file_write(s_file_reference* reference, ulong size, const void* buffer);
extern void file_location_get_full_path(short location, const char* path, c_static_wchar_string<0x100> out_full_path);
extern void invalidate_file_handle(s_file_handle* handle);
extern ulong __fastcall file_get_eof(const s_file_reference* reference);
extern bool file_set_position(s_file_reference* reference, ulong offset, bool print_error);
extern bool file_handle_is_valid(s_file_handle handle);
extern bool file_read(s_file_reference* reference, ulong size, bool print_error, void* buffer);
extern bool __fastcall file_set_eof(s_file_reference* reference, ulong offset);
extern void find_files_start(s_find_file_data* data, ulong flags, const s_file_reference* file);
extern void find_files_start_with_search_spec(s_find_file_data* data, ulong flags, const s_file_reference* file, const char* search_spec);
extern bool __fastcall find_files_next(s_find_file_data* data, s_file_reference* out_file, s_file_last_modification_date* out_date);
extern void __fastcall find_files_end(s_find_file_data* data);
extern bool __fastcall file_delete(s_file_reference* reference);