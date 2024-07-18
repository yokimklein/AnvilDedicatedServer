#pragma once

// ogebp = ogesp + ogsp
// ogesp = esp + default stack space + original_ebp/esp size + variable space + 3x pre-call pushes & return address + 4x hook func prologue pushes
// NOTE: this overwrites eax
#define DEFINE_ORIGINAL_EBP_ESP(original_sp, variable_space)                    \
    long original_ebp;                                                          \
    long original_esp;                                                          \
    __asm { mov original_esp, esp }                                             \
    original_esp += 0x40 + 0x08 + (long)variable_space + 0x10 + 0x10;           \
    original_ebp = (long)original_esp + (long)original_sp;

enum e_hook_type
{
    _hook_replace, // overwrite original code section with inserted code
    _hook_execute_replaced_first, // executes replaced section before inserted code
    _hook_execute_replaced_last, // executes replaced section after inserted code
    _hook_replace_no_nop, // don't nop code section & don't run original code section, useful for if original section still needs to be accessed
    _hook_stack_frame_increase,
    _hook_stack_frame_cleanup
};

void insert_hook(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type = _hook_execute_replaced_first, bool redirect_oob_jumps = false);
void add_variable_space_to_stack_frame(size_t function_start, size_t function_end, size_t space_in_bytes);

void anvil_patches_apply();
void anvil_hooks_apply();