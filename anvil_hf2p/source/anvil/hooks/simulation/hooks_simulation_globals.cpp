#include "hooks_simulation_globals.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <simulation\game_interface\simulation_game_engine_globals.h>
#include <simulation\game_interface\simulation_game_engine_ctf.h>

void __cdecl game_engine_update_time_hook(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_timer);
}

void __cdecl game_engine_update_after_game_hook2(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_game_finished);
}

void __cdecl game_engine_update_after_game_update_state_hook1(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_index);
}

void __cdecl game_engine_update_after_game_update_state_hook2(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_engine_state);
}

void __cdecl game_engine_build_initial_teams_hook1(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_initial_teams);
}

void __cdecl game_engine_build_initial_teams_hook2(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_team_lives_per_round);
}

void __cdecl game_engine_build_valid_team_mapping_hook(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_initial_teams);
}

void __cdecl game_engine_recompute_active_teams_hook(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_initial_teams);
}

void __cdecl game_engine_teams_use_one_shared_life_hook(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_team_lives_per_round);
}

void __cdecl c_ctf_engine__game_starting(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_defensive_team);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook1(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook2(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook3(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook4(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook5(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__get_time_left_in_ticks_hook6(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __cdecl c_ctf_engine__initialize_for_new_round_hook(s_hook_registers& registers)
{
    simulation_action_game_engine_globals_update(_simulation_ctf_engine_globals_update_helper_flags);
}

void __fastcall c_ctf_engine__initialize_object_data_hook(c_ctf_engine* thisptr, void* unused, long index)
{
    thisptr->initialize_object_data_(index);
}

void anvil_hooks_simulation_globals_apply()
{
    // pre-game camera countdown
    hook::function(0xC6C00, 0x111, game_engine_update_round_conditions);

    // round timer
    hook::insert(0xC98CB, 0xC98D1, game_engine_update_time_hook, _hook_execute_replaced_last);

    // sync game end & podium
    hook::insert(0xCA265, 0xCA26C, game_engine_update_after_game_hook2, _hook_execute_replaced_first);

    // sync round index
    hook::insert(0xC9C2A, 0xC9C31, game_engine_update_after_game_update_state_hook1, _hook_execute_replaced_first);

    // sync game engine state
    hook::insert(0xC9D9B, 0xC9DA2, game_engine_update_after_game_update_state_hook2, _hook_execute_replaced_first);

    // sync initial teams
    hook::insert(0xDC9F2, 0xDC9F9, game_engine_build_initial_teams_hook1, _hook_execute_replaced_first);
    hook::insert(0xDC51D, 0xDC522, game_engine_build_valid_team_mapping_hook, _hook_execute_replaced_first);
    hook::insert(0xDC661, 0xDC666, game_engine_recompute_active_teams_hook, _hook_execute_replaced_first);

    // sync team lives per round
    hook::insert(0xDCA19, 0xDCA1F, game_engine_build_initial_teams_hook2, _hook_execute_replaced_first);
    hook::insert(0xDC847, 0xDC84C, game_engine_teams_use_one_shared_life_hook, _hook_execute_replaced_first);

    // ctf defense team
    hook::insert(0x22852F, 0x228535, c_ctf_engine__game_starting, _hook_execute_replaced_first);

    // ctf helper flags - sudden death
    hook::insert(0x229A03, 0x229A0A, c_ctf_engine__get_time_left_in_ticks_hook1, _hook_execute_replaced_first);
    hook::insert(0x229A2A, 0x229A31, c_ctf_engine__get_time_left_in_ticks_hook2, _hook_execute_replaced_first);
    hook::insert(0x229A5B, 0x229A62, c_ctf_engine__get_time_left_in_ticks_hook3, _hook_execute_replaced_first);
    hook::insert(0x229A7B, 0x229A82, c_ctf_engine__get_time_left_in_ticks_hook4, _hook_execute_replaced_first);
    hook::insert(0x229992, 0x22999A, c_ctf_engine__get_time_left_in_ticks_hook5, _hook_execute_replaced_first);
    hook::insert(0x229ADB, 0x229AE2, c_ctf_engine__get_time_left_in_ticks_hook6, _hook_execute_replaced_first);
    hook::insert(0x228379, 0x228383, c_ctf_engine__initialize_for_new_round_hook, _hook_execute_replaced_first);

    // ctf initial flag reset timers, touch return timers, flag weapon flags
    hook::function(0x228220, 0x41, c_ctf_engine__initialize_object_data_hook);
}