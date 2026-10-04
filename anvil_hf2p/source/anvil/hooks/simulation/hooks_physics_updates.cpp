#include "hooks_physics_updates.h"
#include <anvil\hooks\hooks.h>
#include <objects\objects.h>
#include <objects\scenery.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_items.h>
#include <simulation\game_interface\simulation_game_projectiles.h>

void __cdecl object_set_position_internal_hook1(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    simulation_action_object_update(object_index, _simulation_object_update_position);
}

void __cdecl object_set_position_internal_hook2(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    simulation_action_object_update(object_index, _simulation_object_update_forward_and_up);
}

void __cdecl object_move_respond_to_physics_hook(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.ebx;
    real_point3d* desired_position = (real_point3d*)(registers.esp + 0x70 - 0x0C);
    real_vector3d* desired_forward = (real_vector3d*)registers.edx;
    real_vector3d* desired_up = (real_vector3d*)registers.esi;

    object_set_position_internal(object_index, desired_position, desired_forward, desired_up, NULL, false, false, false, true);
}

// wrap usercall function to rewritten function
__declspec(naked) void object_set_velocities_internal_hook()
{
    __asm
    {
        // preserve registers
        push edi
        push esi
        push edx
        push ecx
        push ebx
        push eax

        push 0 // skip_update = false
        push [esp + 0x20] // angular_velocity (7x previous pushes, 1x previous call)
        //push edx // transitional_velocity
        //push ecx // object_index
        call object_set_velocities_internal

        // restore registers
        pop eax
        pop ebx
        pop ecx
        pop edx
        pop esi
        pop edi

        retn
    }
}

void __cdecl object_apply_acceleration_hook(s_hook_registers& registers)
{
    datum_index accelerated_object_index = *(datum_index*)(registers.esp + 0x30 - 0x1C);
    real_vector3d* translational_velocity = (real_vector3d*)(registers.esp + 0x30 - 0x0C);
    real_vector3d* angular_velocity = (real_vector3d*)(registers.esp + 0x30 - 0x18);

    object_set_velocities_internal(accelerated_object_index, translational_velocity, angular_velocity, false);
}

