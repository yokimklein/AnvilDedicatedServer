#include "async_helpers.h"

#pragma runtime_checks("", off)
long __fastcall async_read_position(s_file_handle file, void* buffer, long size, ulong offset, e_async_category category, e_async_priority priority, c_synchronized_long* bytes_read, c_synchronized_long* done)
{
    long result = INVOKE(ADDRESS_ASYNC_READ_POSITION, async_read_position, file, buffer, size, offset, category, priority, bytes_read, done);
    __asm
    {
        add esp, 24; // Fix usercall & cleanup stack
    }
    return result;
}
#pragma runtime_checks("", restore)