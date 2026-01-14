#include "hooks_object_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game.h>
#include <game\players.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <objects\objects.h>
#include <simulation\game_interface\simulation_game_generics.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_vehicles.h>
#include <simulation\game_interface\simulation_game_items.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <simulation\game_interface\simulation_game_device_machines.h>
#include <units\bipeds.h>
#include <units\units.h>
#include <motor\action_system.h>
#include <game\game_results.h>
#include <game\game_time.h>

void __cdecl object_update_hook(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    TLS_DATA_GET_VALUE_REFERENCE(object_headers);
    object_header_datum* object_header = (object_header_datum*)datum_get(object_headers, object_index);
    
    // we've left the scope of this if check where we've hooked, so we need to check it again
    if (!object_header->flags.test(_object_header_do_not_update_bit))
    {
        if (object_needs_rigid_body_update(object_index))
        {
            simulation_action_object_update(object_index, _simulation_generic_update_rigid_body);
        }
    }
}

void __cdecl c_map_variant__remove_object_hook(s_hook_registers& registers)
{
    // map_variant_placement->object_index
    datum_index object_index = *(datum_index*)(registers.edi + 0x04);

    if (game_is_authoritative())
    {
        simulation_action_object_update(object_index, _simulation_object_update_map_variant_index);
    }
}

void __cdecl c_map_variant__unknown4_hook1(s_hook_registers& registers)
{
    // map_variant->objects[object_placement_index].object_index
    datum_index object_index = *(datum_index*)(registers.ebx + registers.esi + 0x134);

    c_simulation_object_update_flags update_flags;
    update_flags.set_flag(object_index, _simulation_object_update_position);
    update_flags.set_flag(object_index, _simulation_object_update_forward_and_up);
    simulation_action_object_update_internal(object_index, update_flags);
}

void __cdecl c_map_variant__unknown4_hook2(s_hook_registers& registers)
{
    // map_variant->objects[object_placement_index].object_index
    datum_index object_index = *(datum_index*)(registers.ebx + registers.esi + 0x134);

    simulation_action_object_update(object_index, _simulation_object_update_parent_state);
}

void __cdecl player_set_unit_index_hook2(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.edi + 0x30);

    simulation_action_object_update(unit_index, _simulation_unit_update_assassination_data);
}

void __cdecl unit_died_hook(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.eax;
    datum_index unit_index = (datum_index)registers.esi;

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

void __cdecl grenade_throw_move_to_hand_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp - 0x08);

    simulation_action_object_update(unit_index, _simulation_unit_update_grenade_counts);
}

void __cdecl unit_add_grenade_to_inventory_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp - 0x08);

    simulation_action_object_update(unit_index, _simulation_unit_update_grenade_counts);
}

void __cdecl unit_add_equipment_to_inventory_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.esp + 0x20 - 0x0C);

    c_simulation_object_update_flags update_flags;
    update_flags.set_flag(unit_index, _simulation_unit_update_equipment);
    update_flags.set_flag(unit_index, _simulation_unit_update_equipment_charges);
    simulation_action_object_update_internal(unit_index, update_flags);
}

void __cdecl unit_update_control_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;

    simulation_action_object_update(unit_index, _simulation_unit_update_desired_aiming_vector);
}

// preserve player_object_index variable
void __cdecl unit_add_initial_loadout_hook0(s_hook_registers& registers)
{
    datum_index* player_object_index = (datum_index*)(registers.ebp + 0x04);
    *player_object_index = registers.ecx;
}

// syncs grenade counts on spawn
void __cdecl unit_add_initial_loadout_hook1(s_hook_registers& registers)
{
    datum_index player_object_index = *(datum_index*)(registers.ebp + 0x04);

    simulation_action_object_update(player_object_index, _simulation_unit_update_grenade_counts);
}

// syncs revenge shield bonus on spawn
void __cdecl unit_add_initial_loadout_hook2(s_hook_registers& registers)
{
    datum_index player_object_index = *(datum_index*)(registers.ebp + 0x04);

    simulation_action_object_update(player_object_index, _simulation_object_update_shield_vitality);
}

void __cdecl projectile_attach_hook(s_hook_registers& registers)
{
    datum_index projectile_index = *(datum_index*)(registers.esp + 0x58 - 0x48);

    simulation_action_object_update(projectile_index, _simulation_object_update_parent_state);
}

