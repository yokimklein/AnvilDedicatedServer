#include "network_session_parameters_ui.h"

bool c_network_session_parameter_ui_game_mode::request_change(e_gui_game_mode gui_gamemode)
{
    return DECLFUNC(ADDRESS_C_NETWORK_SESSION_PARAMETER_UI_GAME_MODE_REQUEST_CHANGE, bool, __thiscall, c_network_session_parameter_ui_game_mode*, e_gui_game_mode)(this, gui_gamemode);
}