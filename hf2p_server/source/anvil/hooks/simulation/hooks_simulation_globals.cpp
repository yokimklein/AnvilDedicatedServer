#include "hooks_simulation_globals.h"
#include <anvil\hooks\hooks.h>
#include <game\game_engine.h>
#include <simulation\game_interface\simulation_game_engine_globals.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall game_engine_update_time_hook()
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_timer);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_hook2()
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_game_finished);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_update_state_hook1()
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_index);
}

__declspec(safebuffers) void __fastcall game_engine_update_after_game_update_state_hook2()
{
    simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_engine_state);
}
#pragma runtime_checks("", restore)

void anvil_hooks_simulation_globals_apply()
{
    // pre-game camera countdown
    hook_function(0xC6C00, 0x111, game_engine_update_round_conditions);

    // round timer
    insert_hook(0xC98CB, 0xC98D1, game_engine_update_time_hook, _hook_execute_replaced_last);

    // sync game end & podium
    insert_hook(0xCA265, 0xCA26C, game_engine_update_after_game_hook2, _hook_execute_replaced_first);

    // sync round index
    insert_hook(0xC9C2A, 0xC9C31, game_engine_update_after_game_update_state_hook1, _hook_execute_replaced_first);

    // sync game engine state
    insert_hook(0xC9D9B, 0xC9DA2, game_engine_update_after_game_update_state_hook2, _hook_execute_replaced_first);
}