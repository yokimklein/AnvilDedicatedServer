#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

struct c_life_cycle_state_handler_pre_game : c_life_cycle_state_handler
{
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	//void initialize(c_life_cycle_state_manager* manager);

	// TODO: 4 bytes worth of data was removed here since ms23
	//dword m_time;
	//long m_countdown_timer;
	//byte __data[0x8];

	byte m_unknown_data[0xC];
};
static_assert(sizeof(c_life_cycle_state_handler_pre_game) == 0x1C);