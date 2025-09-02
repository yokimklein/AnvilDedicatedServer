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

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall damage_section_deplete_hook() // destroy hologram?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x44, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov ecx, original_ebp;
    __asm mov eax, [ecx - 0x0C];
    __asm mov object_index, eax;
    __asm mov eax, [ecx + 0x10];
    __asm mov damage_section_index, eax;
    __asm mov eax, [ecx - 0x08];
    __asm mov response_index, eax;

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

__declspec(safebuffers) void __fastcall damage_section_respond_to_damage_hook() // destroy hologram with powerdrain?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x40, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov ecx, original_ebp;
    __asm mov eax, [ecx - 0x14];
    __asm mov object_index, eax;
    __asm mov eax, [ecx + 0x10];
    __asm mov damage_section_index, eax;
    __asm mov eax, [ecx - 0x18];
    __asm mov response_index, eax;
    
    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

__declspec(safebuffers) void __fastcall object_damage_new_hook() // throwing deployable cover?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x94, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov object_index, edi;
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x94 - 0x6C];
    __asm mov damage_section_index, eax;
    __asm mov response_index, ebx;

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

//__declspec(safebuffers) void __fastcall object_apply_damage_aftermath_hook2()
//{
//    datum_index object_index;
//    s_damage_aftermath_result_data* result_data;
//    __asm mov object_index, esi;
//    __asm mov result_data, edi;
//    simulation_action_damage_aftermath(object_index, result_data);
//}

__declspec(safebuffers) void __fastcall object_cause_damage_hook()
{
    bool update_sim_aftermath;
    datum_index object_index;
    s_damage_aftermath_result_data* aftermath_result;
    bool update_sim_aftermath_list;
    datum_index* player_indices;
    long player_count;
    DEFINE_ORIGINAL_EBP_ESP(0x178,
        (sizeof(update_sim_aftermath) + 3) + sizeof(object_index) + sizeof(aftermath_result) +
        (sizeof(update_sim_aftermath_list) + 3) + sizeof(player_indices) + sizeof(player_count) + 4); // extra 4 for loop iterator

    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx - 0x44];
        mov update_sim_aftermath, al;

        mov eax, [ecx - 0x2C];
        mov object_index, eax;

        lea eax, [ecx - 0x168];
        mov aftermath_result, eax;

        mov eax, [ecx - 0x11];
        mov update_sim_aftermath_list, al;

        lea eax, [ecx - 0xA0];
        mov player_indices, eax;
    }

    if (update_sim_aftermath)
    {
        simulation_action_damage_aftermath(object_index, aftermath_result);
    }
    else if (update_sim_aftermath_list)
    {
        // restore player_count
        player_count = 0;
        for (long i = 0; i < 2; i++)
        {
            if (player_indices[i] != NONE)
                player_count++;
        }
        simulation_action_damage_aftermath_exclusive_list(object_index, aftermath_result, player_indices, player_count);
    }
}

__declspec(safebuffers) void __fastcall projectile_attach_hook2()
{
    datum_index projectile_index;
    datum_index object_index;
    short node_index;
    real_point3d const* position;
    s_location const* location;
    DEFINE_ORIGINAL_EBP_ESP(0x58, sizeof(projectile_index) + sizeof(object_index) + (sizeof(node_index) + 2) + sizeof(position) + sizeof(location));

    __asm
    {
        mov ecx, original_esp;
        mov eax, [ecx + 0x58 - 0x48];
        mov projectile_index, eax;

        mov eax, [ecx + 0x58 - 0x38];
        mov object_index, eax;

        mov ecx, original_ebp;
        mov eax, [ecx + 0x08];
        mov node_index, ax;

        mov eax, [ecx + 0x0C];
        mov position, eax;

        mov eax, [ecx + 0x10];
        mov location, eax;
    }

    simulation_action_projectile_attached(projectile_index, object_index, node_index, position, location);
}

// add back missing variables from_impact & detonation_timer_started
__declspec(safebuffers) void __fastcall projectile_collision_hook0()
{
    c_enum<e_predictability, long, _predictability0, k_predictability_count> predictability;
    projectile_datum* projectile;
    bool from_impact;
    bool detonation_timer_started;
    // 4 bytes for each bool or 4 bytes to share?
    DEFINE_ORIGINAL_EBP_ESP(0x258, sizeof(predictability) + sizeof(projectile) + (sizeof(from_impact) + 3) + (sizeof(detonation_timer_started) + 3) + 8);
    __asm
    {
        mov ecx, original_esp;
        mov eax, [ecx + 0x258 - 0x240];
        mov predictability, eax;

        mov eax, [ecx + 0x258 - 0x218];
        mov projectile, eax;
    }

    from_impact = predictability == 0;
    // NOT flag 5, IS flag 4
    detonation_timer_started = !projectile->projectile.flags.test(_projectile_flags5)
        && (projectile->projectile.flags.test(_projectile_flags4)
            || predictability.get() == _predictability3);

    __asm
    {
        mov ecx, original_ebp;
        mov al, from_impact;
        mov [ecx + 4], al;

        mov al, detonation_timer_started;
        mov [ecx + 8], al;
    }
}

