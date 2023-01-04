#pragma once
#include "..\cseries\cseries.h"
#include "..\game\players.h"
#include "..\game\player_control.h"
#include "..\game\player_configuration.h"

#pragma pack(push, 1)
class c_simulation_world;
class c_simulation_player
{
	long m_player_index;
	long m_player_datum_index;
	long m_player_type;
	s_player_identifier m_player_identifier;
	s_machine_identifier m_player_machine_identifier;
	c_simulation_world* m_world;
	bool m_pending_deletion;
	bool m_active;
	byte __data2A[0x2];
	long __unknown2C;
	player_action m_action;
};
static_assert(sizeof(c_simulation_player) == 0xB0);
#pragma pack(pop)

struct s_player_collection_player
{
	s_player_identifier player_identifier;
	bool left_game;
	long left_game_time;
	s_machine_identifier machine_identifier;
	s_player_configuration configuration;
};
static_assert(sizeof(s_player_collection_player) == 0xB90);

struct s_player_collection
{
	long valid_player_mask;
	long player_count;
	s_player_collection_player players[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_player_collection) == 0xB908);