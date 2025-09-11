#include "hooks_object_deletion.h"
#include <anvil\hooks\hooks.h>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <objects\object_scripting.h>

void __cdecl item_in_unit_inventory_hook2(s_hook_registers registers)
{
    datum_index object_index = (datum_index)registers.edi;

    simulation_action_object_delete(object_index);
}

void __fastcall object_scripting_clear_all_function_variables_hook(datum_index object_index)
{
    simulation_action_object_delete(object_index);
    object_scripting_clear_all_function_variables(object_index);
}

void anvil_hooks_object_deletion_apply()
{
    // add simulation_action_object_delete back to object_delete
    hook::call(0x3FE1BE, object_scripting_clear_all_function_variables_hook);

    // deleted dropped inventory items
    hook::insert(0x484186, 0x48418D, item_in_unit_inventory_hook2, _hook_execute_replaced_first);
}