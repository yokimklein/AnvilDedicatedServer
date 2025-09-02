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

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall object_update_hook()
{
    datum_index object_index;
    __asm mov object_index, edi;

    TLS_DATA_GET_VALUE_REFERENCE(object_headers);
    object_header_datum* object_header = (object_header_datum*)datum_get(*object_headers, object_index);
    
    // we've left the scope of this if check where we've hooked, so we need to check it again
    if (!object_header->flags.test(_object_header_do_not_update_bit))
    {
        if (object_needs_rigid_body_update(object_index))
        {
            simulation_action_object_update(object_index, _simulation_generic_update_rigid_body);
        }
    }
}

__declspec(safebuffers) void __fastcall c_map_variant__remove_object_hook()
{
    datum_index object_index;
    __asm
    {
        mov eax, [edi + 0x4]; // map_variant_placement->object_index
        mov object_index, eax;
    }
    if (game_is_authoritative())
    {
        simulation_action_object_update(object_index, _simulation_object_update_map_variant_index);
    }
}

__declspec(safebuffers) void __fastcall c_map_variant__unknown4_hook1()
{
    datum_index object_index;
    __asm mov eax, [ebx + esi + 0x134]; // map_variant->objects[object_placement_index].object_index
    __asm mov object_index, eax;

    c_simulation_object_update_flags update_flags;
    update_flags.set_flag(object_index, _simulation_object_update_position);
    update_flags.set_flag(object_index, _simulation_object_update_forward_and_up);
    simulation_action_object_update_internal(object_index, update_flags);
}

__declspec(safebuffers) void __fastcall c_map_variant__unknown4_hook2()
{
    datum_index object_index;

    __asm mov eax, [ebx + esi + 0x134]; // map_variant->objects[object_placement_index].object_index
    __asm mov object_index, eax;

    simulation_action_object_update(object_index, _simulation_object_update_parent_state);
}

__declspec(safebuffers) void __fastcall player_set_unit_index_hook2()
{
    datum_index unit_index;
    __asm mov eax, [edi + 0x30];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall unit_died_hook()
{
    unit_datum* unit;
    datum_index unit_index;
    __asm mov unit_index, esi;
    __asm mov unit, eax;

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

__declspec(safebuffers) void __fastcall grenade_throw_move_to_hand_hook()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x20, sizeof(unit_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x08];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_grenade_counts);
}

__declspec(safebuffers) void __fastcall unit_add_grenade_to_inventory_hook()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x20, sizeof(unit_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x08];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_grenade_counts);
}

__declspec(safebuffers) void __fastcall unit_add_equipment_to_inventory_hook()
{
    datum_index unit_index;
    c_simulation_object_update_flags update_flags;
    DEFINE_ORIGINAL_EBP_ESP(0x24, sizeof(unit_index) + sizeof(update_flags));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x20 - 0x0C];
    __asm mov unit_index, eax;

    update_flags.set_flag(unit_index, _simulation_unit_update_equipment);
    update_flags.set_flag(unit_index, _simulation_unit_update_equipment_charges);
    simulation_action_object_update_internal(unit_index, update_flags);
}

__declspec(safebuffers) void __fastcall unit_update_control_hook()
{
    datum_index unit_index;
    __asm mov unit_index, ebx;

    simulation_action_object_update(unit_index, _simulation_unit_update_desired_aiming_vector);
}

// preserve player_object_index variable
__declspec(naked) void unit_add_initial_loadout_hook0()
{
    __asm mov[ebp + 4], ecx;
    __asm retn;
}

