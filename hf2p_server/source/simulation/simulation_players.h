#pragma once
#include <cseries\cseries.h>
#include <game\players.h>
#include <game\player_control.h>
#include <game\player_configuration.h>
#include <game\game_engine_simulation.h>

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
	long m_current_action_time;
	player_action m_action;
};
static_assert(sizeof(c_simulation_player) == 0xB0);

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
	s_player_collection_player players[k_maximum_players];
};
static_assert(sizeof(s_player_collection) == 0xB908);