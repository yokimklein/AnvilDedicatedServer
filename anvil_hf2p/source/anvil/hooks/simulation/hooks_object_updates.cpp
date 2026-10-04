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
#include <game\game_engine_teleporters.h>
#include <simulation\game_interface\simulation_game_weapons.h>

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
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index projectile_index = *(datum_index*)(registers.esp + 0x58 - 0x48);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index projectile_index = *(datum_index*)(registers.esp + 0x58 - 0x44);
#endif

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
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index unit_index = (datum_index)registers.ebx;
    long slot_index = (long)registers.ecx;

    unit_delete_equipment(unit_index, slot_index);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index unit_index = (datum_index)registers.edi;
    long slot_index = (long)registers.esi;

    // replicate the replaced instructions the code after this hook depends on
    *(long*)(registers.ebp - 0x08) = (long)registers.edx; // the slot's consumable byte, restored to edx after the loop body
    registers.ebx = *(size_t*)(registers.ebp - 0x18); // thread local storage game state pointer

    unit_delete_equipment(unit_index, slot_index);
#endif
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
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    s_simulation_unit_state_data* unit_state_data = (s_simulation_unit_state_data*)registers.esi;
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    s_simulation_unit_state_data* unit_state_data = (s_simulation_unit_state_data*)registers.edi;
#endif
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

void __cdecl c_game_engine__player_update_hook(s_hook_registers& registers)
{
    player_datum* player = (player_datum*)registers.esi;
    simulation_action_object_update(player->unit_index, _simulation_unit_update_grenade_counts);
}

void __cdecl game_engine_teleporters_update_hook(s_hook_registers& registers)
{
    c_area_set<c_teleporter_area, 32>* teleporters = (c_area_set<c_teleporter_area, 32>*)(registers.esi);
    teleporters->activate_all(); // use our reimplementation of activate_all w/ sim update
}