// syncs grenade counts on spawn
__declspec(safebuffers) void __fastcall unit_add_initial_loadout_hook1()
{
    datum_index player_object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C8, sizeof(player_object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov player_object_index, eax;

    simulation_action_object_update(player_object_index, _simulation_unit_update_grenade_counts);
}

// syncs revenge shield bonus on spawn
__declspec(safebuffers) void __fastcall unit_add_initial_loadout_hook2()
{
    datum_index player_object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C8, sizeof(player_object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov player_object_index, eax;

    simulation_action_object_update(player_object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall projectile_attach_hook()
{
    datum_index projectile_index;
    DEFINE_ORIGINAL_EBP_ESP(0x58, sizeof(projectile_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x58 - 0x48];
    __asm mov projectile_index, eax;

    simulation_action_object_update(projectile_index, _simulation_object_update_parent_state);
}

__declspec(safebuffers) void __fastcall unit_set_aiming_vectors_hook1()
{
    datum_index unit_index;
    s_simulation_unit_state_data* unit_state_data;

    __asm mov unit_index, ebx;
    __asm mov unit_state_data, edi;

    unit_set_aiming_vectors(unit_index, &unit_state_data->desired_aiming_vector, &unit_state_data->desired_aiming_vector);
}

__declspec(safebuffers) void __fastcall unit_set_aiming_vectors_hook2()
{
    datum_index unit_index;
    real_vector3d* forward;
    DEFINE_ORIGINAL_EBP_ESP(0xE0, sizeof(unit_index) + sizeof(forward));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0xE0 - 0xB4];
    __asm mov unit_index, eax;
    __asm mov forward, edx;

    unit_set_aiming_vectors(unit_index, forward, forward);
}

__declspec(safebuffers) void __fastcall unit_set_aiming_vectors_hook3()
{
    datum_index unit_index;
    real_vector3d* forward;

    __asm mov unit_index, ebx;
    __asm lea eax, [esi + 0x8BC];
    __asm mov forward, eax;

    unit_set_aiming_vectors(unit_index, forward, forward);
}

__declspec(safebuffers) void __fastcall unit_set_aiming_vectors_hook4()
{
    datum_index unit_index;
    real_vector3d* forward;
    DEFINE_ORIGINAL_EBP_ESP(0x8C, sizeof(unit_index) + sizeof(forward));

    __asm mov unit_index, edi;
    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x2C];
    __asm mov forward, eax;

    unit_set_aiming_vectors(unit_index, forward, forward);

    // preserve overwritten assembly instructions re-used later on
    __asm movzx edi, di;
    __asm add edi, edi;
    __asm add esp, 0x28;
}

__declspec(safebuffers) void __fastcall equipment_activate_hook2()
{
    datum_index equipment_index;
    DEFINE_ORIGINAL_EBP_ESP(0x35C, sizeof(equipment_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x358 - 0x328];
    __asm mov equipment_index, eax;

    simulation_action_object_update(equipment_index, _simulation_item_update_equipment_creation_time);
}

__declspec(safebuffers) void __fastcall unit_update_energy_hook()
{
    datum_index unit_index;
    __asm mov unit_index, ebx;

    simulation_action_object_update(unit_index, _simulation_unit_update_consumable_energy);
}

// preserve unit_index variable
__declspec(naked) void unit_handle_equipment_energy_cost_hook0()
{
    __asm mov [ebp + 4], ecx;
    __asm retn;
}

__declspec(safebuffers) void __fastcall unit_handle_equipment_energy_cost_hook1()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C, sizeof(unit_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_consumable_energy);
}

__declspec(safebuffers) void __fastcall unit_set_hologram_hook()
{
    unit_datum* unit;
    datum_index unit_index;
    __asm mov unit_index, ebx;
    __asm mov unit, esi;

    c_simulation_object_update_flags update_flags;
    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

__declspec(safebuffers) void __fastcall object_apply_damage_aftermath_hook()
{
    unit_datum* unit;
    __asm mov unit, esi;
    TLS_DATA_GET_VALUE_REFERENCE(players);

    player_datum* player_data = &players[unit->unit.player_index];
    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall unit_update_damage_hook()
{
    unit_datum* unit;
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x70, sizeof(unit) + sizeof(unit_index));

    __asm mov unit, esi;
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x70 - 0x64];
    __asm mov unit_index, eax;

    if (unit->object.object_identifier.m_type.get() == _object_type_vehicle)
    {
        simulation_action_object_update(unit_index, _simulation_vehicle_update_seat_power);
    }
}

__declspec(safebuffers) void __fastcall unit_respond_to_emp_hook()
{
    unit_datum* unit;
    datum_index unit_index;

    __asm mov unit, edi;
    __asm mov unit_index, esi;

    if (unit->object.object_identifier.m_type.get() == _object_type_vehicle)
    {
        simulation_action_object_update(unit_index, _simulation_vehicle_update_seat_power);
    }
}

__declspec(safebuffers) void __fastcall unit_delete_current_equipment_hook()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C, sizeof(unit_index));

    _asm mov eax, original_ebp;
    _asm mov eax, [eax + 0x08];
    _asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_equipment);
}

