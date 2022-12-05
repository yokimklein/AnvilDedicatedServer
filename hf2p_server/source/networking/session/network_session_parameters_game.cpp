#include "network_session_parameters_game.h"
#include <iostream>

char const* k_game_start_status_strings[k_start_status_count] = {
    "none",
    "not pre game",
    "not leader",
    "join in progress",
    "loading",
    "ready",
    "countdown",
    "countdown unknown",
    "error"
};

// update this whenever the enum updates
char const* k_game_start_error_strings[k_session_game_start_error_count] = {
    "none",
    "no map selected",
    "map load failure",
    "map load precaching",
    "invalid film selected",
    "no film selected",
    "too many teams",
    "too many for local coop",
    "too many for net coop",
    "incompatible for net coop",
    "theater too many players",
    "theater leader must be host",
    "theater all not compatible",
    "too many players in forge"
};

const char* multiplayer_game_start_error_to_string(e_session_game_start_error start_error)
{
    if (start_error < _start_error_none || start_error >= k_session_game_start_error_count)
        return "unknown";
    else
        return k_game_start_error_strings[start_error];
}

const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status)
{
    if (start_status < _start_status_none || start_status >= k_start_status_count)
        return "unknown";
    else
        return k_game_start_status_strings[start_status];
}

bool c_network_session_parameter_ui_game_mode::request_change(e_gui_game_mode gui_gamemode)
{
    bool(__thiscall* request_change)(c_network_session_parameter_ui_game_mode* thisptr, e_gui_game_mode gui_gamemode) = reinterpret_cast<decltype(request_change)>(module_base + 0x3B4D0);
    return request_change(this, gui_gamemode);
}

s_network_session_parameter_game_start_status* c_network_session_parameter_game_start_status::get()
{
    if (this->get_allowed())
        return &this->m_data;
    printf("MP/NET/SESSION,PARAMS: c_network_session_parameter_game_start_status::get: [%s] failed to get, unavailable\n",
        this->get_session_description());
    return nullptr;
}