// initialise new_effect variable as false
__declspec(naked) void projectile_collision_hook1()
{
    __asm
    {
        mov[ebp + 12], eax; // xor eax, eax just ran to zero two other variables
        retn;
    }
}

// set new_effect variable to true
__declspec(naked) void projectile_collision_hook2()
{
    __asm
    {
        mov [ebp + 12], 1;
        retn;
    }
}

__declspec(safebuffers) void __fastcall projectile_collision_hook3()
{
    bool detonation_timer_started;
    bool new_effect;
    bool from_impact;
    projectile_datum* projectile;
    c_enum<e_predictability, long, _predictability0, k_predictability_count> predictability;
    bool not_affected_by_object_collision;
    datum_index projectile_index;
    float material_effect_scale;
    float material_effect_error;
    real_vector3d const* impact_direction_normal;
    collision_result const* collision;
    DEFINE_ORIGINAL_EBP_ESP(0x258, 0x3C);

    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx + 8];
        mov detonation_timer_started, al;

        mov eax, [ecx + 12];
        mov new_effect, al;

        mov eax, [ecx + 4];
        mov from_impact, al;

        mov ecx, original_esp;
        mov eax, [ecx + 0x258 - 0x218];
        mov projectile, eax;

        mov eax, [ecx + 0x258 - 0x240];
        mov predictability, eax;

        mov eax, [ecx + 0x258 - 0x249];
        mov not_affected_by_object_collision, al;

        mov eax, [ecx + 0x258 - 0x210];
        mov projectile_index, eax;

        movss xmm0, [ecx + 0x258 - 0x208];
        movss material_effect_scale, xmm0;

        movss xmm0, [ecx + 0x258 - 0x1F4];
        movss material_effect_error, xmm0;

        lea eax, [ecx + 0x258 - 0x1D0];
        mov impact_direction_normal, eax;

        mov collision, edi;
    }

    if ((detonation_timer_started || new_effect)
        && game_is_authoritative()
        && projectile->object.gamestate_index != NONE
        && (from_impact || predictability.get() == _predictability3)
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

__declspec(safebuffers) void __fastcall unit_action_vehicle_board_submit_hook()
{
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0xDC, sizeof(unit_index));
    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx + 0x08];
        mov unit_index, eax;
    }
    simulation_action_unit_board_vehicle(unit_index);
}

__declspec(safebuffers) void __fastcall motor_animation_exit_seat_internal_hook()
{
    datum_index unit_index;
    __asm mov unit_index, esi;
    simulation_action_unit_exit_vehicle(unit_index);
}

__declspec(safebuffers) void __fastcall unit_resolve_melee_attack_hook()
{
    weapon_definition const* weapon_tag;
    real_point3d const* position;
    unit_datum const* unit;
    DEFINE_ORIGINAL_EBP_ESP(0xAC, sizeof(weapon_tag) + sizeof(position) + sizeof(unit));
    __asm
    {
        mov weapon_tag, edi;

        mov ecx, original_ebp;
        lea eax, [ecx - 0x3C];
        mov position, eax;

        mov eax, [ecx - 0x14];
        mov unit, eax;
    }
    simulation_action_unit_melee_clang(weapon_tag->weapon.clash_effect.index, position, &unit->unit.melee_aiming_vector);
}

__declspec(safebuffers) void __fastcall game_engine_earn_wp_event_hook2()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x40, sizeof(event_data));
    __asm
    {
        mov ecx, original_esp;
        lea eax, [ecx + 0x40 - 0x28];
        mov event_data, eax;
    }

    // Anvil: track number of earned wp events to submit stats for later
    c_backend_data_cache::cache_wp_event(event_data);

    game_engine_send_event(event_data);
}

__declspec(safebuffers) void __fastcall game_engine_scoring_update_leaders_internal_hook()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x50, sizeof(event_data));
    __asm
    {
        mov ecx, original_ebp;
        lea eax, [ecx - 0x40];
        mov event_data, eax;
    }
    game_engine_send_event(event_data);
}

__declspec(safebuffers) void __fastcall game_engine_award_medal_hook()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x3C, sizeof(event_data));
    __asm
    {
        mov ecx, original_esp;
        lea eax, [ecx + 0x38 - 0x28];
        mov event_data, eax;
    }
    game_engine_send_event(event_data);
}

__declspec(safebuffers) void __fastcall c_slayer_engine__emit_game_start_event_hook()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x28, sizeof(event_data));
    __asm
    {
        mov ecx, original_esp;
        lea eax, [ecx + 0x28 - 0x28];
        mov event_data, eax;
    }
    game_engine_send_event(event_data);
}

__declspec(safebuffers) void __fastcall display_teleporter_blocked_message_hook()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x28, sizeof(event_data));
    __asm
    {
        mov ecx, original_ebp;
        lea eax, [ecx - 0x28];
        mov event_data, eax;
    }
    game_engine_send_event(event_data);
}

__declspec(safebuffers) void __fastcall c_teleporter_area__update_players_hook()
{
    s_game_engine_event_data* event_data;
    DEFINE_ORIGINAL_EBP_ESP(0x28, sizeof(event_data));
    __asm
    {
        mov ecx, original_ebp;
        lea eax, [ecx - 0x28];
        mov event_data, eax;
    }
    game_engine_send_event(event_data);
}
#pragma runtime_checks("", restore)

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