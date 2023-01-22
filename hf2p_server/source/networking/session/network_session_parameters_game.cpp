#include "network_session_parameters_game.h"
#include <iostream>

char const* k_game_start_status_strings[k_session_game_start_status_count] = {
    "none",
    "not pre game",
    "changing settings member",
    "join in progress",
    "loading",
    "ready leader",
    "voting",
    "countdown",
    "error"
};

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
    if (start_error < _session_game_start_error_none || start_error >= k_session_game_start_error_count)
        return "unknown";
    else
        return k_game_start_error_strings[start_error];
}

const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status)
{
    if (start_status < _session_game_start_status_none || start_status >= k_session_game_start_status_count)
        return "unknown";
    else
        return k_game_start_status_strings[start_status];
}

s_network_session_parameter_game_start_status* c_network_session_parameter_game_start_status::get()
{
    if (this->get_allowed())
        return &this->m_data;
    printf("MP/NET/SESSION,PARAMS: c_network_session_parameter_game_start_status::get: [%s] failed to get, unavailable\n",
        this->get_session_description());
    return nullptr;
}

bool c_network_session_parameter_countdown_timer::set(e_countdown_type countdown_type, long countdown_timer)
{
    if (this->set_allowed())
    {
        this->m_data.countdown_type = countdown_type;
        this->m_data.countdown_timer = countdown_timer;
        this->set_update_required();
        if (!this->get_allowed())
            this->set_update_required();
        return true;
    }
    else
    {
        printf("MP/NET/SESSION,PARAMS: %s: [%s] can't set [%s]\n", __FUNCTION__, this->get_session_description(), this->get_set_denied_reason());
        return false;
    }
}

long c_network_session_parameter_countdown_timer::get_countdown_timer()
{
    if (this->get_allowed())
        return this->m_data.countdown_timer;
    else
        return -1;
}

e_countdown_type c_network_session_parameter_countdown_timer::get_countdown_type()
{
    if (this->get_allowed())
        return this->m_data.countdown_type;
    else
        return _countdown_type_none;
}