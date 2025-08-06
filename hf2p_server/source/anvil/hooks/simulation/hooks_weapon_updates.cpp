#include "hooks_weapon_updates.h"
#include <anvil\hooks\hooks.h>
#include <Patch.hpp>
#include <units\units.h>
#include <items\weapons.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <simulation\game_interface\simulation_game_weapons.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <items\item_definitions.h>
#include <cache\cache_files.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall weapon_age_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, edi;
    weapon_delay_predicted_state(weapon_index);
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_barrel_fire_hook()
{
    datum_index weapon_index;
    DEFINE_ORIGINAL_EBP_ESP(0xC4, sizeof(weapon_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0xC0 - 0x6C];
    __asm mov weapon_index, eax;

    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_magazine_execute_reload_hook()
{
    datum_index weapon_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C, sizeof(weapon_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x10];
    __asm mov weapon_index, eax;

    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_magazine_update_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, ebx;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_report_kill_hook()
{
    datum_index weapon_index;
    DEFINE_ORIGINAL_EBP_ESP(0x18, sizeof(weapon_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x08];
    __asm mov weapon_index, eax;

    simulation_action_weapon_state_update(weapon_index);
}

// TODO: UNTESTED!! - make sure sp value is correct
__declspec(safebuffers) void __fastcall weapon_set_current_amount_hook()
{
    datum_index weapon_index;
    DEFINE_ORIGINAL_EBP_ESP(0x10, sizeof(weapon_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x04];
    __asm mov weapon_index, eax;

    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_set_total_rounds_hook()
{
    datum_index weapon_index;
    DEFINE_ORIGINAL_EBP_ESP(0x20, sizeof(weapon_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x0C];
    __asm mov weapon_index, eax;

    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_take_inventory_rounds_hook1()
{
    datum_index weapon_index;
    __asm mov weapon_index, edi;
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_take_inventory_rounds_hook2()
{
    datum_index unit_weapon_object_index;
    __asm mov unit_weapon_object_index, esi;
    simulation_action_weapon_state_update(unit_weapon_object_index);
}

__declspec(safebuffers) void __fastcall weapon_trigger_update_hook()
{
    datum_index weapon_index;
    __asm mov weapon_index, ebx
    simulation_action_weapon_state_update(weapon_index);
}

__declspec(safebuffers) void __fastcall weapon_handle_potential_inventory_item_hook()
{
    datum_index weapon_index;
    datum_index item_index;
    item_definition* item;
    item_datum* item_datum;
    DEFINE_ORIGINAL_EBP_ESP(0x40, sizeof(weapon_index) + sizeof(item_index) + sizeof(item) + sizeof(item_datum));
    
    __asm mov ecx, original_ebp;
    __asm mov eax, [ecx - 0x30];
    __asm mov weapon_index, eax;
    __asm mov eax, [ecx - 0x24];
    __asm mov item_index, eax;
    __asm mov item_datum, esi;
    
    // retrieve item definition from existing datum variable
    // retrieving the item variable directly had reliability
    // issues and would contain invalid data in certain edge cases
    item = (item_definition*)tag_get(ITEM_TAG, item_datum->definition_index);

    simulation_action_weapon_state_update(weapon_index);
    if (TEST_BIT(_object_mask_weapon, item->object.type.get()))
    {
        simulation_action_object_update(item_index, _simulation_weapon_update_ammo);
    }
}

// preserve unit_index variable
__declspec(naked) void unit_inventory_set_weapon_index_hook0()
{
    __asm mov[ebp + 4], ecx;
    __asm retn;
}

__declspec(safebuffers) void __fastcall unit_inventory_set_weapon_index_hook1()
{
    datum_index unit_index;
    short inventory_index;
    c_simulation_object_update_flags update_flags;
    DEFINE_ORIGINAL_EBP_ESP(0x18, sizeof(unit_index) + (sizeof(inventory_index) + 2) + sizeof(update_flags));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov unit_index, eax;

    __asm mov eax, original_ebp;
    __asm mov ax, [eax - 2];
    __asm mov inventory_index, ax;

    update_flags.set_flag(unit_index, (e_simulation_unit_update_flag)(inventory_index + _simulation_unit_update_weapon1_type));
    update_flags.set_flag(unit_index, (e_simulation_unit_update_flag)(inventory_index + _simulation_unit_update_weapon1_state));
    simulation_action_object_update_internal(unit_index, update_flags);
    unit_inventory_cycle_weapon_set_identifier(unit_index);
}

__declspec(safebuffers) void __fastcall unit_handle_deleted_object_hook()
{
    datum_index unit_index;
    datum_index inventory_index;
    DEFINE_ORIGINAL_EBP_ESP(0x24, sizeof(unit_index) + sizeof(inventory_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 0x08];
    __asm mov unit_index, eax;
    __asm mov inventory_index, ebx;

    unit_inventory_set_weapon_index(unit_index, inventory_index, NONE, _unit_drop_type_response_to_deletion);
}
#pragma runtime_checks("", restore)

void anvil_hooks_weapon_updates_apply()
{
	// weapon_age
	insert_hook(0x433CE6, 0x433CF0, weapon_age_hook, _hook_replace);

	// weapon_barrel_fire
	insert_hook(0x43577A, 0x43577F, weapon_barrel_fire_hook, _hook_execute_replaced_first);

	// weapon_magazine_execute_reload
	insert_hook(0x434ECE, 0x434ED6, weapon_magazine_execute_reload_hook, _hook_execute_replaced_first);

	// weapon_magazine_update (syncs Weapon > Magazines > RoundsRecharged)
	insert_hook(0x42DBB4, 0x42DBB9, weapon_magazine_update_hook, _hook_execute_replaced_first);

	// weapon_report_kill (syncs weapon ages with each kill flag, used by the energy sword)
	insert_hook(0x4339C5, 0x4339CA, weapon_report_kill_hook, _hook_execute_replaced_first);

	// weapon_set_current_amount (used by scripts & actors only, TODO: untested)
	insert_hook(0x43374B, 0x433750, weapon_set_current_amount_hook, _hook_execute_replaced_first);

	// weapon_set_total_rounds
	insert_hook(0x433479, 0x43347F, weapon_set_total_rounds_hook, _hook_execute_replaced_last);

	// weapon_take_inventory_rounds 2x
	insert_hook(0x432147, 0x43214E, weapon_take_inventory_rounds_hook1, _hook_execute_replaced_first);
	insert_hook(0x4321E1, 0x4321E8, weapon_take_inventory_rounds_hook2, _hook_execute_replaced_first);

	// weapon_trigger_update - trigger charge drain rate (plasma pistol)
	insert_hook(0x42E055, 0x42E05A, weapon_trigger_update_hook, _hook_execute_replaced_first);

	// sync taking ammo from dropped weapons
    insert_hook(0x4310CC, 0x4310D2, weapon_handle_potential_inventory_item_hook, _hook_execute_replaced_first);

	// weapon set index updates
	add_variable_space_to_stack_frame(0x426D10, 0x426DD6, 4); // Add 4 bytes of variable space to the stack frame
	insert_hook(0x426D16, 0x426D1C, unit_inventory_set_weapon_index_hook0, _hook_execute_replaced_last); // preserve unit_index in a new variable
	insert_hook(0x426D8E, 0x426DCF, unit_inventory_set_weapon_index_hook1, _hook_replace);
	insert_hook(0x426DCF, 0x426DD5, (void*)4, _hook_stack_frame_cleanup); // cleanup new variable from stack on return

	// weapon set identifier updates
	Hook(0x426CC0, unit_inventory_cycle_weapon_set_identifier, HookFlags::IsCall).Apply();

	// add inlined unit_inventory_set_weapon_index with sim updates back to unit_delete_all_weapons_internal
    hook_function(0x424E60, 0xC5, unit_delete_all_weapons_internal);

	// add inlined unit_inventory_set_weapon_index with sim updates back to unit_handle_deleted_object
    insert_hook(0x427119, 0x427178, unit_handle_deleted_object_hook, _hook_replace);
}