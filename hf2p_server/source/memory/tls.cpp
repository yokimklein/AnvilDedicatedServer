#include "tls.h"
#include <windows.h>

s_thread_local_storage* get_tls()
{
	static dword tls_index = 'NOGO';
	if (tls_index == 'NOGO')
	{
		byte* module_base_byte = reinterpret_cast<byte*>(module_base);
		PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_base_byte);
		PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(module_base_byte + dos_header->e_lfanew);
		PIMAGE_TLS_DIRECTORY tls_directory = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(module_base_byte + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		tls_index = *reinterpret_cast<dword*>(tls_directory->AddressOfIndex);
	}

	if (tls_index == 'NOGO')
		return nullptr;

	void** tls_ptr = reinterpret_cast<void**>(__readfsdword(0x2C));
	return reinterpret_cast<s_thread_local_storage*>(tls_ptr[tls_index]);
}