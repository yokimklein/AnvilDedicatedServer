#include "hooks_physics_updates.h"
#include <anvil\hooks\hooks.h>
#include <objects\objects.h>
#include <objects\scenery.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_items.h>
#include <simulation\game_interface\simulation_game_projectiles.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall object_set_position_internal_hook1()
{
    datum_index object_index;
    __asm mov object_index, edi;
    simulation_action_object_update(object_index, _simulation_object_update_position);
}

__declspec(safebuffers) void __fastcall object_set_position_internal_hook2()
{
    datum_index object_index;
    __asm mov object_index, edi;
    simulation_action_object_update(object_index, _simulation_object_update_forward_and_up);
}

__declspec(safebuffers) void __fastcall object_move_respond_to_physics_hook()
{
    datum_index object_index;
    real_point3d* desired_position;
    real_vector3d* desired_forward;
    real_vector3d* desired_up;
    DEFINE_ORIGINAL_EBP_ESP(0x70, sizeof(object_index) + sizeof(desired_position) + sizeof(desired_forward) + sizeof(desired_up));

    __asm mov object_index, ebx;
    __asm mov eax, original_esp;
    __asm lea eax, [eax + 0x70 - 0x0C];
    __asm mov desired_position, eax;
    __asm mov desired_forward, edx;
    __asm mov desired_up, esi;

    object_set_position_internal(object_index, desired_position, desired_forward, desired_up, nullptr, false, false, false, true);
}

// wrap usercall function to rewritten function
__declspec(naked) void object_set_velocities_internal_hook()
{
    __asm
    {
        push 0 // skip_update = false
        push[esp + 0x8] // angular_velocity
        push edx // transitional_velocity
        push ecx // object_index
        call object_set_velocities_internal
        // restore registers
        pop ecx
        pop edx
        // cleanup stack & return
        add esp, 8
        retn
    }
}

