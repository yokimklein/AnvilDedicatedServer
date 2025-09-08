#pragma once

using binary_search_comparison_proc_t = long __cdecl(const void* a, const void* b, const void* c);
void* __fastcall binary_search_elements(void const* search_for, void const* base, long element_size, ulong count, binary_search_comparison_proc_t comparison_function, void const* a6);