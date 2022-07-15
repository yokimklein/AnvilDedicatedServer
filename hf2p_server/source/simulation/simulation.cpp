#include "simulation.h"

/*
e_game_simulation_type game_simulation_get()
{
    return game_options_get()->game_simulation;
}

bool game_is_distributed()
{
    e_game_simulation game_simulation = game_simulation_get();
    if game_simulation >= _game_simulation_distributed_client && game_simulation <= _game_simulation_distributed_server)
    return true;

    return false;
}

bool game_is_predicted()
{
    return game_simulation_get() == _game_simulation_distributed_client;
}

bool game_is_authoritative
{
    return !game_is_predicted();
}

bool game_is_server()
{
    e_game_simulation game_simulation = game_simulation_get();
    if (game_simulation >= _game_simulation_synchronous_client && game_simulation <= _game_simulation_distributed_client)
        return false;

    return true;
}

bool game_is_networked()
{
    e_game_simulation game_simulation = game_simulation_get();

    if (game_simulation >= _game_simulation_synchronous_client && game_simulation <= _game_simulation_distributed_server)
        return true;

    return false;
}

bool game_is_synchronous_networking()
{
    e_game_simulation game_simulation = game_simulation_get();
    if game_simulation >= _game_simulation_synchronous_client && game_simulation <= _game_simulation_synchronous_server)
    return true;

    return false;
}
game_options* game_options_get()
{
    // game_globals tls
    ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

    return &game_globals->options;
}
*/