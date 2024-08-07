#include "hooks_damage_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game.h>
#include <game\game_globals.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_damage.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <units\units.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall object_damage_update_hook1()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x100, sizeof(object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x20];
    __asm mov object_index, eax;

    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_update_hook2()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x100, sizeof(object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x20];
    __asm mov object_index, eax;

    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_update_hook3()
{
    // preserve xmm2
    __asm movd eax, xmm2;
    __asm push eax;

    datum_index object_index;
    s_unit_data* unit;
    bool unit_updated;
    real unknown_ticks;
    real new_body_vitality;
    real unknown_vitality;
    DEFINE_ORIGINAL_EBP_ESP(0x100, 0x50);
    __asm
    {
        mov ecx, original_ebp;

        mov eax, [ecx - 0x20];
        mov object_index, eax;

        mov unit, edi;

        mov al, [ecx - 0x01]; // [ebp - 0x01]
        mov unit_updated, al;

        movss unknown_ticks, xmm0;
        movss new_body_vitality, xmm3;

        movss xmm1, [ecx - 0x0C]; // [ebp - 0x0C]
        movss unknown_vitality, xmm1;
    }

    if (unit->body_stun_ticks != 0)
    {
        if (!game_is_predicted())
        {
            unit->body_stun_ticks--;
            unit_updated = true;
        }
    }
    else
    {
        TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
        real vitality_delta = game_time_globals->tick_length * unknown_ticks;
        if (TEST_BIT(_object_mask_unit, unit->object_identifier.m_type.get()))
        {
            // TODO: see if this gets called and works now I'm not using a broken version of the call
            real vitality_multiplier = game_difficulty_get_team_value(_game_difficulty_enemy_recharge, unit->team.get());
            new_body_vitality = unknown_vitality;
            vitality_delta = vitality_delta * vitality_multiplier;
        }
        unit->damage_flags |= FLAG(9); // enable _model_shield_depletion_is_permanent_bit?
        real vitality_result = unit->body_vitality + vitality_delta;
        unit->body_vitality = vitality_result;
        if (vitality_result > new_body_vitality)
        {
            unit->body_vitality = new_body_vitality;
            unit->damage_flags &= ~FLAG(9); // disable _model_shield_depletion_is_permanent_bit?
        }
        else
        {
            unit_updated = true;
        }
    }
    simulation_action_object_update(object_index, _simulation_object_update_body_vitality);

    __asm
    {
        // set unknown_bool value back to its original address
        mov ecx, original_ebp;
        mov al, unit_updated;
        mov [ecx - 0x01], al; // [ebp - 0x01]

        // restore xmm2
        pop eax;
        movd xmm2, eax;
    }
}

__declspec(safebuffers) void __fastcall object_damage_shield_hook1()
{
    s_player_datum* player_data;
    __asm mov player_data, esi;
    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_shield_hook2()
{
    datum_index object_index;
    __asm mov object_index, esi;
    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

__declspec(safebuffers) void __fastcall object_damage_body_hook1()
{
    datum_index object_index;
    float damage;

    __asm mov object_index, edx;
    __asm movss xmm0, [edi + 0x24];
    __asm movss damage, xmm0;

    if (damage > 0.0)
    {
        simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
    }
}

__declspec(safebuffers) void __fastcall object_deplete_body_internal_hook1()
{
    datum_index object_index;
    __asm mov object_index, edi;
    simulation_action_object_update(object_index, _simulation_object_update_dead);
}

__declspec(safebuffers) void __fastcall damage_section_response_fire_hook()
{
    s_object_data* object;
    datum_index object_index;
    long damage_section_index;
    long response_index;
    c_simulation_object_update_flags update_flags;
    DEFINE_ORIGINAL_EBP_ESP(0x9C, sizeof(object) + sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index) + sizeof(update_flags));

    __asm
    {
        mov ecx, original_esp;
        mov eax, [ecx + 0x98 - 0x74]; // sp 0x98, [esp+0x94-0x74]
        mov object, eax;
        mov eax, [ecx + 0x98 - 0x84]; // sp 0x98, [esp+0x94-0x84]
        mov object_index, eax;

        mov ecx, original_ebp;
        mov eax, [ecx + 0x10]; // [ebp + 0x10]
        mov damage_section_index, eax;
        mov eax, [ecx + 0x14]; // [ebp + 0x14]
        mov response_index, eax;
    }

    if (!game_is_predicted() && object->gamestate_index != -1)
    {
        simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_all_damage);
        update_flags.set_flag(object_index, _simulation_object_update_region_state);
        update_flags.set_flag(object_index, _simulation_object_update_constraint_state);
        simulation_action_object_update_internal(object_index, update_flags);
    }
}

