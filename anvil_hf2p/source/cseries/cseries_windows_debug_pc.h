#pragma once

struct _EXCEPTION_POINTERS;
struct s_exception_information;

long __fastcall exceptions_update();
bool has_cached_exception();
const char* __fastcall exception_code_get_string(ulong code);
bool is_debugger_present();
void cache_exception_information(_EXCEPTION_POINTERS* exception_pointers);
void crashdump_from_exception();
long generic_exception_filter(ulong exception_code, _EXCEPTION_POINTERS* exception_pointers);
void build_exception_information(_EXCEPTION_POINTERS* exception_pointers, s_exception_information* exception_information);
void __fastcall build_exception_pointers(_EXCEPTION_POINTERS* exception_pointers);