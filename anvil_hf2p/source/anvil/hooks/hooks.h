#pragma once
#include <cseries\cseries.h>
#include <initializer_list>

// $TODO: split some of these up into flags, this is a bit messy
enum e_hook_type
{
    _hook_replace, // overwrite original code section with inserted code
    _hook_execute_replaced_first, // executes replaced section before inserted code
    _hook_execute_replaced_last, // executes replaced section after inserted code
    _hook_stack_frame_increase,
    _hook_stack_frame_cleanup, // NOTE: ensure you cleanup after the function epilogue but before the return - best practice is to hook the entire epilogue region
};

enum e_hook_flags
{
    _hook_no_nop,
    _hook_no_preserve
};

struct s_hook_registers
{
    // read-write
    size_t eax;
    size_t ebx;
    size_t ecx;
    size_t edx;
    size_t esi;
    size_t edi;
    
    // read-only, these values of these registers in hooks will not be restored when a hook returns
    const size_t ebp;
    const size_t esp; // esp  comes first in push order so we can push without modifying its value
};

namespace hook
{
    void insert(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type, ulong hook_flags = 0, bool redirect_oob_jumps = false);
    void add_variable_space_to_stack_frame(size_t function_start, size_t function_end, size_t space_in_bytes);
    void function(size_t function_address, size_t length, void* function);
    void call(size_t call_address, void* function);
}

namespace patch
{
    ulong set_memory_protect(size_t address, ulong new_protect, size_t size = sizeof(void*));
    void nop_region(size_t address, size_t length);
    void bytes(size_t address, std::initializer_list<byte> bytes);
    void bytes(size_t address, byte* bytes, size_t size);
    void function(size_t address, void* function_address);
}

void anvil_patches_apply();
void anvil_hooks_apply();