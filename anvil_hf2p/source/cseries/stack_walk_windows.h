#pragma once

struct s_file_reference;
struct _CONTEXT;

extern const char* symbol_name_from_address(ulong routine_address, long* out_symbol_offset);
extern void walk_stack(ulong* routine_addresses, ulong number_of_levels, short levels_to_ignore, long* levels_dumped);
extern void stack_walk_with_context_internal(const s_file_reference* file, short levels_to_ignore, _CONTEXT* context, long number_of_levels, ulong* routine_addresses, long* out_levels_dumped);
extern void stack_walk_print(s_file_reference* file, _CONTEXT* context, long levels_dumped, const ulong* routine_addresses);
extern void stack_walk_with_context_common(s_file_reference* file, short levels_to_ignore, _CONTEXT* context);
extern void stack_walk_with_context(s_file_reference* file, short levels_to_ignore, _CONTEXT* context);
extern void stack_walk_to_file(s_file_reference* file, short levels_to_ignore);
extern void stack_walk(short levels_to_ignore);