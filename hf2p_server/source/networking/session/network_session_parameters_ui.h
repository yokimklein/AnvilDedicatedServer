#pragma once
#include <networking\session\network_session_parameters_base.h>
#include <networking\session\network_session_parameters_generic.h>
#include <cseries\cseries.h>
#include <interface\gui_pregame_setup_manager.h>

enum e_network_ui_state_flags
{
	_network_ui_state_flag_game_setup_changing_bit = 0,

	k_network_ui_state_flag_count
};

struct s_network_ui_state
{
	c_flags<e_network_ui_state_flags, word, k_network_ui_state_flag_count> flags;
};
static_assert(sizeof(s_network_ui_state) == 0x2);

class c_network_session_parameter_ui_game_mode : public c_network_session_parameter_base, c_generic_network_session_parameter_data<e_gui_game_mode>
{
public:
	bool request_change(e_gui_game_mode gui_gamemode);
};
static_assert(sizeof(c_network_session_parameter_ui_game_mode) == 0x38);