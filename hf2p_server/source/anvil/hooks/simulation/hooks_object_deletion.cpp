#include "hooks_object_deletion.h"
#include <anvil\hooks\hooks.h>
#include <Patch.hpp>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <objects\object_scripting.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)
__declspec(naked) void item_in_unit_inventory_hook2()
{
    datum_index object_index;
    __asm mov object_index, edi;

    simulation_action_object_delete(object_index);

    __asm retn;
}
#pragma runtime_checks("", restore)

void __fastcall object_scripting_clear_all_function_variables_hook(datum_index object_index)
{
    simulation_action_object_delete(object_index);
    object_scripting_clear_all_function_variables(object_index);
}

void anvil_hooks_object_deletion_apply()
{
    // add simulation_action_object_delete back to object_delete
    Hook(0x3FE1BE, object_scripting_clear_all_function_variables_hook, HookFlags::IsCall).Apply();

    // deleted dropped inventory items
    insert_hook(0x484186, 0x48418D, item_in_unit_inventory_hook2, _hook_execute_replaced_first);
}