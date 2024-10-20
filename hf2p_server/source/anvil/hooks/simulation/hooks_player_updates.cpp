#include "hooks_player_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <hf2p\loadouts.h>
#include <game\player_mapping.h>
#include <units\units.h>
#include <simulation\simulation_queue_global_events.h>
#include <game\game.h>
#include <game\game_engine_util.h>
#include <Patch.hpp>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall player_spawn_hook1()
{
    player_datum* player_data;
    datum_index player_index;
    __asm mov player_data, ebx;
    __asm mov player_index, esi;

    if (player_data->flags.test(_player_initial_spawn_bit))
    {
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_consumable_supression);
    }
}

__declspec(safebuffers) void __fastcall player_spawn_hook2()
{
    datum_index player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x21C, sizeof(player_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x18];
    __asm mov player_index, eax;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_spawn_timer);
}

__declspec(safebuffers) void __fastcall player_spawn_hook3()
{
    datum_index player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x21C, sizeof(player_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax - 0x18];
    __asm mov player_index, eax;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

__declspec(safebuffers) void __fastcall equipment_handle_energy_cost_hook2()
{
    unit_datum* unit;
    __asm mov unit, ebx;

    simulation_action_game_engine_player_update(unit->unit.player_index, _simulation_player_update_consumable_supression);
}

__declspec(safebuffers) void __fastcall player_update_loadout_hook1()
{
    datum_index player_index;
    player_datum* player_data;

    __asm mov player_index, esi;
    __asm mov player_data, ebx;

    player_update_loadout(player_index, player_data);
}

__declspec(safebuffers) void __fastcall player_update_loadout_hook2()
{
    player_datum* player;
    __asm mov player, esi;
    datum_index player_index = player_mapping_get_player_by_input_user(_input_user_index0);
    player_update_loadout(player_index, player);
}

__declspec(safebuffers) void __fastcall player_reset_hook()
{
    datum_index player_index;
    player_datum* player;
    __asm
    {
        mov player_index, ebx;
        mov player, edi;
    }
    player_update_loadout(player_index, player);
}

__declspec(safebuffers) void __fastcall game_engine_update_hook()
{
    for (long i = 0; i < k_maximum_players; i++)
    {
        simulation_action_game_engine_player_update(i, _simulation_player_update_netdebug);
    }
}

__declspec(safebuffers) void __fastcall players_update_after_game_hook1()
{
    datum_index player_index;
    __asm mov player_index, ebx;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_blocking_teleporter);
}

__declspec(safebuffers) void __fastcall players_update_after_game_hook2()
{
    datum_index player_index;
    player_datum* player;
    __asm
    {
        mov player_index, ebx;
        mov player, esi;
    }
    if (player->equipment_cooldown_ticks > 0)
        player->equipment_cooldown_ticks--;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_consumable_supression);
}

__declspec(safebuffers) void __fastcall players_update_after_game_hook3()
{
    datum_index player_index;
    player_datum* player;
    __asm
    {
        mov player_index, ebx;
        mov player, esi;
    }
    player->vehicle_entrance_ban_ticks--;
    if (player->vehicle_entrance_ban_ticks == 0)
        player->flags.set(_player_vehicle_entrance_ban_bit, false);
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_vehicle_entrance_ban);
}

__declspec(safebuffers) void __fastcall game_engine_player_killed_hook1()
{
    datum_index dead_player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x40C0, sizeof(dead_player_index));
    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx + 0x08];
        mov dead_player_index, eax;
    }

    if (game_is_multiplayer() && game_is_authoritative() && game_engine_in_round())
    {
        simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_last_killer);
    }
}

__declspec(safebuffers) void __fastcall c_game_statborg__record_player_death_hook1()
{
    datum_index dead_player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x14, sizeof(dead_player_index));
    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx + 0x08];
        mov dead_player_index, eax;
    }
    simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_grief_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_player_fired_weapon_hook()
{
    datum_index player_index;
    __asm mov player_index, ecx;
    game_engine_set_player_navpoint_action(player_index, _navpoint_action_fired_weapon);
}

