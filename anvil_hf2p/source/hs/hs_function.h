#pragma once
#include "hs\hs_runtime_internal.h"
#include "hs\hs_enum.h"
#include "cseries\cseries.h"

using hs_function_evaluate_t = void __cdecl(short function_index, long thread_index, bool initialize);

struct hs_function_definition
{
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;
	word flags;
	hs_function_evaluate_t* evaluate;
	const char* parameters;

	short formal_parameter_count;
	union
	{
		__pragma(warning(disable : 4200)) c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> formal_parameters_[];
		__pragma(warning(disable : 4200)) short formal_parameters[];
	};
};
static_assert(sizeof(hs_function_definition) == 0x10);

long const hs_function_table_count = 1698;
extern const hs_function_definition* (&hs_function_table)[hs_function_table_count];

extern void __cdecl print_hs_print_1_evaluate(short function_index, long thread_index, bool initialize);
extern void __cdecl log_print_hs_log_print_1_evaluate(short function_index, long thread_index, bool initialize);
extern void __cdecl events_suppress_display_events_suppress_output_1_evaluate(short function_index, long thread_index, bool initialize);