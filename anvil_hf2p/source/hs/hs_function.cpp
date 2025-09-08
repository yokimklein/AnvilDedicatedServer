#include "hs_function.h"
#include "hs\hs_library_external.h"

REFERENCE_DECLARE_ARRAY(0xEA4188, const hs_function_definition*, hs_function_table, hs_function_table_count);

void __cdecl print_hs_print_1_evaluate(short function_index, long thread_index, bool initialize)
{
    const long* arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if (arguments)
    {
        hs_print((const char*)arguments[0]);
        hs_return(thread_index, 0);
    }
}

void __cdecl log_print_hs_log_print_1_evaluate(short function_index, long thread_index, bool initialize)
{
    const long* arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if (arguments)
    {
        hs_log_print((const char*)arguments[0]);
        hs_return(thread_index, 0);
    }
}