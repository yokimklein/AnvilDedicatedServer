#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

enum e_life_cycle_state_handler_in_game_flags
{
	_life_cycle_state_handler_in_game_map_load = 0,
	_life_cycle_state_handler_in_game_core_load_pending,

	k_life_cycle_state_handler_in_game_flags_count
};
typedef c_flags<e_life_cycle_state_handler_in_game_flags, byte, k_life_cycle_state_handler_in_game_flags_count> c_life_cycle_state_handler_in_game_flags;

class c_life_cycle_state_handler_in_game : public c_life_cycle_state_handler
{
public:

protected:
	c_life_cycle_state_handler_in_game_flags m_flags;
	bool m_handled_simulation_abort;
	long m_time;
	ulong m_simulation_aborted_timestamp;
	ulong64 m_game_instance;
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x28);