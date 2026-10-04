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

void __cdecl player_spawn_hook1(s_hook_registers& registers)
{
    player_datum* player = (player_datum*)registers.ebx;
    datum_index player_index = (datum_index)registers.esi;

    if (player->flags.test(_player_initial_spawn_bit))
    {
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_consumable_supression);
    }
}

void __cdecl player_spawn_hook2(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index player_index = *(datum_index*)(registers.ebp - 0x18);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index player_index = *(datum_index*)(registers.ebp - 0x14);
#endif

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_spawn_timer);
}

void __cdecl player_spawn_hook3(s_hook_registers& registers)
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
    datum_index player_index = *(datum_index*)(registers.ebp - 0x18);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    datum_index player_index = *(datum_index*)(registers.ebp - 0x14);
#endif

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

void __cdecl unit_handle_equipment_energy_cost_hook2(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.ebx;

    simulation_action_game_engine_player_update(unit->unit.player_index, _simulation_player_update_consumable_supression);
}

void __cdecl player_update_loadout_hook1(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.esi;
    player_datum* player = (player_datum*)registers.ebx;

    player_update_loadout(player_index, player);
}

#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
// ms30 added two more player_update_loadout calls which drop the player_index argument, both only reached when the player's loadout index is unset
void __cdecl player_update_loadout_hook3(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;
    player_datum* player = (player_datum*)registers.esi;

    player_update_loadout(player_index, player);
}

void __cdecl player_update_loadout_hook4(s_hook_registers& registers)
{
    unit_datum* unit = (unit_datum*)registers.ebx;
    datum_index player_index = unit->unit.player_index;
    player_datum* player = (player_datum*)registers.esi;

    player_update_loadout(player_index, player);
}
#endif

void __cdecl player_update_loadout_hook2(s_hook_registers& registers)
{
    player_datum* player = (player_datum*)registers.esi;

    datum_index player_index = player_mapping_get_player_by_input_user(_input_user_index0);
    player_update_loadout(player_index, player);
}

void __cdecl player_reset_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;
    player_datum* player = (player_datum*)registers.edi;

    player_update_loadout(player_index, player);
}

void __cdecl game_engine_update_player_netdebug_state_hook(s_hook_registers& registers)
{
    for (long i = 0; i < k_maximum_players; i++)
    {
        simulation_action_game_engine_player_update(i, _simulation_player_update_netdebug);
    }
}

void __cdecl players_update_after_game_hook1(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_blocking_teleporter);
}

void __cdecl players_update_after_game_hook2(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;
    player_datum* player = (player_datum*)registers.esi;

    if (player->equipment_cooldown_ticks > 0)
    {
        player->equipment_cooldown_ticks--;
    }
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_consumable_supression);
}

void __cdecl players_update_after_game_hook3(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;
    player_datum* player = (player_datum*)registers.esi;

    player->vehicle_entrance_ban_ticks--;
    if (player->vehicle_entrance_ban_ticks == 0)
    {
        player->flags.set(_player_vehicle_entrance_ban_bit, false);
    }
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_vehicle_entrance_ban);
}

void __cdecl game_engine_player_killed_hook1(s_hook_registers& registers)
{
    datum_index dead_player_index = *(datum_index*)(registers.ebp + 0x08);

    if (game_is_multiplayer() && game_is_authoritative() && game_engine_in_round())
    {
        simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_last_killer);
    }
}

void __cdecl c_game_statborg__record_player_death_hook1(s_hook_registers& registers)
{
    datum_index dead_player_index = *(datum_index*)(registers.ebp + 0x08);

    simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_grief_player_index);
}

void __cdecl game_engine_player_fired_weapon_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ecx;

    game_engine_set_player_navpoint_action(player_index, _navpoint_action_fired_weapon);
}

void __cdecl game_engine_player_damaged_player_hook(s_hook_registers& registers)
{
    datum_index player_index = *(datum_index*)(registers.ebp + 0x08);

    game_engine_set_player_navpoint_action(player_index, _navpoint_action_player_damaged);
}

