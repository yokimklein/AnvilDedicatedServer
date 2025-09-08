#pragma once

long* __fastcall hs_macro_function_evaluate(short function_index, long thread_index, bool initialize);
void __fastcall hs_return(long thread_index, long value);
extern long* __fastcall hs_arguments_evaluate(long thread_index, short parameter_count, const short* formal_parameters, bool initialize);