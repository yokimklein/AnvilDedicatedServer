#include "game_engine.h"
#include "..\simulation\simulation.h"
#include "..\simulation\game_interface\simulation_game_action.h"
#include "..\memory\tls.h"

void game_engine_attach_to_simulation()
{
	simulation_action_game_engine_globals_create();
	simulation_action_game_statborg_create();
	simulation_action_game_ai_create();
	if (game_engine_is_sandbox())
		simulation_action_game_map_variant_create_all();
	for (short i = 0; i < k_maximum_multiplayer_players; i++)
		simulation_action_game_engine_player_create(i);
	simulation_action_breakable_surfaces_create();
}

bool game_engine_is_sandbox()
{
	// TODO
	return false;
}

c_game_engine* current_game_engine()
{
	auto tls = get_tls();
	auto game_engine_globals = tls->game_engine_globals;
	if (game_engine_globals->game_engine_index < _game_engine_base_variant || game_engine_globals->game_engine_index >= k_game_engine_type_count)
		return nullptr;
	else
		return game_engines[game_engine_globals->game_engine_index.get()];
}

bool game_engine_running()
{
	return current_game_engine() != nullptr;
}