void __cdecl game_engine_update_after_game_update_state_hook3(s_hook_registers& registers)
{
    c_player_in_game_iterator player_iterator;
    player_iterator.begin();
    while (player_iterator.next())
    {
        e_shield_multiplier_setting shield_multiplier = current_game_variant()->get_active_variant()->get_map_override_options()->get_base_player_traits()->get_shield_vitality_traits()->get_shield_multiplier_setting();
        player_iterator.get_datum()->multiplayer.player_traits.get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(shield_multiplier, true);
        simulation_action_game_engine_player_update(player_iterator.get_index(), _simulation_player_update_shield_vitality_traits);
    }
}

void __cdecl game_engine_update_after_game_update_state_hook4(s_hook_registers& registers)
{
    c_player_in_game_iterator* player_iterator = (c_player_in_game_iterator*)(registers.ebp - 0x10);

    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_lives_remaining);
}

void __cdecl game_engine_update_player_hook2(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.esi;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_grief_player_index);
}

void __cdecl game_engine_update_player_sitting_out_hook(s_hook_registers& registers)
{
    c_player_in_game_iterator* player_iterator = (c_player_in_game_iterator*)(registers.ebp - 0x10);

    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_sitting_out);
}

void __cdecl game_engine_player_changed_indices_hook1(s_hook_registers& registers)
{
    datum_index player1_index = (datum_index)registers.ebx;
    datum_index player2_index = *(datum_index*)(registers.esp + 0x3390 - 0x337C);

    c_simulation_object_update_flags update_flags;
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player1_index), update_flags);
    simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player2_index), update_flags);
}

void __cdecl game_engine_player_changed_indices_hook2(s_hook_registers& registers)
{
    datum_index player1_index = (datum_index)registers.edi;
    datum_index player2_index = (datum_index)registers.ebx;

    c_simulation_object_update_flags update_flags;
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player1_index), update_flags);
    simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player2_index), update_flags);
}

void __cdecl player_delete_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.edi;

    c_simulation_object_update_flags update_flags;
    update_flags.set_unsafe(MASK(k_simulation_player_update_flag_count));
    simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), update_flags);
}

void __cdecl game_engine_player_killed_hook2(s_hook_registers& registers)
{
    short lives_remaining = (short)registers.ecx;
    player_datum* dead_player = (player_datum*)registers.edx;
    datum_index dead_player_index = (datum_index)registers.edi;

    if (lives_remaining == 0 && game_engine_has_teams() && game_engine_teams_use_one_shared_life(dead_player->configuration.host.team_index))
    {
        dead_player->multiplayer.remaining_lives++;
    }
    simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_lives_remaining);
}

void __cdecl game_engine_player_left_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.ebx;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_active_in_game);
}

void __cdecl game_engine_player_rejoined_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.esi;

    if (game_is_authoritative())
    {
        c_simulation_object_update_flags update_flags;
        update_flags.set(_simulation_player_update_shield_vitality_traits, true);
        update_flags.set(_simulation_player_update_weapon_traits, true);
        update_flags.set(_simulation_player_update_movement_traits, true);
        update_flags.set(_simulation_player_update_appearance_traits, true);
        update_flags.set(_simulation_player_update_sensor_traits, true);
        simulation_action_game_engine_player_update((short)DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), update_flags);
    }
}

void __cdecl game_engine_setup_player_for_respawn_hook(s_hook_registers& registers)
{
    datum_index player_index = *(datum_index*)(registers.esp + 0x90 - 0x84);

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

void __cdecl objective_game_player_forced_base_respawn_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.edi;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_early_respawn);
}

void __cdecl player_killed_player_perform_respawn_on_kill_check_hook(s_hook_registers& registers)
{
    c_player_in_game_iterator* player_iterator = (c_player_in_game_iterator*)registers.ecx;

    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_early_respawn);
}

