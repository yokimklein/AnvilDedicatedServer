#include "files_windows.h"
#include "tag_files\files.h"
#include "cseries\cseries_events.h"
#include "memory\tls.h"
#include <windows.h>

void suppress_file_errors(bool suppress)
{
	TLS_DATA_GET_VALUE_REFERENCE(g_file_errors_suppressed);
	g_file_errors_suppressed = suppress;
}

bool file_errors_suppressed()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_file_errors_suppressed);
	return g_file_errors_suppressed;
}

bool file_close(s_file_reference* reference)
{
	if (CloseHandle(reference->handle.handle))
	{
		invalidate_file_handle(&reference->handle);
		reference->position = 0;

		return true;
	}

	file_error(__FUNCTION__, reference, NULL, false);

	return false;
}

bool file_copy_to(const s_file_reference* reference, s_file_reference* other, bool fail_if_not_exists)
{
	const file_reference_info* info_reference = file_reference_get_info(reference);
	const file_reference_info* info_other = file_reference_get_info(other);

	c_static_wchar_string<0x100> existing_filename{};
	c_static_wchar_string<0x100> new_filename{};

	file_create_parent_directories_if_not_present(other);

	file_location_get_full_path(info_reference->location, info_reference->path.get_string(), existing_filename);
	file_location_get_full_path(info_other->location, info_other->path.get_string(), new_filename);

	if (!CopyFileW(existing_filename.get_string(), new_filename.get_string(), fail_if_not_exists == 0))
	{
		file_error(__FUNCTION__, reference, other, false);
		return false;
	}
	return true;
}

bool file_create(s_file_reference* reference)
{
	ASSERT(reference);

	if (TEST_BIT(reference->flags, _file_reference_flag_is_file_name))
	{
		HANDLE handle = CreateFileA(reference->path.get_string(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle && handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}
	}
	else if (CreateDirectoryA(reference->path.get_string(), NULL))
	{
		return true;
	}

	return false;
}

void file_error(const char* file_function, const s_file_reference* reference_a, const s_file_reference* reference_b, bool suppress_error)
{
	const file_reference_info* info0 = file_reference_get_info(reference_a);
	const file_reference_info* info1 = NULL;
	if (reference_b)
	{
		info1 = file_reference_get_info(reference_b);
	}

	DWORD error_message_id = GetLastError();
	if (!file_errors_suppressed() && !suppress_error)
	{
		char system_message[1024]{};
		if (info1)
		{
			csnzprintf(system_message, sizeof(system_message), "%s('%s', '%s')", file_function, info0->path.get_string(), info1->path.get_string());
		}
		else
		{
			csnzprintf(system_message, sizeof(system_message), "%s('%s')", file_function, info0->path.get_string());
		}

		char error_message[2048]{};
		get_error_message(error_message_id, error_message);

		event(_event_warning, "system: %s error 0x%08x '%s'", system_message, error_message_id, error_message);
	}
	SetLastError(ERROR_SUCCESS);
}

bool file_exists(const s_file_reference* reference)
{
	ASSERT(reference);

	return GetFileAttributesA(reference->path.get_string()) != INVALID_FILE_ATTRIBUTES;
}

bool file_open(s_file_reference* reference, ulong open_flags, ulong* error)
{
	ASSERT(reference);
	ASSERT(error);

	bool result = false;
	ulong desired_access = 0;
	ulong share_mode = 0;
	ulong flags_and_attributes = FILE_READ_ATTRIBUTES;

	*error = _file_open_error_none;

	if (TEST_BIT(open_flags, _file_open_flag_desired_access_read))
	{
		desired_access = GENERIC_READ;
	}
	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
	{
		desired_access |= GENERIC_WRITE;
	}

	if (TEST_BIT(open_flags, _file_open_flag_share_mode_read))
	{
		share_mode |= FILE_SHARE_READ;
	}
	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
	{
		share_mode |= FILE_SHARE_WRITE;
	}

	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_write))
	{
		flags_and_attributes = FILE_WRITE_ATTRIBUTES;
	}
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_delete_on_close))
	{
		flags_and_attributes = FILE_FLAG_DELETE_ON_CLOSE;
	}
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_random_access))
	{
		flags_and_attributes = FILE_FLAG_RANDOM_ACCESS;
	}
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_sequecial_scan))
	{
		flags_and_attributes = FILE_FLAG_SEQUENTIAL_SCAN;
	}

	HANDLE handle = CreateFileA(reference->path.get_string(), desired_access, share_mode, NULL, OPEN_EXISTING, flags_and_attributes, NULL);
	if (!handle || handle == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
			case ERROR_FILE_NOT_FOUND:
			{
				*error = _file_open_error_file_not_found;
			}
			break;
			case ERROR_PATH_NOT_FOUND:
			{
				*error = _file_open_error_path_not_found;
			}
			break;
			case ERROR_ACCESS_DENIED:
			{
				*error = _file_open_error_access_denied;
			}
			break;
			case ERROR_INVALID_DRIVE:
			{
				*error = _file_open_error_invalid_drive;
			}
			break;
			case ERROR_SHARING_VIOLATION:
			{
				*error = _file_open_error_sharing_violation;
			}
			break;
			default:
			{
				*error = _file_open_error_unknown;
			}
			break;
		}
	}
	else
	{
		result = true;

		reference->handle.handle = handle;
		reference->position = 0;

		if (TEST_BIT(open_flags, _file_open_flag_set_file_end_and_close))
		{
			reference->position = SetFilePointer(reference->handle.handle, 0, 0, FILE_END);
			if (reference->position == INVALID_SET_FILE_POINTER)
			{
				CloseHandle(reference->handle.handle);
				invalidate_file_handle(&reference->handle);
				reference->position = 0;

				result = false;
			}
		}
	}

	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
	{
		reference->flags &= ~FLAG(_file_reference_flag_open_for_write);
	}

	return result;
}