void __cdecl object_set_at_rest_hook2(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook3(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook4(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook5(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index object_index = *(datum_index*)(registers.esp + 0x58 - 0x48);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index object_index = *(datum_index*)(registers.esp + 0x58 - 0x44);
#endif

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook6(s_hook_registers& registers)
{
    datum_index object_index = *(datum_index*)(registers.esp + 0x258 - 0x210);

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook7(s_hook_registers& registers)
{
    datum_index object_index = *(datum_index*)(registers.esp + 0x298 - 0x28C);

    object_set_at_rest(object_index, false);
}

void __cdecl object_set_at_rest_hook8(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.esi;

    object_set_at_rest(object_index, false);
}

void __cdecl object_set_at_rest_hook9(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.esi;

    object_set_at_rest(object_index, false);
}

// TODO: I'm pretty sure swarms are only used for the flood, so it's probably fine to ignore this - may be used on cold storage?
void __cdecl object_set_at_rest_hook10(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.esi;

    object_set_at_rest(object_index, false);
}

void __cdecl object_set_at_rest_hook12(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.esi;

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_hook13(s_hook_registers& registers)
{
    datum_index object_index = (datum_index)registers.edi;

    object_set_at_rest(object_index, true);
}

void __cdecl object_set_at_rest_simulation_update(datum_index object_index)
{
    if (TEST_BIT(_object_mask_item, object_get_type(object_index)))
    {
        simulation_action_object_update(object_index, _simulation_item_update_set_at_rest);
    }
    else if (TEST_BIT(_object_mask_projectile, object_get_type(object_index)))
    {
        simulation_action_object_update(object_index, _simulation_projectile_update_set_at_rest);
    }
}

void __fastcall object_set_at_rest_hook(datum_index object_index)
{
    object_wake(object_index);
    object_set_at_rest_simulation_update(object_index);
}

// scenery_new
void __fastcall object_set_at_rest_hook11(datum_index object_index)
{
    object_set_at_rest_simulation_update(object_index);
    scenery_animation_idle(object_index);
}

// unit_custom_animation_play_animation_submit
void __fastcall object_set_at_rest_hook14(datum_index object_index)
{
    object_set_at_rest_simulation_update(object_index);
    object_compute_node_matrices(object_index);
}

void anvil_hooks_physics_updates_apply()
{
    // object_set_position_internal
    hook::insert(ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1, ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1_RETURN, object_set_position_internal_hook1, _hook_execute_replaced_first); // updates position
    hook::insert(ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2, ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2_RETURN, object_set_position_internal_hook2, _hook_execute_replaced_first); // updates forward & up
    hook::insert(ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK, ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK_RETURN, object_move_respond_to_physics_hook, _hook_replace); // replaced inlined code with call

    // object_set_velocities_internal - TODO: check if object_set_velocities was inlined anywhere
    hook::function(ADDRESS_OBJECT_SET_VELOCITIES_INTERNAL, 0x53, object_set_velocities_internal_hook);
    hook::insert(ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK, ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK_RETURN, object_apply_acceleration_hook, _hook_replace); // replaced inlined code with call
    
    // object_set_at_rest
    hook::function(ADDRESS_OBJECT_SET_AT_REST, 0x90, object_set_at_rest); // add updates back to original call
    // hook nearby object_wake calls in inlined object_set_at_rest instances to add back sim updates
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL, object_set_at_rest_hook); // c_simulation_object_entity_definition::object_apply_update
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_2, object_set_at_rest_hook); // c_simulation_generic_entity_definition::handle_delete_object
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_3, object_set_at_rest_hook); // garbage_collect_multiplayer
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_4, object_set_at_rest_hook); // c_candy_spawner::spawn_object
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_5, object_set_at_rest_hook); // c_havok_component::wake_all_bodies_in_phantoms
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_6, object_set_at_rest_hook); // object_reset
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_7, object_set_at_rest_hook); // unit_fix_position
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_8, object_set_at_rest_hook); // damage_response_fire
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_9, object_set_at_rest_hook); // object_damage_constraints
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_10, object_set_at_rest_hook); // biped_update_without_parent
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_11, object_set_at_rest_hook); // projectile_accelerate
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_12, object_set_at_rest_hook); // motor_animation_exit_seat_immediate_internal
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_13, object_set_at_rest_hook); // object_wake_physics - inlined into object_wake_physics_evaluate w/ object_wake call
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_14, object_set_at_rest_hook); // object_early_mover_delete
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_15, object_set_at_rest_hook); // item_multiplayer_at_rest_state_initialize 
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_16, object_set_at_rest_hook); // biped_stun_submit
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_17, object_set_at_rest_hook); // c_vehicle_type_mantis::update_physics
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_18, object_set_at_rest_hook); // biped_dead_force_airborne
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_19, object_set_at_rest_hook); // biped_exit_relaxation
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_20, object_set_at_rest_hook); // biped_start_relaxation
    // inlined object_set_at_rest instances
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK2, ADDRESS_OBJECT_SET_AT_REST_HOOK2_RETURN, object_set_at_rest_hook2, _hook_replace); // UNTESTED!! // c_simulation_generic_entity_definition::create_object
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK3, ADDRESS_OBJECT_SET_AT_REST_HOOK3_RETURN, object_set_at_rest_hook3, _hook_replace); // UNTESTED!! // c_simulation_vehicle_entity_definition::create_object
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK4, ADDRESS_OBJECT_SET_AT_REST_HOOK4_RETURN, object_set_at_rest_hook4, _hook_replace); // object_attach_to_node_immediate
    patch::nop_region(ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP, 4); // cleanup redundant instructions
    patch::nop_region(ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP_2, 3); // cleanup redundant instructions
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK5, ADDRESS_OBJECT_SET_AT_REST_HOOK5_RETURN, object_set_at_rest_hook5, _hook_replace); // projectile_attach
    patch::nop_region(ADDRESS_PROJECTILE_ATTACH_NOP, 14); // cleanup redundant instructions
    patch::nop_region(ADDRESS_PROJECTILE_ATTACH_NOP_2, 9); // cleanup redundant instructions
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK6, ADDRESS_OBJECT_SET_AT_REST_HOOK6_RETURN, object_set_at_rest_hook6, _hook_replace); // projectile_collision
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK7, ADDRESS_OBJECT_SET_AT_REST_HOOK7_RETURN, object_set_at_rest_hook7, _hook_replace); // projectile_initial_update (called for conically fired projectiles, ie shotguns)
    patch::nop_region(ADDRESS_PROJECTILE_INITIAL_UPDATE_NOP, 11); // cleanup redundant instructions
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK8, ADDRESS_OBJECT_SET_AT_REST_HOOK8_RETURN, object_set_at_rest_hook8, _hook_replace); // UNTESTED!! // object_early_mover_delete
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK9, ADDRESS_OBJECT_SET_AT_REST_HOOK9_RETURN, object_set_at_rest_hook9, _hook_replace); // UNTESTED!! // object_early_mover_delete
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK10, ADDRESS_OBJECT_SET_AT_REST_HOOK10_RETURN, object_set_at_rest_hook10, _hook_replace); // UNTESTED!! // swarm_accelerate > creature_accelerate inlined
    patch::nop_region(ADDRESS_SWARM_ACCELERATE_NOP, 0x5B); // nop leftover code
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_21, object_set_at_rest_hook11); // scenery_new, hooked scenery_animation_idle call
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK12, ADDRESS_OBJECT_SET_AT_REST_HOOK12_RETURN, object_set_at_rest_hook12, _hook_replace); // vehicle_program_activate // havok vehicle physics invalid flag, used only by troop hog back
    hook::insert(ADDRESS_OBJECT_SET_AT_REST_HOOK13, ADDRESS_OBJECT_SET_AT_REST_HOOK13_RETURN, object_set_at_rest_hook13, _hook_replace); // UNTESTED!! // vehicle_program_update
    hook::call(ADDRESS_OBJECT_SET_AT_REST_CALL_22, object_set_at_rest_hook14); // unit_custom_animation_play_animation_submit // plays on podium
}