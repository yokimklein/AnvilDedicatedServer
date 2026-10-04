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
    hook::function(ADDRESS_GAME_ENGINE_PLAYER_ADDED, 0x1F3, game_engine_player_added);

    // c_game_statborg::stats_finalize_for_game_end
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK_RETURN, game_engine_update_after_game_hook, _hook_execute_replaced_first);

    // c_game_statborg::adjust_player_stat
    hook::insert(ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK, ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK_RETURN, c_game_statborg__adjust_player_stat_hook, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1, ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1_RETURN, game_engine_end_round_with_winner_hook1, _hook_execute_replaced_last); // teams
    hook::insert(ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2, ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2_RETURN, game_engine_end_round_with_winner_hook2, _hook_execute_replaced_last); // ffa
    hook::insert(ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK, ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK_RETURN, game_engine_earn_wp_event_hook, _hook_execute_replaced_last);

    // c_game_statborg::adjust_team_stat
    hook::function(ADDRESS_ADJUST_TEAM_STAT, 0x61, adjust_team_stat_hook);

    hook::insert(ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3, ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3_RETURN, game_engine_end_round_with_winner_hook3, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK, ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK_RETURN, c_game_engine__recompute_team_score_hook, _hook_execute_replaced_last);

    // c_game_statborg::player_changed_teams
    hook::insert(ADDRESS_PLAYER_CHANGED_TEAMS_HOOK, ADDRESS_PLAYER_CHANGED_TEAMS_HOOK_RETURN, player_changed_teams_hook, _hook_execute_replaced_first);

    // game_engine_player_indices_swapped > c_game_statborg::player_indices_swapped (inlined)
    hook::function(ADDRESS_GAME_ENGINE_PLAYER_INDICES_SWAPPED, 0x7F, game_engine_player_indices_swapped); // add back inlined c_game_statborg::player_indices_swapped

    // c_game_statborg::stats_reset_for_round_switch
    hook::function(ADDRESS_STATS_RESET_FOR_ROUND_SWITCH, 0x14E, stats_reset_for_round_switch_hook);

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