__declspec(safebuffers) void __fastcall game_engine_player_damaged_player_hook()
{
    datum_index player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x0C, sizeof(player_index));
    __asm
    {
        mov ecx, original_ebp;
        mov eax, [ecx + 0x08];
        mov player_index, eax;
    }
    game_engine_set_player_navpoint_action(player_index, _navpoint_action_player_damaged);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_update_state_hook3()
{
    c_player_in_game_iterator player_iterator;
    player_iterator.begin();
    while (player_iterator.next())
    {
        e_shield_multiplier_setting shield_multiplier = current_game_variant()->get_active_variant()->get_map_override_options()->get_base_player_traits()->get_shield_vitality_traits()->get_shield_multiplier_setting();
        player_iterator.get_datum()->traits.get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(shield_multiplier, true);
        simulation_action_game_engine_player_update(player_iterator.get_index(), _simulation_player_update_shield_vitality_traits);
    }
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_update_state_hook4()
{
    c_player_in_game_iterator* player_iterator;
    DEFINE_ORIGINAL_EBP_ESP(0x20, sizeof(player_iterator));
    __asm
    {
        mov ecx, original_ebp;
        lea eax, [ecx - 0x10];
        mov player_iterator, eax;
    }
    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_lives_remaining);
}

__declspec(safebuffers) void __fastcall game_engine_update_player_hook2()
{
    datum_index player_index;
    __asm mov player_index, esi;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_grief_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_update_player_sitting_out_hook()
{
    c_player_in_game_iterator* player_iterator;
    DEFINE_ORIGINAL_EBP_ESP(0x10, sizeof(player_iterator));
    __asm
    {
        mov ecx, original_ebp;
        lea eax, [ecx - 0x10];
        mov player_iterator, eax;
    }
    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_sitting_out);
}

__declspec(safebuffers) void __fastcall game_engine_player_changed_indices_hook1()
{
    datum_index player1_index;
    datum_index player2_index;
    c_flags<long, ulong64, 64> update_flags;
    DEFINE_ORIGINAL_EBP_ESP(0x3394, sizeof(player1_index) + sizeof(player2_index) + sizeof(update_flags));
    __asm
    {
        mov player1_index, ebx;
        mov ecx, original_esp;
        mov eax, [ecx + 0x3390 - 0x337C];
        mov player2_index, eax;
    }
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player1_index), &update_flags);
    simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player2_index), &update_flags);
}

__declspec(safebuffers) void __fastcall game_engine_player_changed_indices_hook2()
{
    datum_index player1_index;
    datum_index player2_index;
    __asm
    {
        mov player1_index, edi;
        mov player2_index, ebx;
    }
    c_flags<long, ulong64, 64> update_flags;
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player1_index), &update_flags);
    simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player2_index), &update_flags);
}

__declspec(safebuffers) void __fastcall player_delete_hook()
{
    datum_index player_index;
    __asm mov player_index, edi;
    c_flags<long, ulong64, 64> update_flags;
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), &update_flags);
}

__declspec(safebuffers) void __fastcall game_engine_player_killed_hook2()
{
    short lives_remaining;
    player_datum* dead_player;
    datum_index dead_player_index;
    __asm
    {
        mov lives_remaining, cx;
        mov dead_player, edx;
        mov dead_player_index, edi;
    }
    if (lives_remaining == 0 && game_engine_has_teams() && game_engine_teams_use_one_shared_life(dead_player->configuration.host.team_index))
    {
        dead_player->lives++;
    }
    simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_lives_remaining);
}

__declspec(safebuffers) void __fastcall game_engine_player_left_hook()
{
    datum_index player_index;
    __asm mov player_index, ebx;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_active_in_game);
}