__declspec(safebuffers) void __fastcall unit_place_hook()
{
    datum_index unit_index;
    __asm mov unit_index, edi;

    for (long i = 0; i < 4; i++)
    {
        unit_delete_equipment(unit_index, i);
    }
}

__declspec(safebuffers) void __fastcall unit_add_initial_loadout_hook3()
{
    datum_index unit_index;
    long slot_index;
    __asm mov unit_index, ebx;
    __asm mov slot_index, ecx;

    unit_delete_equipment(unit_index, slot_index);
}

__declspec(naked) void unit_add_initial_loadout_hook4()
{
    __asm mov esi, [ebp - 0x08];
    __asm retn;
}

// TODO: figure out how to trigger this to test
__declspec(safebuffers) void __fastcall c_simulation_unit_entity_definition__apply_object_update_hook()
{
    datum_index unit_index;
    long slot_index;
    __asm mov unit_index, ebx;
    __asm mov slot_index, ecx;

    unit_delete_equipment(unit_index, slot_index);
}

__declspec(safebuffers) void __fastcall unit_active_camouflage_ding_hook()
{
    // wrapper for usercall
    datum_index unit_index;
    real camo_decay;
    real regrowth_seconds;
    __asm mov unit_index, ecx;
    __asm movss camo_decay, xmm1;
    __asm movss regrowth_seconds, xmm2;

    unit_active_camouflage_ding(unit_index, camo_decay, regrowth_seconds);
}

__declspec(safebuffers) void __fastcall throw_release_hook2()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x58, sizeof(unit_index));
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x58 - 0x44];
    __asm mov unit_index, eax;

    unit_active_camouflage_ding(unit_index, 0.6f, 0.0f);
}

__declspec(safebuffers) void __fastcall unit_active_camouflage_disable_hook()
{
    // usercall wrapper
    datum_index unit_index;
    real regrowth_seconds;
    __asm mov unit_index, ecx;
    __asm movss regrowth_seconds, xmm1;

    unit_active_camouflage_disable(unit_index, regrowth_seconds);
}

__declspec(safebuffers) void __fastcall unit_active_camouflage_set_level_hook()
{
    // usercall wrapper
    datum_index unit_index;
    real regrowth_seconds;
    long camouflage_end_time;
    __asm mov unit_index, ecx;
    __asm movss regrowth_seconds, xmm1;
    __asm mov camouflage_end_time, edx;

    unit_active_camouflage_set_level(unit_index, regrowth_seconds, camouflage_end_time);
}

__declspec(safebuffers) void __fastcall unit_scripting_set_active_camo_hook()
{
    datum_index unit_index;
    real regrowth_seconds;
    __asm mov unit_index, ecx;
    __asm movss regrowth_seconds, xmm2;

    unit_active_camouflage_set_level(unit_index, regrowth_seconds, NONE);
}

__declspec(safebuffers) void __fastcall player_update_invisibility_hook()
{
    player_datum* player;
    real camouflage_maximum;
    __asm mov player, esi;
    __asm movss camouflage_maximum, xmm1;

    unit_active_camouflage_set_level(player->unit_index, 4.0f, NONE);
    unit_active_camouflage_set_maximum(player->unit_index, camouflage_maximum);
}

__declspec(safebuffers) void __fastcall c_simulation_unit_entity_definition__apply_object_update_hook2()
{
    s_simulation_unit_state_data* unit_state_data;
    datum_index unit_index;
    __asm mov unit_state_data, esi;
    __asm mov unit_index, ebx;

    if (unit_state_data->active_camo_active)
    {
        unit_active_camouflage_set_level(unit_index, 4.0f, NONE);
    }
    else
    {
        unit_active_camouflage_disable(unit_index, 4.0f);
    }
}

__declspec(safebuffers) void __fastcall c_simulation_vehicle_entity_definition__apply_object_update_hook()
{
    s_simulation_vehicle_state_data* vehicle_state_data;
    datum_index vehicle_index;
    DEFINE_ORIGINAL_EBP_ESP(0x24, sizeof(vehicle_state_data) + sizeof(vehicle_index));
    __asm mov vehicle_state_data, edi;
    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 0x08];
    __asm mov vehicle_index, eax;

    if (vehicle_state_data->active_camo_active)
    {
        unit_active_camouflage_set_level(vehicle_index, 4.0f, NONE);
    }
    else
    {
        unit_active_camouflage_disable(vehicle_index, 4.0f);
    }
}

