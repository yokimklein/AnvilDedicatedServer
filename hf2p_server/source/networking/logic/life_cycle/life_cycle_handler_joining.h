#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>
#include <interface\gui_pregame_setup_manager.h>

struct c_life_cycle_state_handler_joining : c_life_cycle_state_handler
{
	// TODO: 4 bytes worth of data removed from here since ms23
	//byte m_flags;
	//e_gui_game_mode m_game_mode;
	//e_life_cycle_state m_enter_state;
	//byte __data[0x4];

	byte m_unknown_data[0xC];
};
static_assert(sizeof(c_life_cycle_state_handler_joining) == 0x1C);