__declspec(safebuffers) void __fastcall game_engine_player_rejoined_hook()
{
    datum_index player_index;
    __asm mov player_index, esi;
    if (game_is_authoritative())
    {
        c_flags<long, ulong64, 64> update_flags;
        update_flags.set(_simulation_player_update_shield_vitality_traits, true);
        update_flags.set(_simulation_player_update_weapon_traits, true);
        update_flags.set(_simulation_player_update_movement_traits, true);
        update_flags.set(_simulation_player_update_appearance_traits, true);
        update_flags.set(_simulation_player_update_sensor_traits, true);
        simulation_action_game_engine_player_update(DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), &update_flags);
    }
}

__declspec(safebuffers) void __fastcall game_engine_setup_player_for_respawn_hook()
{
    datum_index player_index;
    DEFINE_ORIGINAL_EBP_ESP(0x90, sizeof(player_index));
    __asm
    {
        mov ecx, original_esp;
        mov eax, [ecx + 0x90 - 0x84];
        mov player_index, eax;
    }
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

__declspec(safebuffers) void __fastcall objective_game_player_forced_base_respawn_hook()
{
    datum_index player_index;
    __asm mov player_index, edi;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

__declspec(safebuffers) void __fastcall player_killed_player_perform_respawn_on_kill_check_hook()
{
    c_player_in_game_iterator* player_iterator;
    __asm mov player_iterator, ecx;
    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_early_respawn);
}

__declspec(safebuffers) void __fastcall game_engine_reset_player_respawn_timers_hook()
{
    c_player_in_game_iterator* player_iterator;
    __asm mov player_iterator, ecx;
    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_early_respawn);
}

__declspec(safebuffers) void __fastcall teleporter_teleport_object_hook()
{
    datum_index player_index;
    __asm mov player_index, esi;
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_control_aiming);
}
#pragma runtime_checks("", restore)