// Wrapper for usercall
__declspec(naked) void object_set_damage_owner_hook(datum_index object_index, s_damage_owner* damage_owner, bool skip_update)
{
    // prologue
    __asm
    {
        push        ebp
        mov         ebp, esp
        sub         esp, 0x48
        push        ebx
        push        esi
        push        edi
        mov         edx, [ebp - 0x8]
        mov         ecx, [ebp - 0x4]
    }
    object_set_damage_owner(object_index, damage_owner, skip_update);
    // epilogue
    __asm
    {
        pop        edi
        pop        esi
        pop        ebx
        mov        esp, ebp
        pop        ebp
        ret // usercall - let caller cleanup 4 bytes pushed to the stack
    }
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook2()
{
    datum_index object_index;
    __asm mov eax, [edi + 0x30];
    __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook3()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0xE68, sizeof(object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x18];
    __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook4()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x178, sizeof(object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x10];
    __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook5()
{
    datum_index object_index;
    __asm mov object_index, ebx;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

__declspec(safebuffers) void __fastcall object_set_damage_owner_hook6()
{
    datum_index object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x8C, sizeof(object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 0x08];
    __asm mov object_index, eax;
    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}
#pragma runtime_checks("", restore)

void anvil_hooks_damage_updates_apply()
{
    // object_damage_update
    insert_hook(0x40D4CE, 0x40D553, object_damage_update_hook1, _hook_replace_no_nop); // sync shield recharge vitality
    insert_hook(0x40D526, 0x40D542, object_damage_update_hook2, _hook_execute_replaced_last, true); // sync shield recharge vitality
    insert_hook(0x40D5C1, 0x40D660, object_damage_update_hook3, _hook_replace); // sync body recharge vitality

    // object_damage_shield - syncs immediate shield values on damage
    insert_hook(0x41268C, 0x412694, object_damage_shield_hook1, _hook_execute_replaced_first); // shield vamparism trait
    insert_hook(0x41287B, 0x412881, object_damage_shield_hook2, _hook_execute_replaced_first); // if this runs first, object_index should be popped back into esi
    
    // object_damage_body
    insert_hook(0x411E3C, 0x411E43, object_damage_body_hook1, _hook_execute_replaced_last); // sync object body vitality on damage
    
    // object_deplete_body_internal
    insert_hook(0x40D9D3, 0x40D9DA, object_deplete_body_internal_hook1, _hook_execute_replaced_last); // sync object death
    
    // damage_response_fire
    insert_hook(0x413D47, 0x413D4F, damage_section_response_fire_hook, _hook_execute_replaced_first); // includes simulation_action_damage_section_response
    
    // object_set_damage_owner
    hook_function(0x404320, 0x75, object_set_damage_owner_hook);
    insert_hook(0x113B0F, 0x113B15, object_set_damage_owner_hook2, _hook_execute_replaced_first); // inlined in event_generate_accelerations
    insert_hook(0x20CF07, 0x20CF0D, object_set_damage_owner_hook3, _hook_execute_replaced_first); // inlined in havok_collision_damage_update
    insert_hook(0x40F00C, 0x40F012, object_set_damage_owner_hook4, _hook_execute_replaced_first); // inlined in object_cause_damage
    insert_hook(0x4572A5, 0x4572AB, object_set_damage_owner_hook5, _hook_execute_replaced_first); // inlined in motor_animation_exit_seat_immediate_internal
    insert_hook(0x4BEA5D, 0x4BEA63, object_set_damage_owner_hook6, _hook_execute_replaced_first); // inlined in vehicle_flip_submit
}