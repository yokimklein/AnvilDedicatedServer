#pragma once
#include <cseries\cseries.h>
#include <networking\logic\life_cycle\life_cycle_manager.h>

enum e_life_cycle_state_transition_type
{
	_life_cycle_state_transition_unhandled = 0,
	_life_cycle_state_transition_unchanged,
	_life_cycle_state_transition_change_requested,

	k_life_cycle_state_transition_type_count
};

enum e_life_cycle_state_handler_flags
{
	_life_cycle_state_handler_requires_squad_session_bit = 0,
	_life_cycle_state_handler_requires_group_session_bit,
	_life_cycle_state_handler_allows_group_session_bit,
	_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit,
	_life_cycle_state_handler_group_session_disconnect_recreates_group_bit,
	_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit,

	k_life_cycle_state_handler_flags_count
};

class c_life_cycle_state_manager;
enum e_network_session_type;
class c_life_cycle_state_handler
{
public:
	virtual void update();
	virtual e_life_cycle_state_transition_type update_for_state_transition();
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data);
	virtual void exit(c_life_cycle_state_handler* handler);
	virtual char const* get_state_string();
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type);

protected:
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_state;
	c_life_cycle_state_manager* m_manager;

	// Removed since h3/ms23
	//c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> m_handler_flags;
	//c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> m_required_squad_session_parameter_mask;
	//c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> m_required_group_session_parameter_mask;
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x10);