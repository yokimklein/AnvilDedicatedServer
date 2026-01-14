#include "hooks_simulation_events.h"
#include <anvil\hooks\hooks.h>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_damage.h>
#include <simulation\game_interface\simulation_game_projectiles.h>
#include <game\game.h>
#include <items\projectiles.h>
#include <memory\tls.h>
#include <objects\damage.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <items\weapon_definitions.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <game\game_engine_util.h>
#include <anvil\backend\cache.h>

void __cdecl damage_section_deplete_hook(s_hook_registers& registers) // destroy hologram?
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x0C);
    long damage_section_index = *(long*)(registers.ebp + 0x10);
    long response_index = *(long*)(registers.ebp - 0x08);

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

void __cdecl damage_section_respond_to_damage_hook(s_hook_registers& registers) // destroy hologram with powerdrain?
{
    datum_index object_index = *(datum_index*)(registers.ebp - 0x14);
    long damage_section_index = *(long*)(registers.ebp + 0x10);
    long response_index = *(long*)(registers.ebp - 0x18);

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

void __cdecl object_damage_new_hook(s_hook_registers& registers) // throwing deployable cover?
{
    datum_index object_index = (datum_index)registers.edi;
    long damage_section_index = *(long*)(registers.esp + 0x94 - 0x6C);
    long response_index = (long)registers.ebx;

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

//void __cdecl object_apply_damage_aftermath_hook2(s_hook_registers& registers)
//{
//    datum_index object_index = (datum_index)registers.esi;
//    s_damage_aftermath_result_data* result_data = (s_damage_aftermath_result_data*)registers.edi;
//    
//    simulation_action_damage_aftermath(object_index, result_data);
//}

void __cdecl object_cause_damage_hook(s_hook_registers& registers)
{
    bool update_sim_aftermath = *(bool*)(registers.ebp - 0x44);
    datum_index object_index = *(datum_index*)(registers.ebp - 0x2C);
    s_damage_aftermath_result_data* aftermath_result = (s_damage_aftermath_result_data*)(registers.ebp - 0x168);
    bool update_sim_aftermath_list = *(bool*)(registers.ebp - 0x11);
    datum_index* player_indices = (datum_index*)(registers.ebp - 0xA0);

    if (update_sim_aftermath)
    {
        simulation_action_damage_aftermath(object_index, aftermath_result);
    }
    else if (update_sim_aftermath_list)
    {
        // restore player_count
        long player_count = 0;
        for (long i = 0; i < 2; i++)
        {
            if (player_indices[i] != NONE)
            {
                player_count++;
            }
        }
        simulation_action_damage_aftermath_exclusive_list(object_index, aftermath_result, player_indices, player_count);
    }
}

void __cdecl projectile_attach_hook2(s_hook_registers& registers)
{
    datum_index projectile_index = *(datum_index*)(registers.esp + 0x58 - 0x48);
    datum_index object_index = *(datum_index*)(registers.esp + 0x58 - 0x38);
    short node_index = *(short*)(registers.ebp + 0x08);
    real_point3d const* position = *(real_point3d const**)(registers.ebp + 0x0C);
    s_location const* location = *(s_location const**)(registers.ebp + 0x10);

    simulation_action_projectile_attached(projectile_index, object_index, node_index, position, location);
}

// add back missing variables from_impact & detonation_timer_started
void __cdecl projectile_collision_hook0(s_hook_registers& registers)
{
    e_predictability predictability = *(e_predictability*)(registers.esp + 0x258 - 0x240);
    projectile_datum* projectile = *(projectile_datum**)(registers.esp + 0x258 - 0x218);

    bool from_impact = predictability == 0;
    // NOT flag 5, IS flag 4
    bool detonation_timer_started = !projectile->projectile.flags.test(_projectile_flags5) &&
        (projectile->projectile.flags.test(_projectile_flags4) ||
            predictability == _predictability3);

    *(bool*)(registers.ebp + 0x04) = from_impact;
    *(bool*)(registers.ebp + 0x08) = detonation_timer_started;
}

// initialise new_effect variable as false
void __cdecl projectile_collision_hook1(s_hook_registers& registers)
{
    bool* new_effect = (bool*)(registers.ebp + 0x0C);
    *new_effect = false;
}

// set new_effect variable to true
void __cdecl projectile_collision_hook2(s_hook_registers& registers)
{
    bool* new_effect = (bool*)(registers.ebp + 0x0C);
    *new_effect = true;
}

void __cdecl projectile_collision_hook3(s_hook_registers& registers)
{
    bool detonation_timer_started = *(bool*)(registers.ebp + 0x08);
    bool new_effect = *(bool*)(registers.ebp + 0x0C);
    bool from_impact = *(bool*)(registers.ebp + 0x04);
    projectile_datum* projectile = *(projectile_datum**)(registers.esp + 0x258 - 0x218);
    e_predictability predictability = *(e_predictability*)(registers.esp + 0x258 - 0x240);
    bool not_affected_by_object_collision = *(bool*)(registers.esp + 0x258 - 0x249);
    datum_index projectile_index = *(datum_index*)(registers.esp + 0x258 - 0x210);
    real material_effect_scale = *(real*)(registers.esp + 0x258 - 0x208);
    real material_effect_error = *(real*)(registers.esp + 0x258 - 0x1F4);
    real_vector3d const* impact_direction_normal = (real_vector3d const*)(registers.esp + 0x258 - 0x1D0);
    collision_result const* collision = (collision_result const*)registers.edi;

    if ((detonation_timer_started || new_effect)
        && game_is_authoritative()
        && projectile->object.gamestate_index != NONE
        && (from_impact || predictability == _predictability3)
        && !not_affected_by_object_collision)
    {
        simulation_action_projectile_impact_raw(
            detonation_timer_started,
            projectile_index,
            material_effect_scale,
            material_effect_error,
            impact_direction_normal,
            collision,
            from_impact);
    }
}

void __cdecl unit_action_vehicle_board_submit_hook(s_hook_registers& registers)
{
    datum_index unit_index = *(datum_index*)(registers.ebp + 0x08);

    simulation_action_unit_board_vehicle(unit_index);
}

void __cdecl motor_animation_exit_seat_internal_hook(s_hook_registers& registers)
{
    datum_index unit_index = (datum_index)registers.esi;

    simulation_action_unit_exit_vehicle(unit_index);
}

void __cdecl unit_resolve_melee_attack_hook(s_hook_registers& registers)
{
    weapon_definition const* weapon_tag = (weapon_definition const*)registers.edi;
    real_point3d const* position = *(real_point3d const**)(registers.ebp - 0x3C);
    unit_datum const* unit = *(unit_datum const**)(registers.ebp - 0x14);

    simulation_action_unit_melee_clang(weapon_tag->weapon.clash_effect.index, position, &unit->unit.melee_aiming_vector);
}

void __cdecl game_engine_earn_wp_event_hook2(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.esp + 0x40 - 0x28);


    // Anvil: track number of earned wp events to submit stats for later
    c_backend_data_cache::cache_wp_event(event_data);

    game_engine_send_event(event_data);
}

void __cdecl game_engine_scoring_update_leaders_internal_hook(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.ebp - 0x40);

    game_engine_send_event(event_data);
}