__declspec(safebuffers) void __fastcall actor_set_active_camo_hook()
{
    actor_datum* actor;
    real regrowth_seconds;
    __asm mov actor, edi;
    __asm movss regrowth_seconds, xmm2;

    unit_active_camouflage_set_level(actor->meta.unit_index, regrowth_seconds, NONE);
}

__declspec(safebuffers) void __fastcall unit_active_camouflage_set_maximum_hook()
{
    datum_index unit_index;
    real camouflage_maximum;
    __asm mov unit_index, ecx;
    __asm movss camouflage_maximum, xmm1;

    unit_active_camouflage_set_maximum(unit_index, camouflage_maximum);
}

__declspec(safebuffers) void __fastcall biped_new_hook()
{
    datum_index unit_index;
    real camouflage_maximum;
    __asm mov unit_index, edi;
    __asm movss camouflage_maximum, xmm0;

    unit_active_camouflage_set_maximum(unit_index, camouflage_maximum);
}

__declspec(safebuffers) void __fastcall vehicle_new_hook()
{
    datum_index vehicle_index;
    real camouflage_maximum;
    __asm mov vehicle_index, ebx;
    __asm movss camouflage_maximum, xmm0;

    unit_active_camouflage_set_maximum(vehicle_index, camouflage_maximum);
}

// preserve unit_index variable
__declspec(naked) void unit_update_active_camouflage_hook0()
{
    __asm mov [ebp + 4], ecx;
    __asm retn;
}

