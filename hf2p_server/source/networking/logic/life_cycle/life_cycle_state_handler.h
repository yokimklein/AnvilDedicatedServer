#pragma once
#include <cseries\cseries.h>
#include <networking\logic\life_cycle\life_cycle_manager.h>

enum e_life_cycle_state_transition_type
{
	_life_cycle_state_transition_type_unknown0 = 0,
	_life_cycle_state_transition_type_unknown1,
	_life_cycle_state_transition_type_unknown2,
};

enum e_life_cycle_state_handler_flags
{
	_life_cycle_state_handler_unknown_bit0 = 0,
	_life_cycle_state_handler_unknown_bit1,
	_life_cycle_state_handler_allows_group_session_bit,
	_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit,
	_life_cycle_state_handler_group_session_disconnect_recreates_group_bit,
	_life_cycle_state_handler_unknown_bit5,

	k_life_cycle_state_handler_flags
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

	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_state;
	c_life_cycle_state_manager* m_manager;

	// Removed since h3/ms23
	//c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> m_handler_flags;
	//c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> m_required_squad_session_parameter_mask;
	//c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> m_required_group_session_parameter_mask;
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x10);