#include "game.h"
#include "assert.h"
#include "game_globals.h"
#include "game_engine_util.h"
#include "game_engine_team.h"

game_options* game_options_get()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

e_game_simulation_type game_simulation_get()
{
	return game_options_get()->game_simulation_type;
}

bool game_is_server()
{
	e_game_simulation_type game_simulation = game_simulation_get();
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
		return true;

	return false;
}

bool game_is_distributed()
{
	e_game_simulation_type game_simulation = game_simulation_get();
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

bool game_is_predicted()
{
	return game_simulation_get() == _game_simulation_distributed_client;
}

bool __cdecl game_is_authoritative()
{
	return !game_is_predicted();
}

bool game_engine_has_teams()
{
	if (game_engine_running())
	{
		c_game_variant* game_variant = current_game_variant();
		if (game_engine_variant_has_teams(game_variant))
			return true;
	}
	return false;
}

bool game_in_progress()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals != nullptr && game_globals->game_in_progress && !game_globals->initializing)
		return game_globals->map_active;
	else
		return false;
}