void __cdecl game_engine_award_medal_hook(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.esp + 0x38 - 0x28);

    game_engine_send_event(event_data);
}

void __cdecl c_slayer_engine__emit_game_start_event_hook(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.esp + 0x28 - 0x28);

    game_engine_send_event(event_data);
}

void __cdecl display_teleporter_blocked_message_hook(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.ebp - 0x28);

    game_engine_send_event(event_data);
}

void __cdecl c_teleporter_area__update_players_hook(s_hook_registers& registers)
{
    s_game_engine_event_data* event_data = (s_game_engine_event_data*)(registers.ebp - 0x28);

    game_engine_send_event(event_data);
}

void anvil_hooks_simulation_events_apply()
{
    // simulation_action_damage_section_response
    hook::insert(0x414EC5, 0x414ECA, damage_section_deplete_hook, _hook_execute_replaced_first);
    hook::insert(0x414B96, 0x414B9E, damage_section_respond_to_damage_hook, _hook_execute_replaced_first);
    hook::insert(0x40C9C4, 0x40C9C9, object_damage_new_hook, _hook_execute_replaced_first);

    // simulation_action_damage_aftermath
    // We actually don't want this first hook, the call only exists in ms23 as an accidental leftover from H3's code
    // HO uses Reach's code here, and so simulation_action_damage_aftermath was moved to object_cause_damage
    // But this now means there's duplicate calls as the old H3 one wasn't removed
    // This ends up causing duplicate events to be sent to clients, making physics impulses way stronger than they should be
    //hook::insert(0x41320A, 0x413210, object_apply_damage_aftermath_hook2, _hook_execute_replaced_last);
    hook::insert(0x40FB0E, 0x40FB13, object_cause_damage_hook, _hook_execute_replaced_first);

    // simulation_action_projectile_attached
    hook::insert(0x468045, 0x46804B, projectile_attach_hook2, _hook_execute_replaced_last);

    // simulation_action_projectile_detonate
    hook::function(0x4667D0, 0x86, projectile_detonate_effects_and_damage);
    patch::bytes(0x467250, { 0x08 }); // remove unnecessary cleanup bytes handled by fastcall

    // simulation_action_projectile_impact_raw
    hook::add_variable_space_to_stack_frame(0x463930, 0x465667, 12); // add extra 12 bytes of variable space to projectile_collision
    hook::insert(0x464FD4, 0x464FDC, projectile_collision_hook0, _hook_execute_replaced_last);
    hook::insert(0x46528B, 0x465291, projectile_collision_hook1, _hook_execute_replaced_first);
    hook::insert(0x46530B, 0x465313, projectile_collision_hook2, _hook_execute_replaced_first);
    hook::insert(0x4653D3, 0x4653D8, projectile_collision_hook2, _hook_execute_replaced_first);
    hook::insert(0x465454, 0x46545D, projectile_collision_hook2, _hook_execute_replaced_first);
    hook::insert(0x465472, 0x46547A, projectile_collision_hook3, _hook_execute_replaced_first);
    hook::insert(0x465591, 0x465596, (void*)12, _hook_stack_frame_cleanup); // cleanup
    hook::insert(0x4655C3, 0x4655C8, (void*)12, _hook_stack_frame_cleanup); // cleanup
    hook::insert(0x4655F1, 0x4655F6, (void*)12, _hook_stack_frame_cleanup); // cleanup
    hook::insert(0x465657, 0x46565C, (void*)12, _hook_stack_frame_cleanup); // cleanup
    hook::insert(0x46565F, 0x465666, (void*)12, _hook_stack_frame_cleanup); // cleanup

    // simulation_action_unit_board_vehicle - esi unit index
    hook::insert(0x4479F3, 0x4479FA, unit_action_vehicle_board_submit_hook, _hook_execute_replaced_first);

    // simulation_action_unit_exit_vehicle
    hook::insert(0x456CC6, 0x456CCF, motor_animation_exit_seat_internal_hook, _hook_execute_replaced_last);

    // simulation_action_unit_melee_clang
    hook::insert(0x42C270, 0x42C276, unit_resolve_melee_attack_hook, _hook_execute_replaced_last);

    // simulation_action_multiplayer_event
    hook::function(0x11C0C0, 0x40, game_engine_send_event);
    hook::insert(0xFAF89, 0xFAFC3, game_engine_earn_wp_event_hook2, _hook_replace); // inline in game_engine_earn_wp_event
    hook::insert(0xE00DF, 0xE011A, game_engine_scoring_update_leaders_internal_hook, _hook_replace); // score leaders ('x' took the lead!)
    hook::insert(0xFB1D8, 0xFB209, game_engine_award_medal_hook, _hook_replace); // medals
    hook::insert(0x22E2D2, 0x22E308, c_slayer_engine__emit_game_start_event_hook, _hook_replace); // slayer popup on game start
    hook::insert(0x11887F, 0x1188B4, display_teleporter_blocked_message_hook, _hook_replace); // teleporter blocked message
    hook::insert(0x1187EF, 0x118824, c_teleporter_area__update_players_hook, _hook_replace); // teleporter used - actually hooks another function called by update_players but we don't have a name for it
}