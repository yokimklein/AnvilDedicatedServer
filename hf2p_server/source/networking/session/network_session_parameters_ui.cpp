#include "network_session_parameters_ui.h"

bool c_network_session_parameter_ui_game_mode::request_change(e_gui_game_mode gui_gamemode)
{
    return DECLFUNC(0x3B4D0, bool, __thiscall, c_network_session_parameter_ui_game_mode*, e_gui_game_mode)(this, gui_gamemode);
}