void __cdecl unit_set_aiming_vectors_hook1(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;
    s_simulation_unit_state_data* unit_state_data = (s_simulation_unit_state_data*)registers.edi;

    unit_set_aiming_vectors(unit_index, &unit_state_data->desired_aiming_vector, &unit_state_data->desired_aiming_vector);
}

void __cdecl unit_set_aiming_vectors_hook2(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.esp + 0xE0 - 0xB4);
    real_vector3d* forward = (real_vector3d*)registers.edx;

    unit_set_aiming_vectors(unit_index, forward, forward);
}

void __cdecl unit_set_aiming_vectors_hook3(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;
    real_vector3d* forward = (real_vector3d*)(registers.esi + 0x8BC);

    unit_set_aiming_vectors(unit_index, forward, forward);
}

void __cdecl unit_set_aiming_vectors_hook4(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.edi;
    real_vector3d* forward = *(real_vector3d**)(registers.ebp - 0x2C);

    unit_set_aiming_vectors(unit_index, forward, forward);
}

void __cdecl equipment_activate_hook2(s_hook_registers& registers)
{
    datum_index equipment_index = *(datum_index*)(registers.esp + 0x358 - 0x328);

    simulation_action_object_update(equipment_index, _simulation_item_update_equipment_creation_time);
}

void __cdecl unit_update_energy_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;

    simulation_action_object_update(unit_index, _simulation_unit_update_consumable_energy);
}

// not referenced anymore? must be from an old hook $TODO: investigate
//// preserve unit_index variable
//void __cdecl unit_handle_equipment_energy_cost_hook0(s_hook_registers& registers)
//{
//    datum_index* unit_index = (datum_index*)(registers.ebp + 0x04);
//    *unit_index = registers.ecx;
//}
//
//void __cdecl unit_handle_equipment_energy_cost_hook1(s_hook_registers& registers)
//{
//    datum_index unit_index = *(datum_index*)(registers.ebp + 0x04);
//
//    simulation_action_object_update(unit_index, _simulation_unit_update_consumable_energy);
//}

void __cdecl unit_set_hologram_hook(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.esi;
    datum_index unit_index = (datum_index)registers.ebx;

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

void __cdecl object_apply_damage_aftermath_hook(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.esi;

    TLS_DATA_GET_VALUE_REFERENCE(players);
    player_datum* player_data = (player_datum*)datum_get(players, unit->unit.player_index);
    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

void __cdecl unit_update_damage_hook(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.esi;
    datum_index unit_index = *(datum_index*)(registers.esp + 0x70 - 0x64);

    if (unit->object.object_identifier.m_type.get() == _object_type_vehicle)
    {
        simulation_action_object_update(unit_index, _simulation_vehicle_update_seat_power);
    }
}

void __cdecl unit_respond_to_emp_hook(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.edi;
    datum_index unit_index = (datum_index)registers.esi;

    if (unit->object.object_identifier.m_type.get() == _object_type_vehicle)
    {
        simulation_action_object_update(unit_index, _simulation_vehicle_update_seat_power);
    }
}

void __cdecl unit_delete_current_equipment_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp + 0x08);

    simulation_action_object_update(unit_index, _simulation_unit_update_equipment);
}

void __cdecl unit_place_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.edi;

    for (long i = 0; i < 4; i++)
    {
        unit_delete_equipment(unit_index, i);
    }
}

void __cdecl unit_add_initial_loadout_hook3(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;
    long slot_index = (long)registers.ecx;

    unit_delete_equipment(unit_index, slot_index);
}

void __cdecl unit_add_initial_loadout_hook4(s_hook_registers& registers)
{
    registers.esi = *(long*)(registers.ebp - 0x08);
}

// TODO: figure out how to trigger this to test
void __cdecl c_simulation_unit_entity_definition__apply_object_update_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ebx;
    long slot_index = (long)registers.ecx;

    unit_delete_equipment(unit_index, slot_index);
}

void __cdecl throw_release_hook2(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.esp + 0x58 - 0x44);

    unit_active_camouflage_ding(unit_index, 0.6f, 0.0f);
}

