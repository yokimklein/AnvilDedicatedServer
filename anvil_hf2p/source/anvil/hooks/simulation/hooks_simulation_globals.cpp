#include "hooks_simulation_globals.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <simulation\game_interface\simulation_game_engine_globals.h>

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
}