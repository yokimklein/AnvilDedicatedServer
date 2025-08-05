#pragma once
#include <cseries\cseries.h>
#include <networking\logic\life_cycle\life_cycle_state_handler.h>
#include <interface\gui_pregame_setup_manager.h>

enum e_joining_flags
{
	_joining_can_exit_bit = 0,

	k_joining_flags_count,
};

struct c_life_cycle_state_handler_joining : public c_life_cycle_state_handler
{
public:
	// TODO: 4 bytes worth of data removed from here since ms23, verify these fields are correct
	c_flags<e_joining_flags, byte, k_joining_flags_count> m_flags;
	e_gui_game_mode m_failure_game_mode;
	e_life_cycle_state m_previous_state;
	//byte __data[0x4];
};
static_assert(sizeof(c_life_cycle_state_handler_joining) == 0x1C);
