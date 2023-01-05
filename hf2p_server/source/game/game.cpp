#include "game.h"
#include "assert.h"
#include "game_globals.h"

game_options* game_options_get()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

bool game_is_server()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation_type;
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
		return true;

	return false;
}

bool game_is_distributed()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation_type;
	if (game_simulation >= _game_simulation_distributed_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

bool game_is_playback()
{
	return game_playback_get();
}

e_game_playback_type game_playback_get()
{
	return game_options_get()->game_playback;
}

bool game_is_available()
{
	game_globals_storage* game_globals = game_globals_get();
	return game_globals != nullptr && game_globals->map_active && game_globals->options.game_mode != _game_mode_none;
}