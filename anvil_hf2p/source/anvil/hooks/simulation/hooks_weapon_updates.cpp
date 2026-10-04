#include "hooks_weapon_updates.h"
#include <anvil\hooks\hooks.h>
#include <units\units.h>
#include <items\weapons.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <simulation\game_interface\simulation_game_weapons.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <items\item_definitions.h>
#include <cache\cache_files.h>

void __cdecl weapon_age_hook(s_hook_registers& registers)
{
    datum_index weapon_index = (datum_index)registers.edi;

    weapon_delay_predicted_state(weapon_index);
    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_barrel_fire_hook(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index weapon_index = *(datum_index*)(registers.esp + 0xC0 - 0x6C);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index weapon_index = *(datum_index*)(registers.esp + 0x80 - 0x64);
#endif

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_magazine_execute_reload_hook(s_hook_registers& registers)
{
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x10);

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_magazine_update_hook(s_hook_registers& registers)
{
    datum_index weapon_index = (datum_index)registers.ebx;

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_report_kill_hook(s_hook_registers& registers)
{
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x08);

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_set_current_amount_hook(s_hook_registers& registers)
{
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x04);

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_set_total_rounds_hook(s_hook_registers& registers)
{
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x0C);

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_take_inventory_rounds_hook1(s_hook_registers& registers)
{
    datum_index weapon_index = (datum_index)registers.edi;

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_take_inventory_rounds_hook2(s_hook_registers& registers)
{
    datum_index unit_weapon_object_index = (datum_index)registers.esi;

    simulation_action_weapon_state_update(unit_weapon_object_index);
}

void __cdecl weapon_trigger_update_hook(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index weapon_index = (datum_index)registers.ebx;
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index weapon_index = (datum_index)registers.edi;
#endif

    simulation_action_weapon_state_update(weapon_index);
}

void __cdecl weapon_handle_potential_inventory_item_hook(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x30);
    datum_index item_index = *(datum_index*)(registers.ebp - 0x24);
    item_datum* item_data = (item_datum*)registers.esi;
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index weapon_index = *(datum_index*)(registers.ebp - 0x2C);
    datum_index item_index = *(datum_index*)(registers.ebp - 0x30);
    item_datum* item_data = (item_datum*)registers.edi;
#endif

    // retrieve item definition from existing datum variable
    // retrieving the item variable directly had reliability
    // issues and would contain invalid data in certain edge cases
    item_definition* item = (item_definition*)tag_get(ITEM_TAG, item_data->definition_index);

    simulation_action_weapon_state_update(weapon_index);
    if (TEST_BIT(_object_mask_weapon, item->object.type.get()))
    {
        simulation_action_object_update(item_index, _simulation_weapon_update_ammo);
    }
}

// preserve unit_index variable
void __cdecl unit_inventory_set_weapon_index_hook0(s_hook_registers& registers)
{
    datum_index* unit_index = (datum_index*)(registers.ebp + 0x04);
    *unit_index = registers.ecx;
}

void __cdecl unit_inventory_set_weapon_index_hook1(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp + 0x04);
    short inventory_index = *(short*)(registers.ebp - 0x02);

    c_simulation_object_update_flags update_flags;
    update_flags.set_flag(unit_index, (e_simulation_unit_update_flag)(inventory_index + _simulation_unit_update_weapon1_type));
    update_flags.set_flag(unit_index, (e_simulation_unit_update_flag)(inventory_index + _simulation_unit_update_weapon1_state));
    simulation_action_object_update_internal(unit_index, update_flags);
    unit_inventory_cycle_weapon_set_identifier(unit_index);
}

void __cdecl unit_handle_deleted_object_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp + 0x08);
    datum_index inventory_index = (datum_index)registers.ebx;

    unit_inventory_set_weapon_index(unit_index, inventory_index, NONE, _unit_drop_type_response_to_deletion);
}

void anvil_hooks_weapon_updates_apply()
{
	// weapon_age
	hook::insert(ADDRESS_WEAPON_AGE_HOOK, ADDRESS_WEAPON_AGE_HOOK_RETURN, weapon_age_hook, _hook_replace);

	// weapon_barrel_fire
	hook::insert(ADDRESS_WEAPON_BARREL_FIRE_HOOK, ADDRESS_WEAPON_BARREL_FIRE_HOOK_RETURN, weapon_barrel_fire_hook, _hook_execute_replaced_first);

	// weapon_magazine_execute_reload
	hook::insert(ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK, ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK_RETURN, weapon_magazine_execute_reload_hook, _hook_execute_replaced_first);

	// weapon_magazine_update (syncs Weapon > Magazines > RoundsRecharged)
	hook::insert(ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK, ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK_RETURN, weapon_magazine_update_hook, _hook_execute_replaced_first);

	// weapon_report_kill (syncs weapon ages with each kill flag, used by the energy sword)
	hook::insert(ADDRESS_WEAPON_REPORT_KILL_HOOK, ADDRESS_WEAPON_REPORT_KILL_HOOK_RETURN, weapon_report_kill_hook, _hook_execute_replaced_first);

	// weapon_set_current_amount (used by scripts & actors only, TODO: untested)
	hook::insert(ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK, ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK_RETURN, weapon_set_current_amount_hook, _hook_execute_replaced_first);

	// weapon_set_total_rounds
	hook::insert(ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK, ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK_RETURN, weapon_set_total_rounds_hook, _hook_execute_replaced_last);

	// weapon_take_inventory_rounds 2x
	hook::insert(ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1, ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1_RETURN, weapon_take_inventory_rounds_hook1, _hook_execute_replaced_first);
	hook::insert(ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2, ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2_RETURN, weapon_take_inventory_rounds_hook2, _hook_execute_replaced_first);

	// weapon_trigger_update - trigger charge drain rate (plasma pistol)
	hook::insert(ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK, ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK_RETURN, weapon_trigger_update_hook, _hook_execute_replaced_first);

	// sync taking ammo from dropped weapons
    hook::insert(ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK, ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK_RETURN, weapon_handle_potential_inventory_item_hook, _hook_execute_replaced_first);

	// weapon set index updates
	hook::add_variable_space_to_stack_frame(ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX, ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_END, 4); // Add 4 bytes of variable space to the stack frame
	hook::insert(ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0, ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0_RETURN, unit_inventory_set_weapon_index_hook0, _hook_execute_replaced_last); // preserve unit_index in a new variable
	hook::insert(ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1, ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1_RETURN, unit_inventory_set_weapon_index_hook1, _hook_replace);
	hook::insert(ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP, ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP_RETURN, (void*)4, _hook_stack_frame_cleanup); // cleanup new variable from stack on return

	// weapon set identifier updates
    hook::function(ADDRESS_UNIT_INVENTORY_CYCLE_WEAPON_SET_IDENTIFIER, 0x4E, unit_inventory_cycle_weapon_set_identifier);

	// add inlined unit_inventory_set_weapon_index with sim updates back to unit_delete_all_weapons_internal
    hook::function(ADDRESS_UNIT_DELETE_ALL_WEAPONS_INTERNAL, 0xC5, unit_delete_all_weapons_internal);

	// add inlined unit_inventory_set_weapon_index with sim updates back to unit_handle_deleted_object
    hook::insert(ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK, ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK_RETURN, unit_handle_deleted_object_hook, _hook_replace);
}