void __cdecl game_engine_reset_player_respawn_timers_hook(s_hook_registers& registers)
{
    c_player_in_game_iterator* player_iterator = (c_player_in_game_iterator*)registers.ecx;

    simulation_action_game_engine_player_update(player_iterator->get_index(), _simulation_player_update_early_respawn);
}

void __cdecl teleporter_teleport_object_hook(s_hook_registers& registers)
{
    datum_index player_index = (datum_index)registers.esi;

    simulation_action_game_engine_player_update(player_index, _simulation_player_update_control_aiming);
}

void anvil_hooks_player_updates_apply()
{
    // sync equipment charges on spawn
    hook::insert(ADDRESS_PLAYER_SPAWN_HOOK1, ADDRESS_PLAYER_SPAWN_HOOK1_RETURN, player_spawn_hook1, _hook_execute_replaced_first);

    // sync equipment cooldown reset
    //hook::insert(0x42D3ED, 0x42D3F2, unit_handle_equipment_energy_cost_hook2, _hook_execute_replaced_first); // sets cooldown after use (No longer required, function was rewritten)
    hook::insert(ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2, ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2_RETURN, players_update_after_game_hook2, _hook_replace); // updates tick countdown

    // sync spawn timer
    hook::insert(ADDRESS_PLAYER_SPAWN_HOOK2, ADDRESS_PLAYER_SPAWN_HOOK2_RETURN, player_spawn_hook2, _hook_execute_replaced_first);
    hook::function(ADDRESS_GAME_ENGINE_PLAYER_SET_SPAWN_TIMER, 0x62, game_engine_player_set_spawn_timer);

    // sync early respawn
    hook::insert(ADDRESS_PLAYER_SPAWN_HOOK3, ADDRESS_PLAYER_SPAWN_HOOK3_RETURN, player_spawn_hook3, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK, ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK_RETURN, game_engine_setup_player_for_respawn_hook, _hook_execute_replaced_first);
    hook::insert(ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK, ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK_RETURN, objective_game_player_forced_base_respawn_hook, _hook_execute_replaced_first);
    hook::insert(ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK, ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK_RETURN, player_killed_player_perform_respawn_on_kill_check_hook, _hook_execute_replaced_first); // inlined game_engine_reset_player_respawn_timer
    hook::insert(ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK, ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK_RETURN, game_engine_reset_player_respawn_timers_hook, _hook_execute_replaced_first);

    // update spectating player
    hook::function(ADDRESS_C_SIMULATION_PLAYER_RESPAWN_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT, 0x80, c_simulation_player_respawn_request_event_definition__apply_game_event);

    // sync loadout index
    hook::insert(ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1, ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1_RETURN, player_update_loadout_hook1, _hook_replace); // add player_index argument back to call in player_spawn
    hook::insert(ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2, ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2_RETURN, player_update_loadout_hook2, _hook_replace); // add player_index argument back to call in equipment_add
#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
    hook::insert(ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK3, ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK3_RETURN, player_update_loadout_hook3, _hook_replace); // add player_index argument back to call in sub_717360 (hf2p player loop)
    hook::insert(ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK4, ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK4_RETURN, player_update_loadout_hook4, _hook_replace); // add player_index argument back to call in sub_4587C0 (unit baseline state)
#endif
    hook::function(ADDRESS_PLAYER_UPDATE_LOADOUT, 0x40, player_update_loadout);
    hook::insert(ADDRESS_PLAYER_RESET_HOOK, ADDRESS_PLAYER_RESET_HOOK_RETURN, player_reset_hook, _hook_replace); // replace inlined player_update_loadout, added new since ms23

    // sync player netdebug data
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK, ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK_RETURN, game_engine_update_player_netdebug_state_hook, _hook_execute_replaced_last);

    // sync player active in game flag
    hook::function(ADDRESS_SIMULATION_QUEUE_PLAYER_EVENT_APPLY_SET_ACTIVATION, 0x68, simulation_queue_player_event_apply_set_activation);

    // sync spectating player after boot
    hook::function(ADDRESS_GAME_ENGINE_BOOT_PLAYER_SAFE, 0x43, game_engine_boot_player_safe); // UNTESTED!!

    // sync player booting
    hook::function(ADDRESS_GAME_ENGINE_BOOT_PLAYER, 0xF5, game_engine_boot_player);

    // sync vehicle entrance ban after hijack
    hook::function(ADDRESS_PLAYER_NOTIFY_VEHICLE_EJECTION_FINISHED, 0x79, player_notify_vehicle_ejection_finished); // set ban time
    hook::insert(ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3, ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3_RETURN, players_update_after_game_hook3, _hook_replace); // countdown ban ticks

    // sync telefrag
    hook::insert(ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1, ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1_RETURN, players_update_after_game_hook1, _hook_execute_replaced_last); // countdown blocking ticks

    // sync revenge
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1, ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1_RETURN, game_engine_player_killed_hook1, _hook_execute_replaced_first); // inlined player_set_revenge_shield_boost

    // sync betrayal grief
    hook::insert(ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1, ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1_RETURN, c_game_statborg__record_player_death_hook1, _hook_execute_replaced_first); // inlined game_engine_respond_to_betrayal
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2, ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2_RETURN, game_engine_update_player_hook2, _hook_execute_replaced_first);

    // sync waypoint actions
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK, ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK_RETURN, game_engine_player_fired_weapon_hook, _hook_replace); // set weapon fire waypoint
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK, ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK_RETURN, game_engine_player_damaged_player_hook, _hook_replace); // set damaged player waypoint
    hook::function(ADDRESS_UPDATE_PLAYER_NAVPOINT_DATA, 0xAF, update_player_navpoint_data); // sync countdown ticks

    // sync player traits
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3_RETURN, game_engine_update_after_game_update_state_hook3, _hook_replace); // shield vitality traits
    patch::bytes(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_PATCH, { 0xE9, 0xE0, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90 }); // redirect jump to end of loop 0x4C9D64
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK, ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK_RETURN, game_engine_player_rejoined_hook, _hook_execute_replaced_first); // sync all player traits on rejoin
    hook::function(ADDRESS_GAME_ENGINE_APPLY_APPEARANCE_TRAITS, 0x111, game_engine_apply_appearance_traits); // sync appearance traits
    hook::function(ADDRESS_GAME_ENGINE_APPLY_MOVEMENT_TRAITS, 0xFD, game_engine_apply_movement_traits); // sync movement traits
    hook::function(ADDRESS_GAME_ENGINE_APPLY_SENSORS_TRAITS, 0x51, game_engine_apply_sensors_traits); // sync sensor traits
    hook::function(ADDRESS_GAME_ENGINE_APPLY_SHIELD_VITALITY_TRAITS, 0xA8, game_engine_apply_shield_vitality_traits); // sync shield vitality traits
    hook::function(ADDRESS_GAME_ENGINE_APPLY_WEAPONS_TRAITS, 0x111, game_engine_apply_weapons_traits); // sync weapon traits
    
    // sync lives remaining
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4_RETURN, game_engine_update_after_game_update_state_hook4, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2, ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2_RETURN, game_engine_player_killed_hook2, _hook_replace);

    // sync player sitting out
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK, ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK_RETURN, game_engine_update_player_sitting_out_hook, _hook_execute_replaced_first);

    // update everything on player swap
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1, ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1_RETURN, game_engine_player_changed_indices_hook1, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2, ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2_RETURN, game_engine_player_changed_indices_hook2, _hook_execute_replaced_first);

    // update everything on player delete
    hook::insert(ADDRESS_PLAYER_DELETE_HOOK, ADDRESS_PLAYER_DELETE_HOOK_RETURN, player_delete_hook, _hook_execute_replaced_last); // inlined game_engine_player_deleted
    
    // sync player active
    hook::insert(ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK, ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK_RETURN, game_engine_player_left_hook, _hook_execute_replaced_first);

    // sync player aiming vectors
    hook::insert(ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK, ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK_RETURN, teleporter_teleport_object_hook, _hook_execute_replaced_first);
}