#include "hooks_player_updates.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <hf2p\loadouts.h>
#include <game\player_mapping.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall player_spawn_hook1()
{
    player_datum* player_data;
    datum_index player_index;
    __asm mov player_data, ebx;
    __asm mov player_index, esi;

    if (TEST_BIT(player_data->flags, _player_initial_spawn_bit))
    {
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_equipment_charges);
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
#pragma runtime_checks("", restore)

void anvil_hooks_player_updates_apply()
{
    // player_spawn
    insert_hook(0xBB093, 0xBB098, player_spawn_hook1, _hook_execute_replaced_first); // equipment charges on spawn
    insert_hook(0xBB435, 0xBB43B, player_spawn_hook2, _hook_execute_replaced_first); // spawn timer
    insert_hook(0xBB459, 0xBB460, player_spawn_hook3, _hook_execute_replaced_first); // early respawn

    // update spawn timer
    hook_function(0xC7700, 0x62, game_engine_player_set_spawn_timer);

    // i'm not sure what this request is used for - spectator related
    hook_function(0x68B40, 0x80, c_simulation_player_respawn_request_event_definition__apply_game_event);

    // player_update_loadout
    insert_hook(0xBAF22, 0xBAF29, player_update_loadout_hook1, _hook_replace); // add player_index argument back to call in player_spawn
    insert_hook(0xE05A8, 0xE05AF, player_update_loadout_hook2, _hook_replace); // add player_index argument back to call in equipment_add
    hook_function(0xE0660, 0x40, player_update_loadout); // sync character type
}