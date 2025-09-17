#include "hooks_damage_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game.h>
#include <game\game_globals.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_damage.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <units\units.h>
#include <game\game_time.h>

void __cdecl object_damage_update_hook1(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x20);

    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

void __cdecl object_damage_update_hook2(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x20);

    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

void __cdecl object_damage_update_hook3(s_hook_registers registers)
{
    // preserve xmm2
    __asm
    {
        movd eax, xmm2;
        push eax;
    }

    datum_index object_index = *(datum_index*)(registers.ebp - 0x20);
    unit_datum* unit = (unit_datum*)registers.edi;
    bool unit_updated = *(bool*)(registers.ebp - 0x01);
    real unknown_vitality = *(real*)(registers.ebp - 0x0C);

    real unknown_ticks;
    real new_body_vitality;
    __asm
    {
        movss unknown_ticks, xmm0;
        movss new_body_vitality, xmm3;
    }

    if (unit->object.body_stun_ticks != 0)
    {
        if (!game_is_predicted())
        {
            unit->object.body_stun_ticks--;
            unit_updated = true;
        }
    }
    else
    {
        TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
        real vitality_delta = game_time_globals->tick_length * unknown_ticks;
        if (TEST_BIT(_object_mask_unit, unit->object.object_identifier.m_type.get()))
        {
            real vitality_multiplier = game_difficulty_get_team_value(_game_difficulty_enemy_recharge, unit->unit.team.get());
            new_body_vitality = unknown_vitality;
            vitality_delta = vitality_delta * vitality_multiplier;
        }
        unit->object.damage_flags.set(_object_damage_flag_bit9, true); // enable _model_shield_depletion_is_permanent_bit?
        real vitality_result = unit->object.body_vitality + vitality_delta;
        unit->object.body_vitality = vitality_result;
        if (vitality_result > new_body_vitality)
        {
            unit->object.body_vitality = new_body_vitality;
            unit->object.damage_flags.set(_object_damage_flag_bit9, false); // disable _model_shield_depletion_is_permanent_bit?
        }
        else
        {
            unit_updated = true;
        }
    }
    simulation_action_object_update(object_index, _simulation_object_update_body_vitality);

    // set unit_updated value back to its original address
    *(bool*)(registers.ebp - 0x01) = unit_updated;
    __asm
    {
        // restore xmm2
        pop eax;
        movd xmm2, eax;
    }
}

void __cdecl object_damage_shield_hook1(s_hook_registers registers)
{
    player_datum* player_data = (player_datum*)registers.esi;

    simulation_action_object_update(player_data->unit_index, _simulation_object_update_shield_vitality);
}

void __cdecl object_damage_shield_hook2(s_hook_registers registers)
{
    datum_index object_index = (datum_index)registers.esi;

    simulation_action_object_update(object_index, _simulation_object_update_shield_vitality);
}

void __cdecl object_damage_body_hook1(s_hook_registers registers)
{
    datum_index object_index = (datum_index)registers.edx;
    real damage = *(real*)(registers.edi + 0x24);

    if (damage > 0.0)
    {
        simulation_action_object_update(object_index, _simulation_object_update_body_vitality);
    }
}

void __cdecl object_deplete_body_internal_hook1(s_hook_registers registers)
{
    datum_index object_index = (datum_index)registers.edi;

    simulation_action_object_update(object_index, _simulation_object_update_dead);
}

void __cdecl damage_section_response_fire_hook(s_hook_registers registers)
{
    object_datum* object = *(object_datum**)(registers.esp + 0x98 - 0x74);
    datum_index object_index = *(datum_index*)(registers.esp + 0x98 - 0x84);
    long damage_section_index = *(long*)(registers.ebp + 0x10);
    long response_index = *(long*)(registers.ebp + 0x14);

    if (!game_is_predicted() && object->object.gamestate_index != NONE)
    {
        c_simulation_object_update_flags update_flags;
        simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_all_damage);
        update_flags.set_flag(object_index, _simulation_object_update_region_state);
        update_flags.set_flag(object_index, _simulation_object_update_constraint_state);
        simulation_action_object_update_internal(object_index, update_flags);
    }
}

// wrapper for usercall (caller cleanup fastcall)
__declspec(naked) void object_set_damage_owner_hook1()
{
    __asm
    {
        push [esp + 0x04] // skip update argument
        call object_set_damage_owner;
        ret; // return w/o cleaning up original argument push
    }
}

void __cdecl object_set_damage_owner_hook2(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.edi + 0x30);

    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

void __cdecl object_set_damage_owner_hook3(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x18);

    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

void __cdecl object_set_damage_owner_hook4(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x10);

    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

void __cdecl object_set_damage_owner_hook5(s_hook_registers registers)
{
    datum_index object_index = (datum_index)registers.ebx;

    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

void __cdecl object_set_damage_owner_hook6(s_hook_registers registers)
{
    datum_index object_index = *(datum_index*)(registers.ebp + 0x08);

    simulation_action_object_update(object_index, _simulation_object_update_owner_team_index);
}

void anvil_hooks_damage_updates_apply()
{
    // object_damage_update
    hook::insert(0x40D4CE, 0x40D553, object_damage_update_hook1, _hook_replace_no_nop); // sync shield recharge vitality
    hook::insert(0x40D526, 0x40D542, object_damage_update_hook2, _hook_execute_replaced_last, true); // sync shield recharge vitality
    hook::insert(0x40D5C1, 0x40D660, object_damage_update_hook3, _hook_replace); // sync body recharge vitality

    // object_damage_shield - syncs immediate shield values on damage
    hook::insert(0x41268C, 0x412694, object_damage_shield_hook1, _hook_execute_replaced_first); // shield vamparism trait
    hook::insert(0x41287B, 0x412881, object_damage_shield_hook2, _hook_execute_replaced_first); // if this runs first, object_index should be popped back into esi
    
    // object_damage_body
    hook::insert(0x411E3C, 0x411E43, object_damage_body_hook1, _hook_execute_replaced_last); // sync object body vitality on damage
    
    // object_deplete_body_internal
    hook::insert(0x40D9D3, 0x40D9DA, object_deplete_body_internal_hook1, _hook_execute_replaced_last); // sync object death
    
    // damage_section_response_fire
    hook::insert(0x413D3F, 0x413D47, damage_section_response_fire_hook, _hook_execute_replaced_last); // includes simulation_action_damage_section_response
    
    // object_set_damage_owner
    hook::function(0x404320, 0x75, object_set_damage_owner_hook1);
    hook::insert(0x113B0F, 0x113B15, object_set_damage_owner_hook2, _hook_execute_replaced_first); // inlined in event_generate_accelerations
    hook::insert(0x20CF07, 0x20CF0D, object_set_damage_owner_hook3, _hook_execute_replaced_first); // inlined in havok_collision_damage_update
    hook::insert(0x40F00C, 0x40F012, object_set_damage_owner_hook4, _hook_execute_replaced_first); // inlined in object_cause_damage
    hook::insert(0x4572A5, 0x4572AB, object_set_damage_owner_hook5, _hook_execute_replaced_first); // inlined in motor_animation_exit_seat_immediate_internal
    hook::insert(0x4BEA5D, 0x4BEA63, object_set_damage_owner_hook6, _hook_execute_replaced_first); // inlined in vehicle_flip_submit
}