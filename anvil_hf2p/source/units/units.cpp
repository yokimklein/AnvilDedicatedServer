#include "units.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_vehicles.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <game\game_time.h>

void __fastcall unit_set_actively_controlled(datum_index unit_index, bool actively_controlled)
{
    INVOKE(0x423010, unit_set_actively_controlled, unit_index, actively_controlled);
}

void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index)
{
    if (game_is_predicted())
    {
        return;
    }

    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    word new_identifier = (unit->unit.current_weapon_set.set_identifier + 1) & 0xF;
    ASSERT(new_identifier != NONE);
    unit->unit.current_weapon_set.set_identifier = new_identifier;
    unit->unit.desired_weapon_set.set_identifier = new_identifier;
    simulation_action_object_update(unit_index, _simulation_unit_update_desired_weapon_set);
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
    if (slot_index >= 4)
    {
        return;
    }

    datum_index equipment_index = unit->unit.equipment_object_indices[slot_index];
    if (equipment_index == NONE)
    {
        return;
    }

    unit_drop_item(unit_index, equipment_index, _unit_drop_type_delete);
    unit->unit.equipment_object_indices[slot_index] = NONE;
    unit->unit.equipment_pickup_time = NONE;
    simulation_action_object_update(unit_index, _simulation_unit_update_equipment);
    if (unit->unit.actor_index != NONE)
    {
        actor_handle_equipment_delete(unit->unit.actor_index);
    }
}

void __vectorcall unit_active_camouflage_ding(datum_index unit_index, real camouflage_decay, real regrowth_seconds)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    
    real camo_regrowth = regrowth_seconds;
    if (camo_regrowth == 0.0f)
    {
        camo_regrowth = 4.0f;
    }
    camo_regrowth = FLOOR(camo_regrowth, game_tick_length());
    camo_regrowth = 1.0f / camo_regrowth;
    unit->unit.active_camouflage_regrowth = MIN(unit->unit.active_camouflage_regrowth, camo_regrowth);
    real camo_delta = unit->unit.active_camouflage - camouflage_decay;
    unit->unit.active_camouflage = camo_delta;
    if (unit->unit.flags.test(_unit_flags_camo) && camo_delta < 0.05f)
    {
        unit->unit.active_camouflage = 0.05f;
    }

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
    {
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    }
    else
    {
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    }
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __vectorcall unit_active_camouflage_disable(datum_index unit_index, real regrowth_seconds)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    unit->unit.flags.set(_unit_flags_camo, false);
    unit->unit.active_camouflage_end_time = NONE;
    unit->unit.active_camouflage_regrowth = 1.0f / FLOOR(regrowth_seconds, game_tick_length());

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
    {
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    }
    else
    {
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    }
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __vectorcall unit_active_camouflage_set_level(datum_index unit_index, real regrowth_seconds, long camouflage_end_time)
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
    {
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    }
    else
    {
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    }
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __fastcall unit_active_camouflage_set_maximum(datum_index unit_index, real camouflage_maximum)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    unit->unit.active_camouflage_maximum = CLAMP(camouflage_maximum, 0.0f, 1.0f);

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
    {
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    }
    else
    {
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    }
    simulation_action_object_update_internal(unit_index, update_flags);
}

long unit_get_current_or_last_weak_player_index(datum_index unit_index)
{
    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    if (unit->unit.player_index != NONE)
    {
        return unit->unit.player_index;
    }
    return unit->unit.last_weak_player_index;
}

long __fastcall unit_get_equipment_consumable_slot(datum_index unit_index, long equipment_definition_index)
{
    // these checks didn't originally exist, but I feel like they should
    if (unit_index == NONE)
    {
        return NONE;
    }
    if (equipment_definition_index == NONE)
    {
        return NONE;
    }

    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);

    for (long consumable_slot = 0; consumable_slot < k_consumable_slots; consumable_slot++)
    {
        datum_index equipment_index = unit->unit.equipment_object_indices[consumable_slot];
        if (equipment_index == NONE)
        {
            continue;
        }

        object_datum* equipment = (object_datum*)object_get_and_verify_type(equipment_index, _object_mask_equipment);
        if (equipment->definition_index != equipment_definition_index)
        {
            continue;
        }

        return consumable_slot;
    }
    return NONE;
}

void __fastcall unit_handle_equipment_energy_cost(datum_index unit_index, long equipment_definition_index)
{
    if (game_is_multiplayer() && game_is_predicted())
    {
        return;
    }

    if (unit_index == NONE)
    {
        return;
    }

    unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
    if (unit->unit.player_index == NONE)
    {
        return;
    }

    long consumable_slot = unit_get_equipment_consumable_slot(unit_index, equipment_definition_index);
    if (consumable_slot < 0)
    {
        return;
    }

    TLS_DATA_GET_VALUE_REFERENCE(players);
    player_datum* player = (player_datum*)datum_get(players, unit->unit.player_index);
    if (!player_has_consumable(player, consumable_slot))
    {
        return;
    }

    float energy_regeneration_rate = 1.0f;
    if (unit->unit.consumable_energy_restored_game_time != NONE)
    {
        energy_regeneration_rate = static_cast<float>(unit->unit.consumable_energy_restored_game_time - game_time_get());
        energy_regeneration_rate /= static_cast<float>(player_get_energy_regeneration_duration(player, unit->unit.consumable_energy_level));
    }
    unit->unit.consumable_energy_level -= player_get_consumable_cost(player, consumable_slot);
    unit->unit.consumable_energy_restored_game_time = player_get_consumable_energy_restored_game_time(player, unit->unit.consumable_energy_level, energy_regeneration_rate);
    simulation_action_object_update<e_simulation_unit_update_flag>(unit_index, _simulation_unit_update_consumable_energy);

    // nullsub here calls player_get_loadout_consumable, dedi related perhaps?

    float cooldown_boost = player_get_modifier_value_real(player, _cooldown_boost, 0.0f) + 1.0f;
    float cooldown_init = player_get_consumable_cooldown_init(player, consumable_slot);
    float cooldown = player_get_consumable_cooldown(player, consumable_slot);
    player_set_consumable_cooldown(player, cooldown * cooldown_boost, cooldown_init * cooldown_boost);
    simulation_action_object_update<e_simulation_unit_update_flag>(unit_index, _simulation_unit_update_equipment_charges);
    simulation_action_game_engine_player_update(unit->unit.player_index, _simulation_player_update_consumable_supression);
}
