#include "hooks_statborg.h"
#include <anvil\hooks\hooks.h>
#include <anvil\server_tools.h>
#include <Patch.hpp>
#include <game\game_engine.h>
#include <simulation\game_interface\simulation_game_action.h>

// runtime checks need to be disabled for these, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall game_engine_update_after_game_hook()
{
    simulation_action_game_statborg_update(_simulation_statborg_update_finalize_for_game_end);
}

__declspec(safebuffers) void __fastcall c_game_statborg__adjust_player_stat_hook()
{
    long absolute_player_index; __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook1()
{
    // I have to move this into eax first otherwise it'll refuse to compile for some reason
    long absolute_player_index; __asm mov eax, [esp + 0x90] __asm mov absolute_player_index, eax; // originally dword ptr [esp + 0x50 - 0x24], sp was 0x50
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook2()
{
    long absolute_player_index; __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_earn_wp_event_hook()
{
    long absolute_player_index;
    __asm mov absolute_player_index, esi;
    simulation_action_game_statborg_update(absolute_player_index);
}

__declspec(safebuffers) void __fastcall game_engine_end_round_with_winner_hook3()
{
    long team_index; __asm mov team_index, ebx;
    simulation_action_game_statborg_update(_simulation_statborg_update_team0 + team_index);
}

__declspec(safebuffers) void __fastcall c_game_engine__recompute_team_score_hook()
{
    long team_index; __asm mov team_index, edi;
    simulation_action_game_statborg_update(_simulation_statborg_update_team0 + team_index);
}

__declspec(safebuffers) void __fastcall player_changed_teams_hook()
{
    short player_index; __asm mov player_index, bx;
    simulation_action_game_statborg_update(_simulation_statborg_update_player0 + player_index);
}

__declspec(safebuffers) void __fastcall player_indices_swapped_hook()
{
    long absolute_index_a; __asm mov absolute_index_a, edi;
    long absolute_index_b; __asm mov absolute_index_b, esi;
    simulation_action_game_statborg_update(absolute_index_a);
    simulation_action_game_statborg_update(absolute_index_b);
}
#pragma runtime_checks("", restore)

void __fastcall adjust_team_stat_hook(c_game_statborg* thisptr, void* unused, e_game_team team_index, long statistic, short unknown, long value)
{
    thisptr->adjust_team_stat(team_index, statistic, unknown, value);
}

void __fastcall stats_reset_for_round_switch_hook(c_game_statborg* thisptr)
{
    thisptr->stats_reset_for_round_switch();
}

void anvil_hooks_statborg_apply()
{
    // add back simulation_action_game_statborg_update & simulation_action_game_engine_player_update calls
    Hook(0xFA2D0, game_engine_player_added).Apply();
    // c_game_statborg::stats_finalize_for_game_end
    insert_hook(0xCA2F0, 0xCA2F7, game_engine_update_after_game_hook, _hook_execute_replaced_first);
    // c_game_statborg::adjust_player_stat
    insert_hook(0x1AF61D, 0x1AF624, c_game_statborg__adjust_player_stat_hook);
    insert_hook(0xC8ACE, 0xC8AF3, game_engine_end_round_with_winner_hook1);
    insert_hook(0xC8C18, 0xC8C3D, game_engine_end_round_with_winner_hook2);
    insert_hook(0xFAFDB, 0xFB000, game_engine_earn_wp_event_hook);
    // TODO: other inlined instances of c_game_statborg::adjust_player_stat
    //c_game_statborg::record_kill // this call from ms23 is gone entirely in ms29, not even inlined
    //game_engine_adjust_player_wp // survival only
    //metagame_earn_wp_event_maybe // survival only
    //sub_9DBC70 // infection related, c_infection_engine::update which calls this is empty in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_left
    // c_game_statborg::adjust_team_stat
    Hook(0x1AF710, adjust_team_stat_hook).Apply();
    insert_hook(0xC8A5F, 0xC8A66, game_engine_end_round_with_winner_hook3);
    insert_hook(0x1C7FC4, 0x1C7FD2, c_game_engine__recompute_team_score_hook, _hook_execute_replaced_last);
    // TODO: c_territories_engine::unknown has several c_game_statborg::adjust_team_stat calls
    // c_game_statborg::player_changed_teams
    insert_hook(0xFA956, 0xFA95F, player_changed_teams_hook);
    // game_engine_player_indices_swapped > c_game_statborg::player_indices_swapped (inlined)
    insert_hook(0xFA7B1, 0xFA7B8, player_indices_swapped_hook); // TODO: TEST - WHERE IS THIS USED?
    // c_game_statborg::stats_reset_for_round_switch
    Hook(0x1AEE00, stats_reset_for_round_switch_hook).Apply();
    // TODO: c_game_statborg::reset_player_stat for infection
    // TODO: c_game_statborg::reset_team_stat for territories
}