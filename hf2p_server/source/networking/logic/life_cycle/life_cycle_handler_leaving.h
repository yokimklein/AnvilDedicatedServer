#pragma once
#include <cseries\cseries.h>
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

enum e_leaving_flags
{
	_leaving_ready_for_state_transition = 0,

	k_leaving_flags_count
};

enum e_gui_game_mode;
class c_life_cycle_state_handler_leaving : public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	//void initialize(c_life_cycle_state_manager* manager);

protected:
	c_flags<e_leaving_flags, byte, k_leaving_flags_count> m_flags;
	bool m_leave_and_disconnect;
	//e_gui_game_mode m_failure_game_mode;
};
static_assert(sizeof(c_life_cycle_state_handler_leaving) == 0x14);