#include "network_session_parameters_game.h"
#include <iostream>
#include <anvil\server_tools.h>

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

bool c_network_session_parameter_game_start_status::set(s_network_session_parameter_game_start_status* start_status)
{
    if (this->set_allowed())
    {
        if (start_status->game_start_status != m_data.game_start_status ||
            start_status->game_start_error != m_data.game_start_error ||
            start_status->player_error_mask != m_data.player_error_mask ||
            start_status->map_load_progress != m_data.map_load_progress ||
            !this->get_allowed())
        {
            this->m_data = *start_status;
            this->set_update_required();
        }
        anvil_log_game_start_status(start_status);
        return true;
    }
    else
    {
        printf("MP/NET/SESSION,PARAMS: c_network_session_parameter_game_start_status::set: [%s] type %d [%s] failed to get [%s]\n",
            this->get_session_description(), m_type, m_name, this->get_set_denied_reason());
        return false;
    }
}

bool c_network_session_parameter_countdown_timer::set(e_network_game_countdown_delayed_reason delayed_reason, long countdown_timer)
{
    if (this->set_allowed())
    {
        this->m_data.delayed_reason = delayed_reason;
        this->m_data.countdown_timer = countdown_timer;
        this->set_update_required(); // TODO: set_internal method call, this gets compiled out to a redundant duplicate call
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
    if (this->get_allowed() && this->m_data.delayed_reason != _network_game_countdown_delayed_reason_none)
        return this->m_data.countdown_timer;
    else
        return 0;
}

e_network_game_countdown_delayed_reason c_network_session_parameter_countdown_timer::get_delayed_reason()
{
    if (this->get_allowed())
        return this->m_data.delayed_reason;
    else
        return _network_game_countdown_delayed_reason_none;
}