__declspec(safebuffers) void __fastcall object_apply_acceleration_hook()
{
    datum_index accelerated_object_index;
    real_vector3d* translational_velocity;
    real_vector3d* angular_velocity;
    DEFINE_ORIGINAL_EBP_ESP(0x34, sizeof(accelerated_object_index) + sizeof(translational_velocity) + sizeof(angular_velocity));

    __asm mov ecx, original_esp;
    __asm mov eax, [ecx + 0x30 - 0x1C];
    __asm mov accelerated_object_index, eax;
    __asm lea eax, [ecx + 0x30 - 0x0C];
    __asm mov translational_velocity, eax;
    __asm lea eax, [ecx + 0x30 - 0x18];
    __asm mov angular_velocity, eax;

    object_set_velocities_internal(accelerated_object_index, translational_velocity, angular_velocity, false);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook2()
{
    datum_index object_index;
    __asm mov object_index, edi;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook3()
{
    datum_index object_index;
    __asm mov object_index, edi;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook4()
{
    datum_index object_index;
    __asm mov object_index, edi;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook5()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x58, sizeof(object_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x58 - 0x48];
    __asm mov object_index, eax;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook6()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x25C, sizeof(object_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x258 - 0x210];
    __asm mov object_index, eax;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook7()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x298, sizeof(object_index));

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x298 - 0x28C];
    __asm mov object_index, eax;

    object_set_at_rest(object_index, false);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook8()
{
    datum_index object_index;
    __asm mov object_index, esi;

    object_set_at_rest(object_index, false);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook9()
{
    datum_index object_index;
    __asm mov object_index, esi;

    object_set_at_rest(object_index, false);
}

// TODO: I'm pretty sure swarms are only used for the flood, so it's probably fine to ignore this - may be used on cold storage?
__declspec(safebuffers) void __fastcall object_set_at_rest_hook10()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x3C, sizeof(object_index));

    __asm mov object_index, esi;
    // preserve overwritten variable
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x38 - 0x28];
    __asm mov edi, eax;

    object_set_at_rest(object_index, false);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook12()
{
    datum_index object_index;
    __asm mov object_index, esi;

    object_set_at_rest(object_index, true);
}

__declspec(safebuffers) void __fastcall object_set_at_rest_hook13()
{
    datum_index object_index;
    __asm mov object_index, edi;

    object_set_at_rest(object_index, true);
}
#pragma runtime_checks("", restore)

void __cdecl object_set_at_rest_simulation_update(datum_index object_index)
{
    if (TEST_BIT(_object_mask_item, object_get_type(object_index)))
        simulation_action_object_update(object_index, _simulation_item_update_set_at_rest);
    else if (TEST_BIT(_object_mask_projectile, object_get_type(object_index)))
        simulation_action_object_update(object_index, _simulation_projectile_update_set_at_rest);
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
    hook::insert(0x3FC038, 0x3FC03E, object_set_position_internal_hook1, _hook_execute_replaced_first); // updates position
    hook::insert(0x3FC060, 0x3FC066, object_set_position_internal_hook2, _hook_execute_replaced_first); // updates forward & up
    hook::insert(0x404A51, 0x404ADD, object_move_respond_to_physics_hook, _hook_replace); // replaced inlined code with call

    // object_set_velocities_internal - TODO: check if object_set_velocities was inlined anywhere
    hook::function(0x3FC500, 0x53, object_set_velocities_internal_hook);
    hook::insert(0x3FC7F8, 0x3FC833, object_apply_acceleration_hook, _hook_replace); // replaced inlined code with call
    
    // object_set_at_rest
    hook::function(0x4011F0, 0x90, object_set_at_rest); // add updates back to original call
    // hook nearby object_wake calls in inlined object_set_at_rest instances to add back sim updates
    hook::call(0x6BBB8, object_set_at_rest_hook); // c_simulation_object_entity_definition::object_apply_update
    hook::call(0x773D6, object_set_at_rest_hook); // c_simulation_generic_entity_definition::handle_delete_object
    hook::call(0xC82A1, object_set_at_rest_hook); // garbage_collect_multiplayer
    hook::call(0x172CB3, object_set_at_rest_hook); // c_candy_spawner::spawn_object
    hook::call(0x1282B8, object_set_at_rest_hook); // c_havok_component::wake_all_bodies_in_phantoms
    hook::call(0x3FBF35, object_set_at_rest_hook); // object_reset
    hook::call(0x419397, object_set_at_rest_hook); // unit_fix_position
    hook::call(0x413DD3, object_set_at_rest_hook); // damage_response_fire
    hook::call(0x415DC3, object_set_at_rest_hook); // object_damage_constraints
    hook::call(0x43DAF6, object_set_at_rest_hook); // biped_update_without_parent
    hook::call(0x4632B7, object_set_at_rest_hook); // projectile_accelerate
    hook::call(0x456EB5, object_set_at_rest_hook); // motor_animation_exit_seat_immediate_internal
    hook::call(0x1EF12D, object_set_at_rest_hook); // object_wake_physics - inlined into object_wake_physics_evaluate w/ object_wake call
    hook::call(0x46D33E, object_set_at_rest_hook); // object_early_mover_delete
    hook::call(0x4848B8, object_set_at_rest_hook); // item_multiplayer_at_rest_state_initialize 
    hook::call(0x4980E3, object_set_at_rest_hook); // biped_stun_submit
    hook::call(0x4A9126, object_set_at_rest_hook); // c_vehicle_type_mantis::update_physics
    hook::call(0x4A4300, object_set_at_rest_hook); // biped_dead_force_airborne
    hook::call(0x4A1F00, object_set_at_rest_hook); // biped_exit_relaxation
    hook::call(0x4A2396, object_set_at_rest_hook); // biped_start_relaxation
    // inlined object_set_at_rest instances
    hook::insert(0x770F5, 0x77142, object_set_at_rest_hook2, _hook_replace); // UNTESTED!! // c_simulation_generic_entity_definition::create_object
    hook::insert(0x7255B, 0x725A8, object_set_at_rest_hook3, _hook_replace); // UNTESTED!! // c_simulation_vehicle_entity_definition::create_object
    hook::insert(0x400A97, 0x400AD1, object_set_at_rest_hook4, _hook_replace); // object_attach_to_node_immediate
    patch::nop_region(0x400A89, 4); // cleanup redundant instructions
    patch::nop_region(0x400A91, 3); // cleanup redundant instructions
    hook::insert(0x467DB1, 0x467DE7, object_set_at_rest_hook5, _hook_replace); // projectile_attach
    patch::nop_region(0x467D93, 14); // cleanup redundant instructions
    patch::nop_region(0x467DA4, 9); // cleanup redundant instructions
    hook::insert(0x464E82, 0x464EC7, object_set_at_rest_hook6, _hook_replace); // projectile_collision
    hook::insert(0x4623A6, 0x462431, object_set_at_rest_hook7, _hook_replace); // projectile_initial_update (called for conically fired projectiles, ie shotguns)
    patch::nop_region(0x462398, 11); // cleanup redundant instructions
    hook::insert(0x46D200, 0x46D281, object_set_at_rest_hook8, _hook_replace); // UNTESTED!! // object_early_mover_delete
    hook::insert(0x46D2D0, 0x46D343, object_set_at_rest_hook9, _hook_replace); // UNTESTED!! // object_early_mover_delete
    hook::insert(0x6D506C, 0x6D50EF, object_set_at_rest_hook10, _hook_replace); // UNTESTED!! // swarm_accelerate > creature_accelerate inlined
    hook::call(0x48FB9A, object_set_at_rest_hook11); // scenery_new, hooked scenery_animation_idle call
    hook::insert(0x4BEB84, 0x4BEBB7, object_set_at_rest_hook12, _hook_replace); // vehicle_program_activate // havok vehicle physics invalid flag, used only by troop hog back
    hook::insert(0x4BFF31, 0x4BFF76, object_set_at_rest_hook13, _hook_replace); // UNTESTED!! // vehicle_program_update
    hook::call(0x4C7A66, object_set_at_rest_hook14); // unit_custom_animation_play_animation_submit // plays on podium
}