#include "async.h"
#include "cseries\cseries.h"

void __fastcall internal_async_yield_until_done(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, long line)
{
	async_yield_until_done_function(done, /*simple_yield_function*/NULL, idle_sound, false, show_debug_progress, _yield_for_unknown);
}

#pragma runtime_checks("", off)
void __fastcall async_yield_until_done_function(c_synchronized_long* done, bool(*yield_function)(c_synchronized_long*), bool idle, bool networking, bool spinner, e_yield_reason yield_reason)
{
	VASSERT(yield_function == NULL, "yield_function field was optimised out for this invoke, but the field was populated!");
	INVOKE(0x98B80, async_yield_until_done_function, done, NULL, idle, networking, spinner, yield_reason);
    __asm
    {
        add esp, 16; // Fix usercall & cleanup stack
    }
}
#pragma runtime_checks("", restore)
