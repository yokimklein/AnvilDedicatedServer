#include "hooks_object_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game.h>
#include <game\players.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <objects\objects.h>
#include <Patch.hpp>
#include <simulation\game_interface\simulation_game_generics.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_vehicles.h>
#include <simulation\game_interface\simulation_game_items.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <units\bipeds.h>
#include <units\units.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall object_update_hook()
{
    datum_index object_index;
    __asm mov object_index, edi;

    TLS_DATA_GET_VALUE_REFERENCE(object_headers);
    s_object_header* object_header = (s_object_header*)datum_get(*object_headers, object_index);
    
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

    __asm mov eax, [edi + 0x4]; // map_variant_placement->object_index
    __asm mov object_index, eax;

    if (game_is_authoritative())
    {
        simulation_action_object_update(object_index, _simulation_object_update_map_variant_index);
    }
}

__declspec(safebuffers) void __fastcall c_map_variant__unknown4_hook1()
{
    datum_index object_index;
    c_simulation_object_update_flags update_flags;

    __asm mov eax, [ebx + esi + 0x134]; // map_variant->objects[object_placement_index].object_index
    __asm mov object_index, eax;

    update_flags.m_flags.clear();
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
    __asm mov ecx, eax; // preserve eax value

    c_simulation_object_update_flags update_flags;
    s_unit_data* unit;
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x74, sizeof(update_flags) + sizeof(unit) + sizeof(unit_index) + 4); // where's this extra 4 bytes of space coming from?

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x70 - 0x60];
    __asm mov unit_index, eax;
    __asm mov unit, ecx;

    update_flags.m_flags.clear();
    if (unit->object_identifier.type == _object_type_vehicle)
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

    update_flags.m_flags.clear();
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
__declspec(naked) void equipment_handle_energy_cost_hook0()
{
    __asm mov [ebp + 4], ecx;
    __asm retn;
}

__declspec(safebuffers) void __fastcall equipment_handle_energy_cost_hook1()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C, sizeof(unit_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_consumable_energy);
}

__declspec(safebuffers) void __fastcall equipment_handle_energy_cost_hook2()
{
    s_unit_data* unit;
    __asm mov unit, ebx;

    simulation_action_game_engine_player_update(unit->player_index, _simulation_player_update_equipment_charges);
}