__declspec(naked) void unit_active_camouflage_ding_hook()
{
    // this is almost a vectorcall, but the real arg is in xmm1 instead of xmm0 for some reason
    __asm
    {
        movaps xmm0, xmm1
        movaps xmm1, xmm2
        call unit_active_camouflage_ding
        retn;
    }
}

__declspec(naked) void unit_active_camouflage_disable_hook()
{
    // this is almost a vectorcall, but the real arg is in xmm1 instead of xmm0 for some reason
    __asm
    {
        movaps xmm0, xmm1
        call unit_active_camouflage_disable
        retn;
    }
}

__declspec(naked) void unit_active_camouflage_set_level_hook()
{
    // this is almost a vectorcall, but the real arg is in xmm1 instead of xmm0 for some reason
    __asm
    {
        movaps xmm0, xmm1
        call unit_active_camouflage_set_level
        retn;
    }
}

void __cdecl unit_scripting_set_active_camo_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ecx;
    real regrowth_seconds;
    __asm
    {
        movss regrowth_seconds, xmm2;
    }
    unit_active_camouflage_set_level(unit_index, regrowth_seconds, NONE);
}

void __cdecl player_update_invisibility_hook(s_hook_registers& registers)
{
    player_datum* player = (player_datum*)registers.esi;
    real camouflage_maximum;
    __asm
    {
        movss camouflage_maximum, xmm1;
    }
    unit_active_camouflage_set_level(player->unit_index, 4.0f, NONE);
    unit_active_camouflage_set_maximum(player->unit_index, camouflage_maximum);
}

void __cdecl c_simulation_unit_entity_definition__apply_object_update_hook2(s_hook_registers& registers)
{
    s_simulation_unit_state_data* unit_state_data = (s_simulation_unit_state_data*)registers.esi;
    datum_index unit_index = (datum_index)registers.ebx;

    if (unit_state_data->active_camo_active)
    {
        unit_active_camouflage_set_level(unit_index, 4.0f, NONE);
    }
    else
    {
        unit_active_camouflage_disable(unit_index, 4.0f);
    }
}

void __cdecl c_simulation_vehicle_entity_definition__apply_object_update_hook(s_hook_registers& registers)
{
    s_simulation_vehicle_state_data* vehicle_state_data = (s_simulation_vehicle_state_data*)registers.edi;
    datum_index vehicle_index = *(datum_index*)(registers.ebp + 0x08);

    if (vehicle_state_data->active_camo_active)
    {
        unit_active_camouflage_set_level(vehicle_index, 4.0f, NONE);
    }
    else
    {
        unit_active_camouflage_disable(vehicle_index, 4.0f);
    }
}

void __cdecl actor_set_active_camo_hook(s_hook_registers& registers)
{
    actor_datum* actor = (actor_datum*)registers.edi;
    real regrowth_seconds;
    __asm
    {
        movss regrowth_seconds, xmm2;
    }
    unit_active_camouflage_set_level(actor->meta.unit_index, regrowth_seconds, NONE);
}

void __cdecl unit_active_camouflage_set_maximum_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.ecx;
    real camouflage_maximum;
    __asm
    {
        movss camouflage_maximum, xmm1;
    }
    unit_active_camouflage_set_maximum(unit_index, camouflage_maximum);
}

void __cdecl biped_new_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.edi;
    real camouflage_maximum;
    __asm
    {
        movss camouflage_maximum, xmm0;
    }
    unit_active_camouflage_set_maximum(unit_index, camouflage_maximum);
}

void __cdecl vehicle_new_hook(s_hook_registers& registers)
{
    datum_index vehicle_index = (datum_index)registers.ebx;
    real camouflage_maximum;
    __asm
    {
        movss camouflage_maximum, xmm0;
    }
    unit_active_camouflage_set_maximum(vehicle_index, camouflage_maximum);
}

// preserve unit_index variable
void __cdecl unit_update_active_camouflage_hook0(s_hook_registers& registers)
{
    datum_index* unit_index = (datum_index*)(registers.ebp + 0x04);
    *unit_index = registers.ecx;
}

