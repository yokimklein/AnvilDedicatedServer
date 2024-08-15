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
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_equipment_cooldown);
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

    simulation_action_game_engine_player_update(unit->unit.player_index, _simulation_player_update_equipment_cooldown);
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
    player_datum* player_data;
    __asm mov player_data, esi;

    datum_index player_index = player_mapping_get_player_by_input_user(_input_user_index0);
    player_update_loadout(player_index, player_data);
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
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_telefrag);
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
    simulation_action_game_engine_player_update(player_index, _simulation_player_update_equipment_cooldown);
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
        simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_revenge);
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
    simulation_action_game_engine_player_update(dead_player_index, _simulation_player_update_grief);
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

    // update spectating player
    hook_function(0x68B40, 0x80, c_simulation_player_respawn_request_event_definition__apply_game_event);

    // sync character type
    insert_hook(0xBAF22, 0xBAF29, player_update_loadout_hook1, _hook_replace); // add player_index argument back to call in player_spawn
    insert_hook(0xE05A8, 0xE05AF, player_update_loadout_hook2, _hook_replace); // add player_index argument back to call in equipment_add
    hook_function(0xE0660, 0x40, player_update_loadout);

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

}