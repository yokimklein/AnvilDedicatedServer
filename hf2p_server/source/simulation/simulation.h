#pragma once

constexpr long k_network_maximum_machines_per_session = 17; // 16 in h3 debug
constexpr long k_network_maximum_players_per_session = 16;

enum e_network_game_simulation_protocol
{
	_network_game_simulation_local = 0,

	_network_game_simulation_synchronous,
	_network_game_simulation_distributed,

	k_network_game_simulation_count
};

enum e_game_simulation_type
{
    _game_simulation_none = 0,
    _game_simulation_local,

    _game_simulation_synchronous_client, // predicted
    _game_simulation_synchronous_server, // authoritative

    _game_simulation_distributed_client, // predicted
    _game_simulation_distributed_server, // authoritative

    k_game_simulation_count
};