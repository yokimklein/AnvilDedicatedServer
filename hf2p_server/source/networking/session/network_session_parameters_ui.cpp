#include "network_session_parameters_ui.h"
#include "..\..\dllmain.h"

bool c_network_session_parameter_ui_game_mode::request_change(e_gui_game_mode gui_gamemode)
{
    FUNCTION_DEF(0x3B4D0, bool, __thiscall, request_change, c_network_session_parameter_ui_game_mode* thisptr, e_gui_game_mode gui_gamemode);
    return request_change(this, gui_gamemode);
}