void __cdecl game_engine_initialize_for_new_map_hook(s_hook_registers& registers)
{
    c_teleporter_area* selected_area = (c_teleporter_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_ctf_engine__initialize_for_new_round_hook1(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_ctf_engine__initialize_for_new_round_hook2(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_3__initialize_hook(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_10__initialize_hook(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_10__select_area_hook1(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edi;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_10__select_area_hook2(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edx;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_12__initialize_hook(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_12__select_area_hook1(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edi;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_area_12__select_area_hook2(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edx;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_s_territory_data_8__initialize_hook(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_destination_zone_12__initialize_hook(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.eax;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_destination_zone_12__select_area_hook1(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edi;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl c_area_set_c_destination_zone_12__select_area_hook2(s_hook_registers& registers)
{
    c_area* selected_area = (c_area*)registers.edx;
    selected_area->set_selected(false); // use our reimplementation of set_selected w/ sim update
}

void __cdecl game_engine_multiplayer_weapon_register_hook(s_hook_registers& registers)
{
    datum_index weapon_index = registers.edi;
    simulation_action_object_update(weapon_index, _simulation_weapon_update_multiplayer_weapon_registration);
}

void __cdecl game_engine_multiplayer_weapon_deregister_hook(s_hook_registers& registers)
{
    datum_index weapon_index = registers.edi;
    simulation_action_object_update(weapon_index, _simulation_weapon_update_multiplayer_weapon_registration);
}

void __cdecl create_flag_hook(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index object_index = registers.edi;
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index object_index = registers.esi;
#endif
    simulation_action_object_update(object_index, _simulation_object_update_parent_state);
}

void __cdecl flag_reset_hook(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index object_index = registers.edi;
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index object_index = registers.esi;
#endif
    simulation_action_object_update(object_index, _simulation_object_update_parent_state);
}

void anvil_hooks_object_updates_apply()
{
    // add simulation_action_object_update back to object_update
    hook::insert(ADDRESS_OBJECT_UPDATE_HOOK, ADDRESS_OBJECT_UPDATE_HOOK_RETURN, object_update_hook, _hook_execute_replaced_last);

    // add simulation_action_object_update back to player_set_facing
    hook::function(ADDRESS_PLAYER_SET_FACING, 0xAE, player_set_facing);

    // c_map_variant::remove_object - should fix map variant object respawn times
    hook::insert(ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK, ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK_RETURN, c_map_variant__remove_object_hook, _hook_execute_replaced_first, 0, true);

    // c_map_variant::unknown4 - called when objects spawn/respawn on sandtrap's elephants
    hook::insert(ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1, ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1_RETURN, c_map_variant__unknown4_hook1, _hook_execute_replaced_last); // UNTESTED!!
    hook::insert(ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2, ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2_RETURN, c_map_variant__unknown4_hook2, _hook_execute_replaced_first); // UNTESTED!!

    // player_set_unit_index
    hook::call(ADDRESS_PLAYER_SET_UNIT_INDEX_CALL, player_set_unit_index_hook1); // hooks nearby unit_set_actively_controlled call
    hook::call(ADDRESS_PLAYER_SET_UNIT_INDEX_CALL_2, player_set_unit_index_hook1); // hooks nearby unit_set_actively_controlled call
    hook::insert(ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2, ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2_RETURN, player_set_unit_index_hook2, _hook_execute_replaced_first);

    // player_increment_control_context - fixes player control after respawning
    hook::function(ADDRESS_PLAYER_INCREMENT_CONTROL_CONTEXT, 0x44, player_increment_control_context);

    // unit_died - update camo state on death for units & vehicles
    hook::insert(ADDRESS_UNIT_DIED_HOOK, ADDRESS_UNIT_DIED_HOOK_RETURN, unit_died_hook, _hook_execute_replaced_first);

    // sync grenade count after throw
    hook::insert(ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK, ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK_RETURN, grenade_throw_move_to_hand_hook, _hook_execute_replaced_first);

    // sync grenade pickups
    hook::insert(ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK, ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK_RETURN, unit_add_grenade_to_inventory_hook, _hook_execute_replaced_first);

    // sync equipment pickup
    hook::insert(ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK, ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK_RETURN, unit_add_equipment_to_inventory_hook, _hook_execute_replaced_first);

    // unit_update_control
    hook::insert(ADDRESS_UNIT_UPDATE_CONTROL_HOOK, ADDRESS_UNIT_UPDATE_CONTROL_HOOK_RETURN, unit_update_control_hook, _hook_execute_replaced_first); // UNTESTED!!
    hook::insert(ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2, ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2_RETURN, unit_update_control_hook, _hook_execute_replaced_first); // called for units with flag bit 2 set
    hook::insert(ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3, ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3_RETURN, unit_update_control_hook, _hook_execute_replaced_first); // sets aim & look vectors for controlled units - ie driving vehicles
    
    // unit_add_initial_loadout - sync spawn loadouts
    hook::add_variable_space_to_stack_frame(ADDRESS_UNIT_ADD_INITIAL_LOADOUT, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_END, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0_RETURN, unit_add_initial_loadout_hook0, _hook_execute_replaced_last); // preserve player_object_index in a new variable
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1_RETURN, unit_add_initial_loadout_hook1, _hook_execute_replaced_last); // syncs grenade counts
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2_RETURN, unit_add_initial_loadout_hook2, _hook_execute_replaced_last); // used to sync the revenge_shield_boost modifier shield bonus
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_RETURN, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2_RETURN, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // projectile_attach - prevents plasma nades from appearing like they can be picked up when stuck to a player
    hook::insert(ADDRESS_PROJECTILE_ATTACH_HOOK, ADDRESS_PROJECTILE_ATTACH_HOOK_RETURN, projectile_attach_hook, _hook_execute_replaced_first);

    // rewrite biped_update_melee_turning w/ updates
    hook::function(ADDRESS_BIPED_UPDATE_MELEE_TURNING, 0x8C, biped_update_melee_turning);

    // unit_control
    hook::call(ADDRESS_UNIT_CONTROL_CALL, unit_control);
    hook::call(ADDRESS_UNIT_CONTROL_CALL_2, unit_control);
    hook::call(ADDRESS_UNIT_CONTROL_CALL_3, unit_control);
    hook::call(ADDRESS_UNIT_CONTROL_CALL_4, unit_control);
    hook::call(ADDRESS_UNIT_CONTROL_CALL_5, unit_control);
    hook::call(ADDRESS_UNIT_CONTROL_CALL_6, unit_control);

    // unit_set_aiming_vectors
    hook::function(ADDRESS_UNIT_SET_AIMING_VECTORS, 0x49, unit_set_aiming_vectors); // UNTESTED!! called by c_game_engine::apply_player_update & player_teleport_on_bsp_switch
    patch::nop_region(ADDRESS_C_GAME_ENGINE_APPLY_PLAYER_UPDATE_NOP, 3); // remove push 4 after original call to convert usercall to fastcall
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    patch::bytes(ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH, { 0x20 }); // 0x24 to 0x20 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    patch::bytes(ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH, { 0x24 }); // 0x28 to 0x24 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
#endif
    patch::bytes(ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH_2, { 0x10 }); // 0x14 to 0x10 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    hook::insert(ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1, ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1_RETURN, unit_set_aiming_vectors_hook1, _hook_replace); // UNTESTED!! c_simulation_unit_entity_definition::apply_object_update
    hook::insert(ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2, ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2_RETURN, unit_set_aiming_vectors_hook2, _hook_replace); // handles recoil - c_simulation_weapon_fire_event_definition::apply_object_update
    hook::insert(ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3, ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3_RETURN, unit_set_aiming_vectors_hook3, _hook_replace); // auto turret aiming direction, but not facing? - c_vehicle_auto_turret::control
    hook::insert(ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4, ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4_RETURN, unit_set_aiming_vectors_hook4, _hook_replace); // UNTESTED!! attach_biped_to_player, sandbox only
    patch::nop_region(ADDRESS_ATTACH_BIPED_TO_PLAYER_NOP, 11); // cleanup leftover code

    // equipment_activate
    hook::insert(ADDRESS_EQUIPMENT_ACTIVATE_HOOK2, ADDRESS_EQUIPMENT_ACTIVATE_HOOK2_RETURN, equipment_activate_hook2, _hook_execute_replaced_last); // sync equipment creation time

    // sync unit energy levels
    hook::insert(ADDRESS_UNIT_UPDATE_ENERGY_HOOK, ADDRESS_UNIT_UPDATE_ENERGY_HOOK_RETURN, unit_update_energy_hook, _hook_execute_replaced_first);

    // No longer need these, unit_handle_equipment_energy_cost was rewritten
    //// sync energy costs / energy levels decreasing when throwing an equipment
    //hook::add_variable_space_to_stack_frame(0x42D290, 0x42D3F9, 4); // Add 4 bytes of variable space to the stack frame
    //hook::insert(0x42D2A4, 0x42D2A9, equipment_handle_energy_cost_hook0, _hook_execute_replaced_last); // preserve unit_index
    //hook::insert(0x42D392, 0x42D398, equipment_handle_energy_cost_hook1, _hook_execute_replaced_first); // unit energy
    //hook::insert(0x42D3F2, 0x42D3F8, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // sync hologram camo
    hook::insert(ADDRESS_UNIT_SET_HOLOGRAM_HOOK, ADDRESS_UNIT_SET_HOLOGRAM_HOOK_RETURN, unit_set_hologram_hook, _hook_execute_replaced_first);

    // sync shield restoration with shield_recharge_on_melee_kill modifier
    hook::insert(ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK, ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK_RETURN, object_apply_damage_aftermath_hook, _hook_execute_replaced_first, 0, true);

    // sync vehicle emp timer
    hook::insert(ADDRESS_UNIT_UPDATE_DAMAGE_HOOK, ADDRESS_UNIT_UPDATE_DAMAGE_HOOK_RETURN, unit_update_damage_hook, _hook_execute_replaced_first);

    // sync vehicle emp timer
    hook::insert(ADDRESS_UNIT_RESPOND_TO_EMP_HOOK, ADDRESS_UNIT_RESPOND_TO_EMP_HOOK_RETURN, unit_respond_to_emp_hook, _hook_execute_replaced_first);

    // sync equipment deletion
    hook::insert(ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK, ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK_RETURN, unit_delete_current_equipment_hook, _hook_execute_replaced_first);
    hook::function(ADDRESS_UNIT_DELETE_EQUIPMENT, 0x75, unit_delete_equipment); // called by unit_drop_equipment & equipment_add
    hook::insert(ADDRESS_UNIT_PLACE_HOOK, ADDRESS_UNIT_PLACE_HOOK_RETURN, unit_place_hook, _hook_replace); // replace inlined function
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3_RETURN, unit_add_initial_loadout_hook3, _hook_replace); // replace inlined function
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    hook::insert(ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4, ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4_RETURN, unit_add_initial_loadout_hook4, _hook_execute_replaced_last); // add back overwritten variable
#endif // ms30's hook3 restores the overwritten variables itself, and every path to hook4's address goes through hook3
    hook::insert(ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK, ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN, c_simulation_unit_entity_definition__apply_object_update_hook, _hook_replace); // replace inlined function

    // camo decreasing on weapon fire/grenade throw/damage
    hook::function(ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DING, 0x9C, unit_active_camouflage_ding_hook);
    hook::insert(ADDRESS_THROW_RELEASE_HOOK2, ADDRESS_THROW_RELEASE_HOOK2_RETURN, throw_release_hook2, _hook_replace); // replace inlined unit_active_camouflage_ding

    // camo disable
    hook::function(ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DISABLE, 0x56, unit_active_camouflage_disable_hook);
    // INLINE IN c_simulation_unit_entity_definition::apply_object_update handled below
    // INLINE IN c_simulation_vehicle_entity_definition::apply_object_update handled below

    // camo set level
    hook::function(ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_LEVEL, 0x5E, unit_active_camouflage_set_level_hook);
    hook::insert(ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK, ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK_RETURN, unit_scripting_set_active_camo_hook, _hook_replace); // UNTESTED!! // replace inline
    hook::insert(ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK, ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK_RETURN, player_update_invisibility_hook, _hook_replace); // replace inlined unit_active_camouflage_set_level & unit_active_camouflage_set_maximum
    hook::insert(ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2, ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2_RETURN, c_simulation_unit_entity_definition__apply_object_update_hook2, _hook_replace); // UNTESTED!! // replace inlined unit_active_camouflage_set_level & unit_active_camouflage_disable
    hook::insert(ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK, ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN, c_simulation_vehicle_entity_definition__apply_object_update_hook, _hook_replace); // UNTESTED!! // replace inline
    hook::insert(ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK, ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK_RETURN, actor_set_active_camo_hook, _hook_replace); // UNTESTED!! // replace inline
    
    // camo set maximum
    hook::function(ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_MAXIMUM, 0x4A, unit_active_camouflage_set_maximum_hook);
    // INLINE IN player_update_invisibility handled above
    hook::insert(ADDRESS_BIPED_NEW_HOOK, ADDRESS_BIPED_NEW_HOOK_RETURN, biped_new_hook, _hook_replace); // replace inline
    hook::insert(ADDRESS_VEHICLE_NEW_HOOK, ADDRESS_VEHICLE_NEW_HOOK_RETURN, vehicle_new_hook, _hook_replace); // replace inline

    // unit camo update
    hook::add_variable_space_to_stack_frame(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_END, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0_RETURN, unit_update_active_camouflage_hook0, _hook_execute_replaced_first);
    hook::insert(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1_RETURN, unit_update_active_camouflage_hook1, _hook_execute_replaced_first, 0, true);
    hook::insert(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_RETURN, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2_RETURN, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    hook::insert(ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3, ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3_RETURN, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // assassination finish
    hook::insert(ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK, ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK_RETURN, unit_action_assassinate_finished_hook, _hook_execute_replaced_first);

    // assassination submit
    hook::insert(ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1, ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1_RETURN, unit_action_assassinate_submit_hook1, _hook_execute_replaced_first);
    hook::insert(ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2, ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2_RETURN, unit_action_assassinate_submit_hook2, _hook_execute_replaced_first);

    // assassination interrupted
    hook::insert(ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK, ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK_RETURN, unit_action_assassinate_interrupted_hook, _hook_execute_replaced_first);

    // enter vehicle seats
    hook::insert(ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK, ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK_RETURN, motor_task_enter_seat_internal_hook, _hook_execute_replaced_first);

    // exit vehicle seats
    hook::insert(ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK, ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK_RETURN, motor_animation_exit_seat_immediate_internal_hook, _hook_execute_replaced_last);

    // device group values
    hook::insert(ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1, ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1_RETURN, device_group_set_actual_value_hook1, _hook_replace); // UNTESTED!!
    hook::insert(ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2, ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2_RETURN, device_group_set_actual_value_hook2, _hook_replace); // UNTESTED!!
    hook::insert(ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1, ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1_RETURN, device_group_set_desired_value_hook1, _hook_execute_replaced_last, 0, true); // UNTESTED!!
    hook::insert(ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2, ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2_RETURN, device_group_set_desired_value_hook2, _hook_execute_replaced_last);

    // device power
    hook::insert(ADDRESS_DEVICE_SET_POWER_HOOK, ADDRESS_DEVICE_SET_POWER_HOOK_RETURN, device_set_power_hook, _hook_execute_replaced_first); // UNTESTED!!

    // device position
    hook::insert(ADDRESS_MACHINE_UPDATE_HOOK, ADDRESS_MACHINE_UPDATE_HOOK_RETURN, machine_update_hook, _hook_execute_replaced_last);

    // auto turret tracking
    hook::add_variable_space_to_stack_frame(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_END, 4); // Add 4 bytes of variable space to the stack frame
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0_RETURN, c_vehicle_auto_turret__track_auto_target_hook0, _hook_execute_replaced_last);
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1_RETURN, c_vehicle_auto_turret__track_auto_target_hook1, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_RETURN, c_vehicle_auto_turret__track_auto_target_hook2, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2_RETURN, c_vehicle_auto_turret__track_auto_target_hook2, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_RETURN, (void*)4, _hook_stack_frame_cleanup);
    hook::insert(ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2, ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2_RETURN, (void*)4, _hook_stack_frame_cleanup);

    // character & biped physics position
    hook::insert(ADDRESS_OBJECT_MOVE_POSITION_HOOK, ADDRESS_OBJECT_MOVE_POSITION_HOOK_RETURN, object_move_position_hook, _hook_execute_replaced_first); // object_move_position > object_set_position_internal inlined

    // force update player position
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK, ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK_RETURN, game_engine_update_player_hook, _hook_replace);
    
    // sync grenade regeneration player trait
    hook::insert(ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK, ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK_RETURN, c_game_engine__player_update_hook, _hook_execute_replaced_last);

    // sync teleporter multiplayer properties
    hook::insert(ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK, ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK_RETURN, game_engine_teleporters_update_hook, _hook_replace);
    hook::insert(ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK, ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK_RETURN, game_engine_initialize_for_new_map_hook, _hook_replace);

    // $TODO: test these, I'm not entirely sure when/where they are triggered
    // ctf area multiplayer properties
    hook::insert(ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1, ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1_RETURN, c_ctf_engine__initialize_for_new_round_hook1, _hook_replace);
    hook::insert(ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2, ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2_RETURN, c_ctf_engine__initialize_for_new_round_hook2, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK, ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK_RETURN, c_area_set_c_area_3__initialize_hook, _hook_replace);
    // king area multiplayer properties
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK, ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK_RETURN, c_area_set_c_area_10__initialize_hook, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1, ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1_RETURN, c_area_set_c_area_10__select_area_hook1, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2, ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2_RETURN, c_area_set_c_area_10__select_area_hook2, _hook_replace);
    // juggernaut & infection area multiplayer properties
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK, ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK_RETURN, c_area_set_c_area_12__initialize_hook, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1, ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1_RETURN, c_area_set_c_area_12__select_area_hook1, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2, ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2_RETURN, c_area_set_c_area_12__select_area_hook2, _hook_replace);
    // territories area multiplayer properties
    hook::insert(ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK, ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK_RETURN, c_area_set_s_territory_data_8__initialize_hook, _hook_replace);
    // vip area multiplayer properties
    hook::insert(ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK, ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK_RETURN, c_area_set_c_destination_zone_12__initialize_hook, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1, ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1_RETURN, c_area_set_c_destination_zone_12__select_area_hook1, _hook_replace);
    hook::insert(ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2, ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2_RETURN, c_area_set_c_destination_zone_12__select_area_hook2, _hook_replace);

    // multiplayer weapon (de)registration
    hook::insert(ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK, ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK_RETURN, game_engine_multiplayer_weapon_register_hook, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK, ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK_RETURN, game_engine_multiplayer_weapon_deregister_hook, _hook_execute_replaced_first);

    // ctf flag creation
    hook::insert(ADDRESS_CREATE_FLAG_HOOK, ADDRESS_CREATE_FLAG_HOOK_RETURN, create_flag_hook, _hook_execute_replaced_last, 0, true);

    // ctf flag reset
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    patch::bytes(ADDRESS_FLAG_RESET_PATCH, { 0x52, 0x02 }); // redirect jump to identical borrowed function epilogue from player_score so this path doesn't execute our hook
    patch::bytes(ADDRESS_FLAG_RESET_PATCH_2, { 0x1A, 0x02 }); // ditto
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    patch::bytes(ADDRESS_FLAG_RESET_PATCH, { 0xCE, 0xFD, 0xFF, 0xFF }); // redirect jump to identical borrowed function epilogue from create_flag so this path doesn't execute our hook
    patch::bytes(ADDRESS_FLAG_RESET_PATCH_2, { 0xC8 }); // short jump can't reach an epilogue, redirect it to the above jz instead (zf is still set so it's taken)
#endif
    hook::insert(ADDRESS_FLAG_RESET_HOOK, ADDRESS_FLAG_RESET_HOOK_RETURN, flag_reset_hook, _hook_execute_replaced_last);
}