void anvil_hooks_player_updates_apply()
{
    // sync equipment charges on spawn
    insert_hook(0xBB093, 0xBB098, player_spawn_hook1, _hook_execute_replaced_first);

    // sync equipment cooldown reset
    insert_hook(0x42D3ED, 0x42D3F2, equipment_handle_energy_cost_hook2, _hook_execute_replaced_first); // sets cooldown after use
    insert_hook(0xB80BA, 0xB80C8, players_update_after_game_hook2, _hook_replace); // updates tick countdown

    // sync spawn timer
    insert_hook(0xBB435, 0xBB43B, player_spawn_hook2, _hook_execute_replaced_first);
    hook_function(0xC7700, 0x62, game_engine_player_set_spawn_timer);

    // sync early respawn
    insert_hook(0xBB459, 0xBB460, player_spawn_hook3, _hook_execute_replaced_first);
    insert_hook(0xFA088, 0xFA08F, game_engine_setup_player_for_respawn_hook, _hook_execute_replaced_first);
    insert_hook(0xFC021, 0xFC028, objective_game_player_forced_base_respawn_hook, _hook_execute_replaced_first);
    insert_hook(0x1C9FA4, 0x1C9FB0, player_killed_player_perform_respawn_on_kill_check_hook, _hook_execute_replaced_first); // inlined game_engine_reset_player_respawn_timer
    insert_hook(0xFBB7E, 0xFBB86, game_engine_reset_player_respawn_timers_hook, _hook_execute_replaced_first);

    // update spectating player
    hook_function(0x68B40, 0x80, c_simulation_player_respawn_request_event_definition__apply_game_event);

    // sync loadout index
    insert_hook(0xBAF22, 0xBAF29, player_update_loadout_hook1, _hook_replace); // add player_index argument back to call in player_spawn
    insert_hook(0xE05A8, 0xE05AF, player_update_loadout_hook2, _hook_replace); // add player_index argument back to call in equipment_add
    hook_function(0xE0660, 0x40, player_update_loadout);
    insert_hook(0xB5259, 0xB532D, player_reset_hook, _hook_replace); // replace inlined player_update_loadout, added new since ms23

    // sync player netdebug data
    insert_hook(0xC9ADD, 0xC9AE3, game_engine_update_hook, _hook_execute_replaced_last);

    // sync player active in game flag
    hook_function(0x54A70, 0x68, simulation_queue_player_event_apply_set_activation);

    // sync spectating player after boot
    hook_function(0x54AE0, 0x43, game_engine_boot_player_safe); // UNTESTED!!

    // sync player booting
    hook_function(0xCCB20, 0xF5, game_engine_boot_player);

    // sync vehicle entrance ban after hijack
    hook_function(0xBFB90, 0x79, player_notify_vehicle_ejection_finished); // set ban time
    insert_hook(0xB80FE, 0xB8112, players_update_after_game_hook3, _hook_replace); // countdown ban ticks

    // sync telefrag
    insert_hook(0xB80A6, 0xB80AD, players_update_after_game_hook1, _hook_execute_replaced_last); // countdown blocking ticks

    // sync revenge
    insert_hook(0xF8FCB, 0xF8FD2, game_engine_player_killed_hook1, _hook_execute_replaced_first); // inlined player_set_revenge_shield_boost

    // sync betrayal grief
    insert_hook(0x1B0248, 0x1B024E, c_game_statborg__record_player_death_hook1, _hook_execute_replaced_first); // inlined game_engine_respond_to_betrayal
    insert_hook(0xC96E5, 0xC96EF, game_engine_update_player_hook2, _hook_execute_replaced_first);

    // sync waypoint actions
    insert_hook(0xFA0B9, 0xFA1B4, game_engine_player_fired_weapon_hook, _hook_replace); // set weapon fire waypoint
    insert_hook(0xF8E00, 0xF8EEB, game_engine_player_damaged_player_hook, _hook_replace); // set damaged player waypoint
    hook_function(0xCC9D0, 0xAF, update_player_navpoint_data); // sync countdown ticks

    // sync player traits
    insert_hook(0xC9C41, 0xC9C7F, game_engine_update_after_game_update_state_hook3, _hook_replace); // shield vitality traits
    Patch(0xC9C7F, { 0xE9, 0xE0, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90 }).Apply(); // redirect jump to end of loop 0x4C9D64
    insert_hook(0xFA663, 0xFA66A, game_engine_player_rejoined_hook, _hook_execute_replaced_first); // sync all player traits on rejoin
    hook_function(0x11E050, 0x111, game_engine_apply_appearance_traits); // sync appearance traits
    hook_function(0x11DF20, 0xFD, game_engine_apply_movement_traits); // sync movement traits
    hook_function(0x11E170, 0x51, game_engine_apply_sensors_traits); // sync sensor traits
    hook_function(0x11DD50, 0xA8, game_engine_apply_shield_vitality_traits); // sync shield vitality traits
    hook_function(0x11DE00, 0x111, game_engine_apply_weapons_traits); // sync weapon traits
    
    // sync lives remaining
    insert_hook(0xC9D51, 0xC9D58, game_engine_update_after_game_update_state_hook4, _hook_execute_replaced_first);
    insert_hook(0xF917F, 0xF91A7, game_engine_player_killed_hook2, _hook_replace);

    // sync player sitting out
    insert_hook(0xCB1DD, 0xCB1E5, game_engine_update_player_sitting_out_hook, _hook_execute_replaced_first);

    // update everything on player swap
    insert_hook(0xB5544, 0xB554E, game_engine_player_changed_indices_hook1, _hook_execute_replaced_first);
    insert_hook(0xB558C, 0xB5597, game_engine_player_changed_indices_hook2, _hook_execute_replaced_first);

    // update everything on player delete
    insert_hook(0xB5737, 0xB573C, player_delete_hook, _hook_execute_replaced_last); // inlined game_engine_player_deleted
    
    // sync player active
    insert_hook(0xFA5D1, 0xFA5D6, game_engine_player_left_hook, _hook_execute_replaced_first);

    // sync player aiming vectors
    insert_hook(0x11867D, 0x118682, teleporter_teleport_object_hook, _hook_execute_replaced_first);
}