#pragma runtime_checks("", off)
bool __fastcall file_write(s_file_reference* reference, ulong size, const void* buffer)
{
	bool result = INVOKE(0xA47D0, file_write, reference, size, buffer);
	__asm add esp, 4; // Fix usercall & cleanup stack
	return result;
}
#pragma runtime_checks("", restore)

void file_location_get_full_path(short location, const char* path, c_static_wchar_string<0x100> out_full_path)
{
	ASSERT(path);

	c_static_wchar_string<0x100> src;
	char_string_to_wchar_string(src.get_buffer(), path);
	out_full_path.set_length(0);

	if (location <= 0)
	{
		out_full_path.set(src.get_string());
	}
	else
	{
		out_full_path.set(L"?:\\");
		out_full_path.append(src.get_string());
	}
}

void invalidate_file_handle(s_file_handle* handle)
{
	handle->handle = INVALID_HANDLE_VALUE;
}

ulong __fastcall file_get_eof(const s_file_reference* reference)
{
	return INVOKE(0xA46D0, file_get_eof, reference);
}

bool file_set_position(s_file_reference* reference, ulong offset, bool print_error)
{
	if (reference->position == offset)
	{
		return true;
	}

	if (file_handle_is_valid(reference->handle))
	{
		reference->position = SetFilePointer(reference->handle.handle, offset, 0, 0);
	}
	else
	{
		SetLastError(ERROR_INVALID_HANDLE);
	}

	bool result = reference->position != INVALID_SET_FILE_POINTER;
	if (!result)
	{
		file_error(__FUNCTION__, reference, NULL, print_error);
	}

	return result;
}

bool file_handle_is_valid(s_file_handle handle)
{
	return handle.handle && handle.handle != INVALID_HANDLE_VALUE;
}

bool file_read(s_file_reference* reference, ulong size, bool print_error, void* buffer)
{
	ASSERT(reference);
	ASSERT(buffer);

	ulong bytes_read = 0;
	bool result = false;

	if (!size)
	{
		return true;
	}

	if (ReadFile(reference->handle.handle, buffer, size, &bytes_read, NULL))
	{
		if (size == bytes_read)
		{
			result = true;
		}
		else
		{
			SetLastError(ERROR_HANDLE_EOF);
		}
	}

	reference->position += bytes_read;

	if (!result)
	{
		file_error(__FUNCTION__, reference, NULL, print_error);
	}

	return result;
}

bool __fastcall file_set_eof(s_file_reference* reference, ulong offset)
{
	return INVOKE(0xA46D0, file_set_eof, reference, offset);
}

void find_files_start(s_find_file_data* data, ulong flags, const s_file_reference* file)
{
	find_files_start_with_search_spec(data, flags, file, "*.*");
}

void find_files_start_with_search_spec(s_find_file_data* data, ulong flags, const s_file_reference* file, const char* search_spec)
{
	for (short i = 0; i < NUMBEROF(data->active_find_file_state.handles); i++)
	{
		invalidate_file_handle(&data->active_find_file_state.handles[i]);
	}

	data->flags = flags;
	data->depth = 0;
	data->location = file->location;

	data->path.append_print(L"%hs", file->path.get_string());
	data->search_spec.append_print(L"%hs", search_spec);
}

bool __fastcall find_files_next(s_find_file_data* data, s_file_reference* out_file, s_file_last_modification_date* out_date)
{
	return INVOKE(0xA4B40, find_files_next, data, out_file, out_date);
}

void __fastcall find_files_end(s_find_file_data* data)
{
	INVOKE(0xA4AF0, find_files_end, data);
}

bool __fastcall file_delete(s_file_reference* reference)
{
	return INVOKE(0xA3FD0, file_delete, reference);
}