__declspec(safebuffers) void __fastcall unit_update_active_camouflage_hook1()
{
    unit_datum* unit;
    datum_index unit_index;
    c_simulation_object_update_flags update_flags;
    DEFINE_ORIGINAL_EBP_ESP(0x48, sizeof(unit_index) + sizeof(unit) + sizeof(update_flags) + 4); // where's this extra 4 bytes coming from? alignment?

    __asm mov unit, esi;
    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov unit_index, eax;

    if (unit->object.object_identifier.m_type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

__declspec(safebuffers) void __fastcall unit_action_assassinate_finished_hook()
{
    datum_index mover_index;
    __asm mov mover_index, edx;
    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall unit_action_assassinate_submit_hook1()
{
    datum_index mover_index;
    __asm mov mover_index, esi;
    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall unit_action_assassinate_submit_hook2()
{
    s_action_request* request;
    DEFINE_ORIGINAL_EBP_ESP(0x3C, sizeof(request));
    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 0x0C];
    __asm mov request, eax;
    simulation_action_object_update(request->assassination.victim_unit_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall unit_action_assassinate_interrupted_hook()
{
    datum_index mover_index;
    __asm mov mover_index, edi;
    simulation_action_object_update(mover_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall motor_task_enter_seat_internal_hook()
{
    datum_index unit_index;
    __asm mov unit_index, edi;
    simulation_action_object_update(unit_index, _simulation_unit_update_parent_vehicle);
}

__declspec(safebuffers) void __fastcall motor_animation_exit_seat_immediate_internal_hook()
{
    datum_index unit_index;
    __asm mov unit_index, esi;
    simulation_action_object_update(unit_index, _simulation_unit_update_parent_vehicle);
}

__declspec(safebuffers) void __fastcall device_group_set_actual_value_hook1()
{
    datum_index device_index;
    __asm mov device_index, esi;

    object_wake(device_index);
    simulation_action_object_update(device_index, _simulation_device_update_power);
}

__declspec(safebuffers) void __fastcall device_group_set_actual_value_hook2()
{
    datum_index device_index;
    __asm mov device_index, esi;

    object_wake(device_index);
    simulation_action_object_update(device_index, _simulation_device_update_position);
}

__declspec(safebuffers) void __fastcall device_group_set_desired_value_hook1()
{
    datum_index device_index;
    __asm mov device_index, esi;
    simulation_action_object_update(device_index, _simulation_device_update_power_group);
}

__declspec(safebuffers) void __fastcall device_group_set_desired_value_hook2()
{
    datum_index device_index;
    __asm mov device_index, esi;
    simulation_action_object_update(device_index, _simulation_device_update_position_group);
}

__declspec(safebuffers) void __fastcall device_set_power_hook()
{
    datum_index device_index;
    __asm mov device_index, edi;
    simulation_action_object_update(device_index, _simulation_device_update_power);
}

__declspec(safebuffers) void __fastcall machine_update_hook()
{
    datum_index device_index;
    __asm mov device_index, esi;
    simulation_action_object_update(device_index, _simulation_device_update_position);
}

// preserve vehicle_index variable
__declspec(naked) void c_vehicle_auto_turret__track_auto_target_hook0()
{
    __asm mov [ebp + 4], ecx;
    __asm retn;
}

__declspec(safebuffers) void __fastcall c_vehicle_auto_turret__track_auto_target_hook1()
{
    datum_index vehicle_index;
    DEFINE_ORIGINAL_EBP_ESP(0x5C, sizeof(vehicle_index));
    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov vehicle_index, eax;
    simulation_action_object_update(vehicle_index, _simulation_vehicle_update_auto_turret_tracking);
}

__declspec(safebuffers) void __fastcall c_vehicle_auto_turret__track_auto_target_hook2()
{
    datum_index vehicle_index;
    DEFINE_ORIGINAL_EBP_ESP(0x5C, sizeof(vehicle_index));
    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov vehicle_index, eax;
    simulation_action_object_update(vehicle_index, _simulation_vehicle_update_auto_turret);
}

__declspec(safebuffers) void __fastcall object_move_position_hook()
{
    datum_index object_index;
    __asm mov object_index, edi;

    simulation_action_object_update(object_index, _simulation_object_update_position);
}

__declspec(safebuffers) void __fastcall game_engine_update_player_hook()
{
    long absolute_player_index;
    player_datum* player;
    c_simulation_object_update_flags flags;
    DEFINE_ORIGINAL_EBP_ESP(0x18, sizeof(absolute_player_index) + sizeof(player) + sizeof(flags));

    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx - 0x0C];
        mov absolute_player_index, eax;
        mov player, edi;
    }

    if (!(game_time_get() % game_tick_rate()))
    {
        game_results_statistic_increment(absolute_player_index, _game_team_none, _game_results_statistic_seconds_alive, 1);
    }
    if (!(game_time_get() % game_seconds_integer_to_ticks(5)))
    {
        flags.set_flag(player->unit_index, _simulation_object_update_position);
        simulation_action_object_force_update(player->unit_index, flags);
    }
}
#pragma runtime_checks("", restore)

void __fastcall player_set_unit_index_hook1(datum_index unit_index, bool unknown)
{
    simulation_action_object_update(unit_index, _simulation_unit_update_control);
    unit_set_actively_controlled(unit_index, unknown);
}

void anvil_hooks_object_updates_apply()
{
    // add simulation_action_object_update back to object_update
    hook::insert(0x404907, 0x40490E, object_update_hook, _hook_execute_replaced_last);

    // add simulation_action_object_update back to player_set_facing
    hook::function(0xB6300, 0xAE, player_set_facing);

    // c_map_variant::remove_object - should fix map variant object respawn times
    hook::insert(0xADB2B, 0xADB45, c_map_variant__remove_object_hook, _hook_execute_replaced_first, true);

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
    hook::insert(0xF87BF, 0xF87F4, unit_set_aiming_vectors_hook4, _hook_replace); // UNTESTED!! attach_biped_to_player, sandbox only

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
    hook::insert(0x412E41, 0x412E4F, object_apply_damage_aftermath_hook, _hook_execute_replaced_first, true);

    // sync vehicle emp timer - TODO: test this once vehicles sync
    hook::insert(0x41AE09, 0x41AE10, unit_update_damage_hook, _hook_execute_replaced_first);

    // sync vehicle emp timer - TODO: ditto above
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
    hook::insert(0x41B1B9, 0x41B1D0, unit_update_active_camouflage_hook1, _hook_execute_replaced_first, true);
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
    hook::insert(0x45D6E1, 0x45D6F4, device_group_set_desired_value_hook1, _hook_execute_replaced_last, true); // UNTESTED!!
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