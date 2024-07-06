#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

struct c_life_cycle_state_handler_leaving : c_life_cycle_state_handler
{
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	//void initialize(c_life_cycle_state_manager* manager);

	// TODO: 4 bytes worth of data here was removed since ms23
	//byte_flags m_flags;
	//bool m_disconnected;
	//byte __data[0x6];

	byte unknown_temp[0x4];
};
static_assert(sizeof(c_life_cycle_state_handler_leaving) == 0x14);