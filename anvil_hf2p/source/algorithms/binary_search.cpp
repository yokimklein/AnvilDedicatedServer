#include "binary_search.h"
#include "cseries\cseries.h"

#pragma runtime_checks("", off)
void* __fastcall binary_search_elements(void const* search_for, void const* base, long element_size, ulong count, binary_search_comparison_proc_t comparison_function, void const* a6)
{
    // the function we're invoking has a6 hardcoded to 0, so assert if someone calls this with a6 != 0
    ASSERT(a6 == 0);

    void* result = DECLFUNC(0x160FC0, void*, __fastcall, void const*, void const*, long, ulong, binary_search_comparison_proc_t, long)(search_for, base, element_size, count, comparison_function, 0);

    __asm add esp, 16; // Fix usercall & cleanup stack

    return result;
}
#pragma runtime_checks("", restore)
