#include "network_session_parameters_game.h"

const char* multiplayer_game_start_error_to_string(e_session_game_start_error start_error)
{
    const char* result;
    if (start_error < _start_error_none || start_error >= k_session_game_start_error_count)
        result = "unknown";
    else
        result = k_game_start_error_strings[start_error];
    return result;
}

const char* multiplayer_game_start_status_to_string(e_session_game_start_status start_status)
{
    const char* result;
    if (start_status < _start_status_none || start_status >= k_start_status_count)
        result = "unknown";
    else
        result = k_game_start_status_strings[start_status];
    return result;
}