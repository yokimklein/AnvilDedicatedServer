#include "hooks_statborg.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <simulation\game_interface\simulation_game_statborg.h>

void __cdecl game_engine_update_after_game_hook(s_hook_registers& registers)
{
    simulation_action_game_statborg_update(_simulation_statborg_update_finalize_for_game_end);
}

void __cdecl c_game_statborg__adjust_player_stat_hook(s_hook_registers& registers)
{
    long absolute_player_index = (long)registers.esi;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)absolute_player_index);
}

void __cdecl game_engine_end_round_with_winner_hook1(s_hook_registers& registers)
{
    long absolute_player_index = (long)registers.esi;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)absolute_player_index);
}

void __cdecl game_engine_end_round_with_winner_hook2(s_hook_registers& registers)
{
    long absolute_player_index = (long)registers.esi;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)absolute_player_index);
}

void __cdecl game_engine_earn_wp_event_hook(s_hook_registers& registers)
{
    long absolute_player_index = (long)registers.esi;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)absolute_player_index);
}

void __cdecl game_engine_end_round_with_winner_hook3(s_hook_registers& registers)
{
    long team_index = (long)registers.ebx;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)(_simulation_statborg_update_team0 + team_index));
}

void __cdecl c_game_engine__recompute_team_score_hook(s_hook_registers& registers)
{
    long team_index = (long)registers.edi;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)(_simulation_statborg_update_team0 + team_index));
}

void __cdecl player_changed_teams_hook(s_hook_registers& registers)
{
    short player_index = (short)registers.ebx;

    simulation_action_game_statborg_update((e_simulation_statborg_update_flag)(_simulation_statborg_update_player0 + player_index));
}

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
    hook::function(0xFA2D0, 0x1F3, game_engine_player_added);

    // c_game_statborg::stats_finalize_for_game_end
    hook::insert(0xCA2F0, 0xCA2F7, game_engine_update_after_game_hook, _hook_execute_replaced_first);

    // c_game_statborg::adjust_player_stat
    hook::insert(0x1AF61D, 0x1AF624, c_game_statborg__adjust_player_stat_hook, _hook_execute_replaced_first);
    hook::insert(0xC8AF3, 0xC8AFA, game_engine_end_round_with_winner_hook1, _hook_execute_replaced_last); // teams
    hook::insert(0xC8C3D, 0xC8C44, game_engine_end_round_with_winner_hook2, _hook_execute_replaced_last); // ffa
    hook::insert(0xFB000, 0xFB007, game_engine_earn_wp_event_hook, _hook_execute_replaced_last);

    // c_game_statborg::adjust_team_stat
    hook::function(0x1AF710, 0x61, adjust_team_stat_hook);

    hook::insert(0xC8A5F, 0xC8A66, game_engine_end_round_with_winner_hook3, _hook_execute_replaced_first);
    hook::insert(0x1C7FC4, 0x1C7FD2, c_game_engine__recompute_team_score_hook, _hook_execute_replaced_last);

    // c_game_statborg::player_changed_teams
    hook::insert(0xFA956, 0xFA95F, player_changed_teams_hook, _hook_execute_replaced_first);

    // game_engine_player_indices_swapped > c_game_statborg::player_indices_swapped (inlined)
    hook::function(0xFA740, 0x7F, game_engine_player_indices_swapped); // add back inlined c_game_statborg::player_indices_swapped

    // c_game_statborg::stats_reset_for_round_switch
    hook::function(0x1AEE00, 0x14E, stats_reset_for_round_switch_hook);

    // TODO: other inlined instances of c_game_statborg::adjust_player_stat
    //c_game_statborg::record_kill // this call from ms23 is gone entirely in ms29, not even inlined
    //game_engine_adjust_player_wp // survival only
    //metagame_earn_wp_event_maybe // survival only
    //sub_9DBC70 // infection related, c_infection_engine::update which calls this is empty in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_killed_player // also gone in ms29
    //c_infection_engine::player_left
    // TODO: c_territories_engine::unknown has several c_game_statborg::adjust_team_stat calls
    // TODO: c_game_statborg::reset_player_stat for infection
    // TODO: c_game_statborg::reset_team_stat for territories
}