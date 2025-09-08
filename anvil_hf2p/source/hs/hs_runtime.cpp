#include "hs\hs_runtime_internal.h"
#include "cseries\cseries.h"
#include "hs\hs_function.h"
#include "hs\hs.h"

long* __fastcall hs_macro_function_evaluate(short function_index, long thread_index, bool initialize)
{
    const hs_function_definition* function = hs_function_get(function_index);

    return hs_arguments_evaluate(thread_index, function->formal_parameter_count, function->formal_parameters, initialize);
}

void __fastcall hs_return(long thread_index, long value)
{
	INVOKE(0x1210F0, hs_return, thread_index, value);
}

#pragma runtime_checks("", off)
long* __fastcall hs_arguments_evaluate(long thread_index, short parameter_count, const short* formal_parameters, bool initialize)
{
    long* result = INVOKE(0x121880, hs_arguments_evaluate, thread_index, parameter_count, formal_parameters, initialize);
	__asm add esp, 8; // Fix usercall & cleanup stack
    return result;
}
#pragma runtime_checks("", restore)