void __cdecl unit_update_active_camouflage_hook1(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.esi;
    datum_index unit_index = *(datum_index*)(registers.ebp + 0x04);

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

void __cdecl unit_action_assassinate_finished_hook(s_hook_registers& registers)
{
    datum_index mover_index = (datum_index)registers.edx;

    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

void __cdecl unit_action_assassinate_submit_hook1(s_hook_registers& registers)
{
    datum_index mover_index = (datum_index)registers.esi;

    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

void __cdecl unit_action_assassinate_submit_hook2(s_hook_registers& registers)
{
    s_action_request* request = *(s_action_request**)(registers.ebp + 0x0C);

    simulation_action_object_update(request->assassination.victim_unit_index, _simulation_unit_update_assassination_data);
}

void __cdecl unit_action_assassinate_interrupted_hook(s_hook_registers& registers)
{
    datum_index mover_index = (datum_index)registers.edi;

    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

void __cdecl motor_task_enter_seat_internal_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.edi;

    simulation_action_object_update(unit_index, _simulation_unit_update_parent_vehicle);
}

void __cdecl motor_animation_exit_seat_immediate_internal_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.esi;

    simulation_action_object_update(unit_index, _simulation_unit_update_parent_vehicle);
}

void __cdecl device_group_set_actual_value_hook1(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.esi;

    object_wake(device_index);
    simulation_action_object_update(device_index, _simulation_device_update_power);
}

void __cdecl device_group_set_actual_value_hook2(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.esi;

    object_wake(device_index);
    simulation_action_object_update(device_index, _simulation_device_update_position);
}

void __cdecl device_group_set_desired_value_hook1(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.esi;

    simulation_action_object_update(device_index, _simulation_device_update_power_group);
}

void __cdecl device_group_set_desired_value_hook2(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.esi;

    simulation_action_object_update(device_index, _simulation_device_update_position_group);
}

void __cdecl device_set_power_hook(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.edi;

    simulation_action_object_update(device_index, _simulation_device_update_power);
}

void __cdecl machine_update_hook(s_hook_registers& registers)
{
    datum_index device_index = (datum_index)registers.esi;

    simulation_action_object_update(device_index, _simulation_device_update_position);
}

// preserve vehicle_index variable
void __cdecl c_vehicle_auto_turret__track_auto_target_hook0(s_hook_registers& registers)
{
    datum_index* vehicle_index = (datum_index*)(registers.ebp + 0x04);
    *vehicle_index = registers.ecx;
}

void __cdecl c_vehicle_auto_turret__track_auto_target_hook1(s_hook_registers& registers)
{
    datum_index vehicle_index = *(datum_index*)(registers.ebp + 0x04);

    simulation_action_object_update(vehicle_index, _simulation_vehicle_update_auto_turret_tracking);
}

void __cdecl c_vehicle_auto_turret__track_auto_target_hook2(s_hook_registers& registers)
{
    datum_index vehicle_index = *(datum_index*)(registers.ebp + 0x04);

    simulation_action_object_update(vehicle_index, _simulation_vehicle_update_auto_turret);
}

void __cdecl object_move_position_hook(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    simulation_action_object_update(object_index, _simulation_object_update_position);
}

void __cdecl game_engine_update_player_hook(s_hook_registers& registers)
{
    long absolute_player_index = *(long*)(registers.ebp - 0x0C);
    player_datum* player = (player_datum*)registers.edi;

    if (!(game_time_get() % game_tick_rate()))
    {
        game_results_statistic_increment(absolute_player_index, _game_team_none, _game_results_statistic_seconds_alive, 1);
    }
    if (!(game_time_get() % game_seconds_integer_to_ticks(5)))
    {
        c_simulation_object_update_flags flags;
        flags.set_flag(player->unit_index, _simulation_object_update_position);
        simulation_action_object_force_update(player->unit_index, flags);
    }
}

void __fastcall player_set_unit_index_hook1(datum_index unit_index, bool actively_controlled)
{
    simulation_action_object_update(unit_index, _simulation_unit_update_control);
    unit_set_actively_controlled(unit_index, actively_controlled);
}

void anvil_hooks_object_updates_apply()
{
    // add simulation_action_object_update back to object_update
    hook::insert(0x404907, 0x40490E, object_update_hook, _hook_execute_replaced_last);

    // add simulation_action_object_update back to player_set_facing
    hook::function(0xB6300, 0xAE, player_set_facing);

    // c_map_variant::remove_object - should fix map variant object respawn times
    hook::insert(0xADB2B, 0xADB45, c_map_variant__remove_object_hook, _hook_execute_replaced_first, 0, true);

    // c_map_variant::unknown4 - called when objects spawn/respawn on sandtrap's elephants
    hook::insert(0xABA7E, 0xABA86, c_map_variant__unknown4_hook1, _hook_execute_replaced_last); // UNTESTED!!
    hook::insert(0xABAAA, 0xABAB1, c_map_variant__unknown4_hook2, _hook_execute_replaced_first); // UNTESTED!!

    // player_set_unit_index
    hook::call(0xB5F8E, player_set_unit_index_hook1); // hooks nearby unit_set_actively_controlled call
    hook::call(0xB60E4, player_set_unit_index_hook1); // hooks nearby unit_set_actively_controlled call
    hook::insert(0xB6289, 0xB628E, player_set_unit_index_hook2, _hook_execute_replaced_first);

    // player_increment_control_context - fixes player control after respawning
    hook::function(0xB9A90, 0x44, player_increment_control_context);

    // unit_died - update camo state on death for units & vehicles
    hook::insert(0x421469, 0x421471, unit_died_hook, _hook_execute_replaced_first);

    // sync grenade count after throw
    hook::insert(0x47D42F, 0x47D435, grenade_throw_move_to_hand_hook, _hook_execute_replaced_first);

    // sync grenade pickups
    hook::insert(0x4243D8, 0x4243DF, unit_add_grenade_to_inventory_hook, _hook_execute_replaced_first);

    // sync equipment pickup
    hook::insert(0x424586, 0x42458C, unit_add_equipment_to_inventory_hook, _hook_execute_replaced_first);

    // unit_update_control
    hook::insert(0x41854A, 0x418550, unit_update_control_hook, _hook_execute_replaced_first); // UNTESTED!!
    hook::insert(0x41868D, 0x418693, unit_update_control_hook, _hook_execute_replaced_first); // called for units with flag bit 2 set
    hook::insert(0x418A73, 0x418A79, unit_update_control_hook, _hook_execute_replaced_first); // sets aim & look vectors for controlled units - ie driving vehicles
    
    // unit_add_initial_loadout - sync spawn loadouts
    hook::add_variable_space_to_stack_frame(0xFB6E0, 0xFBAFD, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(0xFB6F1, 0xFB6F6, unit_add_initial_loadout_hook0, _hook_execute_replaced_last); // preserve player_object_index in a new variable
    hook::insert(0xFBA34, 0xFBA3A, unit_add_initial_loadout_hook1, _hook_execute_replaced_last); // syncs grenade counts
    hook::insert(0xFBAD9, 0xFBAE0, unit_add_initial_loadout_hook2, _hook_execute_replaced_last); // used to sync the revenge_shield_boost modifier shield bonus
    hook::insert(0xFBAE0, 0xFBAE6, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(0xFBAF2, 0xFBAFC, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // projectile_attach - prevents plasma nades from appearing like they can be picked up when stuck to a player
    hook::insert(0x467F11, 0x467F19, projectile_attach_hook, _hook_execute_replaced_first);

    // rewrite biped_update_melee_turning w/ updates
    hook::function(0x440E30, 0x8C, biped_update_melee_turning);

    // unit_control
    hook::call(0x2BB82, unit_control);
    hook::call(0xBD4E7, unit_control);
    hook::call(0xBD573, unit_control);
    hook::call(0x181410, unit_control);
    hook::call(0x69BB96, unit_control);
    hook::call(0x69DA76, unit_control);

    // unit_set_aiming_vectors
    hook::function(0x42A490, 0x49, unit_set_aiming_vectors); // UNTESTED!! called by c_game_engine::apply_player_update & player_teleport_on_bsp_switch
    patch::nop_region(0x1C9AF5, 3); // remove push 4 after original call to convert usercall to fastcall
    patch::bytes(0xB911B, { 0x20 }); // 0x24 to 0x20 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    patch::bytes(0xB911E, { 0x10 }); // 0x14 to 0x10 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    hook::insert(0x59EFF, 0x59F48, unit_set_aiming_vectors_hook1, _hook_replace); // UNTESTED!! c_simulation_unit_entity_definition::apply_object_update
    hook::insert(0x61093, 0x610CF, unit_set_aiming_vectors_hook2, _hook_replace); // handles recoil - c_simulation_weapon_fire_event_definition::apply_object_update
    hook::insert(0x4A0FCB, 0x4A1010, unit_set_aiming_vectors_hook3, _hook_replace); // auto turret aiming direction, but not facing? - c_vehicle_auto_turret::control
    hook::insert(0xF87D2, 0xF87F4, unit_set_aiming_vectors_hook4, _hook_replace); // UNTESTED!! attach_biped_to_player, sandbox only
    patch::nop_region(0xF87BF, 11); // cleanup leftover code

    // equipment_activate
    hook::insert(0x4514E2, 0x4514E8, equipment_activate_hook2, _hook_execute_replaced_last); // sync equipment creation time

    // sync unit energy levels
    hook::insert(0x41B600, 0x41B606, unit_update_energy_hook, _hook_execute_replaced_first);

    // No longer need these, unit_handle_equipment_energy_cost was rewritten
    //// sync energy costs / energy levels decreasing when throwing an equipment
    //hook::add_variable_space_to_stack_frame(0x42D290, 0x42D3F9, 4); // Add 4 bytes of variable space to the stack frame
    //hook::insert(0x42D2A4, 0x42D2A9, equipment_handle_energy_cost_hook0, _hook_execute_replaced_last); // preserve unit_index
    //hook::insert(0x42D392, 0x42D398, equipment_handle_energy_cost_hook1, _hook_execute_replaced_first); // unit energy
    //hook::insert(0x42D3F2, 0x42D3F8, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // sync hologram camo
    hook::insert(0x42C56E, 0x42C578, unit_set_hologram_hook, _hook_execute_replaced_first);

    // sync shield restoration with shield_recharge_on_melee_kill modifier
    hook::insert(0x412E41, 0x412E4F, object_apply_damage_aftermath_hook, _hook_execute_replaced_first, 0, true);

    // sync vehicle emp timer
    hook::insert(0x41AE09, 0x41AE10, unit_update_damage_hook, _hook_execute_replaced_first);

    // sync vehicle emp timer
    hook::insert(0x417B9D, 0x417BA4, unit_respond_to_emp_hook, _hook_execute_replaced_first);

    // sync equipment deletion
    hook::insert(0x417CE5, 0x417CEF, unit_delete_current_equipment_hook, _hook_execute_replaced_first);
    hook::function(0x424660, 0x75, unit_delete_equipment); // called by unit_drop_equipment & equipment_add
    hook::insert(0x4171D5, 0x41725C, unit_place_hook, _hook_replace); // replace inlined function
    hook::insert(0xFB7F4, 0xFB864, unit_add_initial_loadout_hook3, _hook_replace); // replace inlined function
    hook::insert(0xFB864, 0xFB869, unit_add_initial_loadout_hook4, _hook_execute_replaced_last); // add back overwritten variable
    hook::insert(0x5A23A, 0x5A2A4, c_simulation_unit_entity_definition__apply_object_update_hook, _hook_replace); // replace inlined function

    // camo decreasing on weapon fire/grenade throw/damage
    hook::function(0x42AAE0, 0x9C, unit_active_camouflage_ding_hook);
    hook::insert(0x47D18D, 0x47D20F, throw_release_hook2, _hook_replace); // replace inlined unit_active_camouflage_ding

    // camo disable
    hook::function(0x42AA80, 0x56, unit_active_camouflage_disable_hook);
    // INLINE IN c_simulation_unit_entity_definition::apply_object_update handled below
    // INLINE IN c_simulation_vehicle_entity_definition::apply_object_update handled below

    // camo set level
    hook::function(0x42AA20, 0x5E, unit_active_camouflage_set_level_hook);
    hook::insert(0x48582F, 0x485884, unit_scripting_set_active_camo_hook, _hook_replace); // UNTESTED!! // replace inline
    hook::insert(0xBFDC9, 0xBFE41, player_update_invisibility_hook, _hook_replace); // replace inlined unit_active_camouflage_set_level & unit_active_camouflage_set_maximum
    hook::insert(0x5A137, 0x5A1B2, c_simulation_unit_entity_definition__apply_object_update_hook2, _hook_replace); // UNTESTED!! // replace inlined unit_active_camouflage_set_level & unit_active_camouflage_disable
    hook::insert(0x7280D, 0x72888, c_simulation_vehicle_entity_definition__apply_object_update_hook, _hook_replace); // UNTESTED!! // replace inline
    hook::insert(0x69BD37, 0x69BD85, actor_set_active_camo_hook, _hook_replace); // UNTESTED!! // replace inline
    
    // camo set maximum
    hook::function(0x42A9D0, 0x4A, unit_active_camouflage_set_maximum_hook);
    // INLINE IN player_update_invisibility handled above
    hook::insert(0x43CEDD, 0x43CF07, biped_new_hook, _hook_replace); // replace inline
    hook::insert(0x4524B7, 0x4524E9, vehicle_new_hook, _hook_replace); // replace inline

    // unit camo update
    hook::add_variable_space_to_stack_frame(0x41AF50, 0x41B1D7, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(0x41AF6C, 0x41AF73, unit_update_active_camouflage_hook0, _hook_execute_replaced_first);
    hook::insert(0x41B1B9, 0x41B1D0, unit_update_active_camouflage_hook1, _hook_execute_replaced_first, 0, true);
    hook::insert(0x41B17F, 0x41B185, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(0x41B193, 0x41B199, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(0x41B1D0, 0x41B1D6, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // assassination finish
    hook::insert(0x44DF23, 0x44DF28, unit_action_assassinate_finished_hook, _hook_execute_replaced_first);

    // assassination submit
    hook::insert(0x44D7EF, 0x44D7F6, unit_action_assassinate_submit_hook1, _hook_execute_replaced_first);
    hook::insert(0x44D852, 0x44D859, unit_action_assassinate_submit_hook2, _hook_execute_replaced_first);

    // assassination interrupted
    hook::insert(0x44DFCD, 0x44DFD2, unit_action_assassinate_interrupted_hook, _hook_execute_replaced_first);

    // enter vehicle seats
    hook::insert(0x45666E, 0x456675, motor_task_enter_seat_internal_hook, _hook_execute_replaced_first);

    // exit vehicle seats
    hook::insert(0x457053, 0x457058, motor_animation_exit_seat_immediate_internal_hook, _hook_execute_replaced_last);

    // device group values
    hook::insert(0x45D9F9, 0x45DA2E, device_group_set_actual_value_hook1, _hook_replace); // UNTESTED!!
    hook::insert(0x45DA53, 0x45DA8C, device_group_set_actual_value_hook2, _hook_replace); // UNTESTED!!
    hook::insert(0x45D6E1, 0x45D6F4, device_group_set_desired_value_hook1, _hook_execute_replaced_last, 0, true); // UNTESTED!!
    hook::insert(0x45D745, 0x45D74B, device_group_set_desired_value_hook2, _hook_execute_replaced_last);

    // device power
    hook::insert(0x45D5BE, 0x45D5C3, device_set_power_hook, _hook_execute_replaced_first); // UNTESTED!!

    // device position
    hook::insert(0x48D398, 0x48D39F, machine_update_hook, _hook_execute_replaced_last);

    // auto turret tracking
    hook::add_variable_space_to_stack_frame(0x4A0BA0, 0x4A0EE4, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(0x4A0BB6, 0x4A0BBC, c_vehicle_auto_turret__track_auto_target_hook0, _hook_execute_replaced_last);
    hook::insert(0x4A0E3E, 0x4A0E44, c_vehicle_auto_turret__track_auto_target_hook1, _hook_execute_replaced_first);
    hook::insert(0x4A0EBF, 0x4A0EC6, c_vehicle_auto_turret__track_auto_target_hook2, _hook_execute_replaced_first);
    hook::insert(0x4A0ED4, 0x4A0EDB, c_vehicle_auto_turret__track_auto_target_hook2, _hook_execute_replaced_first);
    hook::insert(0x4A0EC6, 0x4A0ECC, (void*)4, _hook_stack_frame_cleanup);
    hook::insert(0x4A0EDB, 0x4A0EE1, (void*)4, _hook_stack_frame_cleanup);

    // character & biped physics position
    hook::insert(0x3FC276, 0x3FC27C, object_move_position_hook, _hook_execute_replaced_first); // object_move_position > object_set_position_internal inlined

    // force update player position
    hook::insert(0xC95EE, 0xC9611, game_engine_update_player_hook, _hook_replace);
}