__declspec(safebuffers) void __fastcall unit_set_hologram_hook()
{
    c_simulation_object_update_flags update_flags;
    s_unit_data* unit;
    datum_index unit_index;

    __asm mov unit_index, ebx;
    __asm mov unit, esi;

    update_flags.m_flags.clear();
    if (unit->object_identifier.type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

__declspec(safebuffers) void __fastcall object_apply_damage_aftermath_hook()
{
    s_unit_data* unit;
    __asm mov unit, esi;
    TLS_DATA_GET_VALUE_REFERENCE(players);

    s_player_datum* player_data = &players[unit->player_index];
    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall unit_update_damage_hook()
{
    s_unit_data* unit;
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x70, sizeof(unit) + sizeof(unit_index));

    __asm mov unit, esi;
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x70 - 0x64];
    __asm mov unit_index, eax;

    if (unit->object_identifier.type.get() == _object_type_vehicle)
    {
        simulation_action_object_update(unit_index, _simulation_vehicle_update_seat_power);
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
    insert_hook(0x404907, 0x40490E, object_update_hook, _hook_execute_replaced_last);

    // add simulation_action_object_update back to player_set_facing
    Hook(0xB6300, player_set_facing).Apply();

    // c_map_variant::remove_object - should fix map variant object respawn times
    insert_hook(0xADB2B, 0xADB45, c_map_variant__remove_object_hook, _hook_execute_replaced_first, true);

    // c_map_variant::unknown4 - called when objects spawn/respawn on sandtrap's elephants
    insert_hook(0xABA7E, 0xABA86, c_map_variant__unknown4_hook1, _hook_execute_replaced_last); // UNTESTED!!
    insert_hook(0xABAAA, 0xABAB1, c_map_variant__unknown4_hook2, _hook_execute_replaced_first); // UNTESTED!!

    // player_set_unit_index
    Hook(0xB5F8E, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    Hook(0xB60E4, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    insert_hook(0xB6289, 0xB628E, player_set_unit_index_hook2, _hook_execute_replaced_first);

    // player_increment_control_context - fixes player control after respawning
    Hook(0xB9A90, player_increment_control_context).Apply();

    // unit_died - update camo state on death for units & vehicles
    insert_hook(0x421469, 0x421471, unit_died_hook, _hook_execute_replaced_first);

    // sync grenade count after throw
    insert_hook(0x47D42F, 0x47D435, grenade_throw_move_to_hand_hook, _hook_execute_replaced_first);

    // sync grenade pickups
    insert_hook(0x4243D8, 0x4243DF, unit_add_grenade_to_inventory_hook, _hook_execute_replaced_first);

    // sync equipment pickup
    insert_hook(0x424586, 0x42458C, unit_add_equipment_to_inventory_hook, _hook_execute_replaced_first);

    // unit_update_control
    insert_hook(0x41854A, 0x418550, unit_update_control_hook, _hook_execute_replaced_first); // UNTESTED!!
    insert_hook(0x41868D, 0x418693, unit_update_control_hook, _hook_execute_replaced_first); // called for units with flag bit 2 set
    insert_hook(0x418A73, 0x418A79, unit_update_control_hook, _hook_execute_replaced_first); // sets aim & look vectors for controlled units - ie driving vehicles
    
    // unit_add_initial_loadout - sync spawn loadouts
    add_variable_space_to_stack_frame(0xFB6E0, 0xFBAFD, 4); // Add 4 bytes of variable space to the stack frame
    insert_hook(0xFB6F1, 0xFB6F6, unit_add_initial_loadout_hook0, _hook_execute_replaced_last); // preserve player_object_index in a new variable
    insert_hook(0xFBA34, 0xFBA3A, unit_add_initial_loadout_hook1, _hook_execute_replaced_last); // syncs grenade counts
    insert_hook(0xFBAD9, 0xFBAE0, unit_add_initial_loadout_hook2, _hook_execute_replaced_last); // used to sync the revenge_shield_boost modifier shield bonus
    insert_hook(0xFBAE1, 0xFBAE6, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    insert_hook(0xFBAF2, 0xFBAFC, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // projectile_attach - prevents plasma nades from appearing like they can be picked up when stuck to a player
    insert_hook(0x467F11, 0x467F19, projectile_attach_hook, _hook_execute_replaced_first);

    // rewrite biped_update_melee_turning w/ updates
    Hook(0x440E30, biped_update_melee_turning).Apply();

    // unit_control
    Hook(0x2BB82, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD4E7, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD573, unit_control, HookFlags::IsCall).Apply();
    Hook(0x181410, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69BB96, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69DA76, unit_control, HookFlags::IsCall).Apply();

    // unit_set_aiming_vectors
    Hook(0x42A490, unit_set_aiming_vectors).Apply(); // UNTESTED!! called by c_game_engine::apply_player_update & player_teleport_on_bsp_switch
    Patch::NopFill(Pointer::Base(0x1C9AF5), 3);// remove push 4 after original call to convert usercall to fastcall
    Pointer::Base(0xB911B).Write<byte>(0x20); // 0x24 to 0x20 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    Pointer::Base(0xB911E).Write<byte>(0x10); // 0x14 to 0x10 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    insert_hook(0x59EFF, 0x59F48, unit_set_aiming_vectors_hook1, _hook_replace); // UNTESTED!! c_simulation_unit_entity_definition::apply_object_update
    insert_hook(0x61093, 0x610CF, unit_set_aiming_vectors_hook2, _hook_replace); // handles recoil - c_simulation_weapon_fire_event_definition::apply_object_update
    insert_hook(0x4A0FCB, 0x4A1010, unit_set_aiming_vectors_hook3, _hook_replace); // auto turret aiming direction, but not facing? - c_vehicle_auto_turret::control
    insert_hook(0xF87BF, 0xF87F4, unit_set_aiming_vectors_hook4, _hook_replace); // UNTESTED!! attach_biped_to_player, sandbox only

    // equipment_activate
    insert_hook(0x4514E2, 0x4514E8, equipment_activate_hook2, _hook_execute_replaced_last); // sync equipment creation time

    // sync unit energy levels
    insert_hook(0x41B600, 0x41B606, unit_update_energy_hook, _hook_execute_replaced_first);

    // sync energy costs / energy levels decreasing when throwing an equipment
    add_variable_space_to_stack_frame(0x42D290, 0x42D3F9, 4); // Add 4 bytes of variable space to the stack frame
    insert_hook(0x42D2A4, 0x42D2A9, equipment_handle_energy_cost_hook0, _hook_execute_replaced_last); // preserve unit_index
    insert_hook(0x42D392, 0x42D398, equipment_handle_energy_cost_hook1, _hook_execute_replaced_first); // unit energy
    insert_hook(0x42D3ED, 0x42D3F2, equipment_handle_energy_cost_hook2, _hook_execute_replaced_first); // TODO: move to player updates
    insert_hook(0x42D3F3, 0x42D3F8, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // sync hologram camo
    insert_hook(0x42C56E, 0x42C578, unit_set_hologram_hook, _hook_execute_replaced_first);

    // sync shield restoration with shield_recharge_on_melee_kill modifier
    insert_hook(0x412E41, 0x412E4F, object_apply_damage_aftermath_hook, _hook_execute_replaced_first, true);

    // sync vehicle emp timer - TODO: test this once vehicles sync
    insert_hook(0x41AE09, 0x41AE10, unit_update_damage_hook, _hook_execute_replaced_first);
}