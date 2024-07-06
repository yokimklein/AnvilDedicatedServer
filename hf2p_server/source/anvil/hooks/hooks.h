#pragma once

enum e_hook_type
{
    _hook_replace, // overwrite original code section with inserted code
    _hook_execute_replaced_first, // executes replaced section before inserted code
    _hook_execute_replaced_last, // executes replaced section after inserted code
    _hook_replace_no_nop // don't nop code section & don't run original code section, useful for if original section still needs to be accessed

};

void insert_hook(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type = _hook_execute_replaced_first, bool redirect_oob_jumps = false);
void increase_esp_offsets(size_t function_start, size_t function_end, size_t offset_increase);
void increase_positive_ebp_offsets(size_t function_start, size_t function_end, size_t offset_increase);

void anvil_patches_apply();
void anvil_hooks_apply();