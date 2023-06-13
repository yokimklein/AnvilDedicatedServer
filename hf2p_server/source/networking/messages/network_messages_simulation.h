#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <simulation\simulation_view.h>
#include <simulation\simulation.h>

struct s_network_message_view_establishment : s_network_message
{
	e_simulation_view_establishment_mode establishment_mode;
	long establishment_identifier;
	bool signature_exists;
	long signature_size;
	byte signature_data[0x3C]; // used size is 8 * signature_size
};
static_assert(sizeof(s_network_message_view_establishment) == 0x4C);

struct s_network_message_player_acknowledge : s_network_message
{
	ulong player_valid_mask;
	ulong player_in_game_mask;
	s_player_identifier player_identifiers[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_network_message_player_acknowledge) == 0x88);