#include "units.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_objects.h>

void __fastcall unit_set_actively_controlled(datum_index unit_index, bool actively_controlled)
{
    INVOKE(0x423010, unit_set_actively_controlled, unit_index, actively_controlled);
}

void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index)
{
    if (!game_is_predicted())
    {
        s_unit_data* unit = (s_unit_data*)object_get(unit_index);
        word cycled_identifier = (unit->current_weapon_set.set_identifier + 1) & 0xF;
        unit->current_weapon_set.set_identifier = cycled_identifier;
        unit->desired_weapon_set.set_identifier = cycled_identifier;
        simulation_action_object_update(unit_index, _simulation_unit_update_desired_weapon_set);
    }
}

void __fastcall unit_delete_all_weapons_internal(datum_index unit_index)
{
    s_unit_data* unit = (s_unit_data*)object_get(unit_index);
    long counter = 0;
    do
    {
        if (unit->weapon_object_indices[counter] != -1)
            unit_inventory_set_weapon_index(unit_index, counter, -1, _unit_drop_type_delete);
        counter++;
    }
    while (counter < 4);
}

// I could easily rewrite this function however its strange calling convention makes this finicky to hook & replace
// disabled RTC here to prevent it from throwing an exception before the assembly corrects the stack
#pragma runtime_checks("", off)
void __fastcall unit_inventory_set_weapon_index(datum_index unit_index, datum_index inventory_index, datum_index item_index, e_unit_drop_type drop_type)
{
    INVOKE(0x426D10, unit_inventory_set_weapon_index, unit_index, inventory_index, item_index, drop_type);
    __asm add esp, 8; // cleanup stack after usercall
}
#pragma runtime_checks("", restore)

void __fastcall unit_control(datum_index unit_index, void* unit_control_data)
{
    INVOKE(0x41BA10, unit_control, unit_index, unit_control_data);

    c_simulation_object_update_flags update_flags{};
    update_flags.set_flag(unit_index, _simulation_unit_update_desired_aiming_vector);
    update_flags.set_flag(unit_index, _simulation_unit_update_desired_weapon_set);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_set_aiming_vectors(datum_index unit_index, real_vector3d* aiming_vector, real_vector3d* looking_vector)
{
    s_unit_data* unit = (s_unit_data*)object_get(unit_index);

    unit->aiming_vector = *aiming_vector;
    unit->looking_vector = *looking_vector;
    simulation_action_object_update(unit_index, _simulation_unit_update_desired_aiming_vector);
}

void __fastcall unit_add_initial_loadout(datum_index unit_index)
{
    INVOKE(0xFB6E0, unit_add_initial_loadout, unit_index);
}