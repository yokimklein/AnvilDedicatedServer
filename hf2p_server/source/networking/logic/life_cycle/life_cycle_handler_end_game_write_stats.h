#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

struct c_life_cycle_state_handler_end_game_write_stats : c_life_cycle_state_handler
{
	long __time28;
	word_flags m_flags;
};
static_assert(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x18);