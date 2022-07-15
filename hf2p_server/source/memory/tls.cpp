#include "tls.h"

void* tls_get(const unsigned long offset)
{
	void* address = nullptr;
	_asm
	{
		mov eax, dword ptr fs : [2Ch]
		mov eax, dword ptr ds : [eax]
		mov address, eax
	}

	return (s_thread_local_storage*)address + offset;
}