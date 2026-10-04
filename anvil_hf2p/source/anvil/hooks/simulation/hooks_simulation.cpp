#include "hooks_simulation.h"
#include <anvil\hooks\hooks.h>
#include <simulation\simulation_world.h>
#include <main\main.h>
#include <game\game_engine.h>

// allow view establishment to progress past the connection phase
void __fastcall update_establishing_view_hook(c_simulation_world* simulation_world, void* unused, c_simulation_view* simulation_view)
{
    simulation_world->update_establishing_view(simulation_view);
}

long __cdecl internal_halt_render_thread_and_lock_resources_hook(const char* file_name, long line_number)
{
    long result = _internal_halt_render_thread_and_lock_resources(file_name, line_number);

    game_engine_attach_to_simulation();
    return result;
}

void anvil_hooks_simulation_apply()
{
    // allow view_establishment to progress past connection phase to established in update_establishing_view again
    hook::function(ADDRESS_UPDATE_ESTABLISHING_VIEW, 0x12D, update_establishing_view_hook);

    // add game_engine_attach_to_simulation back to game_engine_game_starting
    hook::call(ADDRESS_INTERNAL_HALT_RENDER_THREAD_AND_LOCK_RESOURCES_CALL, internal_halt_render_thread_and_lock_resources_hook);

    // add game_engine_detach_from_simulation_gracefully back to game_engine_game_ending
    hook::insert(ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY, ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY_RETURN, game_engine_detach_from_simulation_gracefully, _hook_replace);
}