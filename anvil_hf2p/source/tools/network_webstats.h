#pragma once

#include <cseries\cseries.h>

struct s_game_matchmaking_options
{
	word hopper_identifier;
	byte xlast_index;
	bool is_ranked;
	bool team_game;
	byte : 8;
	wchar_t hopper_name[32];
	byte : 8;
	byte : 8;
	long draw_probability;
	long beta;
	long tau;
	long experience_base_increment;
	long experience_penalty_decrement;
};
static_assert(sizeof(s_game_matchmaking_options) == 0x5C);