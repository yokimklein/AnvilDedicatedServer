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
    hook::function(ADDRESS_GAME_ENGINE_UPDATE_ROUND_CONDITIONS, LENGTH_GAME_ENGINE_UPDATE_ROUND_CONDITIONS, game_engine_update_round_conditions);

    // round timer
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK, ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK_RETURN, game_engine_update_time_hook, _hook_execute_replaced_last);

    // sync game end & podium
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2_RETURN, game_engine_update_after_game_hook2, _hook_execute_replaced_first);

    // sync round index
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1_RETURN, game_engine_update_after_game_update_state_hook1, _hook_execute_replaced_first);

    // sync game engine state
    hook::insert(ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2, ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2_RETURN, game_engine_update_after_game_update_state_hook2, _hook_execute_replaced_first);

    // sync initial teams
    hook::insert(ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1, ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1_RETURN, game_engine_build_initial_teams_hook1, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK, ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK_RETURN, game_engine_build_valid_team_mapping_hook, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK, ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK_RETURN, game_engine_recompute_active_teams_hook, _hook_execute_replaced_first);

    // sync team lives per round
    hook::insert(ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2, ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2_RETURN, game_engine_build_initial_teams_hook2, _hook_execute_replaced_first);
    hook::insert(ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK, ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK_RETURN, game_engine_teams_use_one_shared_life_hook, _hook_execute_replaced_first);

    // ctf defense team
    hook::insert(ADDRESS_C_CTF_ENGINE_GAME_STARTING, ADDRESS_C_CTF_ENGINE_GAME_STARTING_RETURN, c_ctf_engine__game_starting, _hook_execute_replaced_first);

    // ctf helper flags - sudden death
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1_RETURN, c_ctf_engine__get_time_left_in_ticks_hook1, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2_RETURN, c_ctf_engine__get_time_left_in_ticks_hook2, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3_RETURN, c_ctf_engine__get_time_left_in_ticks_hook3, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4_RETURN, c_ctf_engine__get_time_left_in_ticks_hook4, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5_RETURN, c_ctf_engine__get_time_left_in_ticks_hook5, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6, ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6_RETURN, c_ctf_engine__get_time_left_in_ticks_hook6, _hook_execute_replaced_first);
    hook::insert(ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK, ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK_RETURN, c_ctf_engine__initialize_for_new_round_hook, _hook_execute_replaced_first);

    // ctf initial flag reset timers, touch return timers, flag weapon flags
    hook::function(ADDRESS_C_CTF_ENGINE_INITIALIZE_OBJECT_DATA, 0x41, c_ctf_engine__initialize_object_data_hook);
}