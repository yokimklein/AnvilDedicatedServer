#include "units.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_vehicles.h>

void __fastcall unit_set_actively_controlled(datum_index unit_index, bool actively_controlled)
{
    INVOKE(0x423010, unit_set_actively_controlled, unit_index, actively_controlled);
}

void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index)
{
    if (!game_is_predicted())
    {
        unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
        word new_identifier = (unit->unit.current_weapon_set.set_identifier + 1) & 0xF;
        assert(new_identifier != NONE);
        unit->unit.current_weapon_set.set_identifier = new_identifier;
        unit->unit.desired_weapon_set.set_identifier = new_identifier;
        simulation_action_object_update(unit_index, _simulation_unit_update_desired_weapon_set);
    }
}

void __fastcall unit_delete_all_weapons_internal(datum_index unit_index)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    for (long i = 0; i < 4; i++)
    {
        if (unit->unit.weapon_object_indices[i] != NONE)
        {
            unit_inventory_set_weapon_index(unit_index, i, NONE, _unit_drop_type_delete);
        }
    }
}

// I could easily rewrite this function however its strange calling convention makes this finicky to hook & replace
// disabled RTC here to prevent it from throwing an exception before the assembly corrects the stack
#pragma runtime_checks("", off)
void __fastcall unit_inventory_set_weapon_index(datum_index unit_index, datum_index inventory_index, datum_index item_index, e_unit_drop_type drop_type)
{
    INVOKE(0x426D10, unit_inventory_set_weapon_index, unit_index, inventory_index, item_index, drop_type);
    __asm add esp, 8; // cleanup stack after usercall
}

void __fastcall unit_drop_item(datum_index unit_index, datum_index equipment_index, e_unit_drop_type drop_type)
{
    INVOKE(0x426500, unit_drop_item, unit_index, equipment_index, drop_type);
    __asm add esp, 4; // cleanup stack after usercall
}
#pragma runtime_checks("", restore)

void __fastcall unit_control(datum_index unit_index, void* unit_control_data)
{
    INVOKE(0x41BA10, unit_control, unit_index, unit_control_data);

    c_simulation_object_update_flags update_flags;
    update_flags.set_flag(unit_index, _simulation_unit_update_desired_aiming_vector);
    update_flags.set_flag(unit_index, _simulation_unit_update_desired_weapon_set);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_set_aiming_vectors(datum_index unit_index, real_vector3d* aiming_vector, real_vector3d* looking_vector)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    unit->unit.aiming_vector = *aiming_vector;
    unit->unit.looking_vector = *looking_vector;
    simulation_action_object_update(unit_index, _simulation_unit_update_desired_aiming_vector);
}

void __fastcall unit_add_initial_loadout(datum_index unit_index)
{
    INVOKE(0xFB6E0, unit_add_initial_loadout, unit_index);
}

void __fastcall unit_delete_equipment(datum_index unit_index, long slot_index)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    if (slot_index < 4)
    {
        datum_index equipment_index = unit->unit.equipment_object_indices[slot_index];
        if (equipment_index != NONE)
        {
            unit_drop_item(unit_index, equipment_index, _unit_drop_type_delete);
            unit->unit.equipment_object_indices[slot_index] = NONE;
            unit->unit.equipment_pickup_time = NONE;
            simulation_action_object_update(unit_index, _simulation_unit_update_equipment);
            if (unit->unit.actor_index != NONE)
            {
                actor_handle_equipment_delete(unit->unit.actor_index);
            }
        }
    }
}

void __fastcall unit_active_camouflage_ding(datum_index unit_index, real camouflage_decay, real regrowth_seconds)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    
    real camo_regrowth = regrowth_seconds;
    if (camo_regrowth == 0.0f)
    {
        camo_regrowth = 4.0f;
    }
    camo_regrowth = FLOOR(camo_regrowth, game_tick_length());
    camo_regrowth = 1.0f / camo_regrowth;
    unit->unit.active_camouflage_regrowth = MIN(camo_regrowth, unit->unit.active_camouflage_regrowth);
    real camo_delta = unit->unit.active_camouflage - camouflage_decay;
    unit->unit.active_camouflage = camo_delta;
    if (unit->unit.flags.test(_unit_flags_camo) && camo_delta < 0.05f)
    {
        unit->unit.active_camouflage = 0.05f;
    }

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_active_camouflage_disable(datum_index unit_index, real regrowth_seconds)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    unit->unit.flags.set(_unit_flags_camo, false);
    unit->unit.active_camouflage_end_time = NONE;
    unit->unit.active_camouflage_regrowth = 1.0f / FLOOR(regrowth_seconds, game_tick_length());

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_active_camouflage_set_level(datum_index unit_index, real regrowth_seconds, long camouflage_end_time)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    unit->unit.flags.set(_unit_flags_camo, true);
    unit->unit.active_camouflage_regrowth = 1.0f / FLOOR(regrowth_seconds, game_tick_length());
    if (game_is_authoritative())
    {
        unit->unit.active_camouflage_end_time = camouflage_end_time;
    }

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_active_camouflage_set_maximum(datum_index unit_index, real camouflage_maximum)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    unit->unit.active_camouflage_maximum = CLAMP(camouflage_maximum, 0.0f, 1.0f);

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

long unit_get_current_or_last_weak_player_index(datum_index unit_index)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    if (unit->unit.player_index != NONE)
        return unit->unit.player_index;
    return unit->unit.last_weak_player_index;
}