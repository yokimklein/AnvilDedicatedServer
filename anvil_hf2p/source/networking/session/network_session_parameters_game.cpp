#include "network_session_parameters_game.h"
#include <iostream>
#include <anvil\server_tools.h>
#include <cseries\cseries_events.h>

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
    if (!VALID_INDEX(start_error, k_session_game_start_error_count))
    {
        return "unknown";
    }
    else
    {
        return k_game_start_error_strings[start_error];
    }
}

const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status)
{
    if (!VALID_INDEX(start_status, k_session_game_start_status_count))
    {
        return "unknown";
    }
    else
    {
        return k_game_start_status_strings[start_status];
    }
}

s_network_session_parameter_game_start_status* c_network_session_parameter_game_start_status::get()
{
    if (get_allowed())
    {
        return &m_data;
    }
    event(_event_warning, "networking:session_parameters:game_start_status: [%s] failed to get, unavailable", get_session_description());
    return NULL;
}

bool c_network_session_parameter_game_start_status::set(s_network_session_parameter_game_start_status* start_status)
{
    if (set_allowed())
    {
        if (start_status->game_start_status != m_data.game_start_status ||
            start_status->game_start_error != m_data.game_start_error ||
            start_status->player_error_mask != m_data.player_error_mask ||
            start_status->map_load_progress != m_data.map_load_progress ||
            !get_allowed())
        {
            m_data = *start_status;
            set_update_required();
        }
        anvil_log_game_start_status(start_status);
        return true;
    }
    else
    {
        event(_event_warning, "networking:session_parameters:game_start_status: [%s] type %d [%s] failed to get [%s]",
            get_session_description(),
            m_parameter_type,
            m_parameter_type_description,
            get_set_denied_reason());
        return false;
    }
}

bool c_network_session_parameter_countdown_timer::set(e_network_game_countdown_delayed_reason delayed_reason, long countdown_timer)
{
    if (set_allowed())
    {
        m_data.delayed_reason = delayed_reason;
        m_data.countdown_timer = countdown_timer;
        set_update_required(); // $TODO: set_internal method call, this gets compiled out to a redundant duplicate call
        if (!get_allowed())
        {
            set_update_required();
        }
        return true;
    }
    else
    {
        event(_event_error, "networking:session_parameters:countdown_timer: [%s] can't set [%s]", get_session_description(), get_set_denied_reason());
        return false;
    }
}

long c_network_session_parameter_countdown_timer::get_countdown_timer()
{
    if (get_allowed() && m_data.delayed_reason != _network_game_countdown_delayed_reason_none)
    {
        return m_data.countdown_timer;
    }
    else
    {
        return 0;
    }
}

e_network_game_countdown_delayed_reason c_network_session_parameter_countdown_timer::get_delayed_reason()
{
    if (get_allowed())
    {
        return m_data.delayed_reason;
    }
    else
    {
        return _network_game_countdown_delayed_reason_none;
    }
}