#include "network_message_type_collection.h"

const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type)
{
    // TODO: original logic using 10th field from c_network_message_type_collection
    const char* name = "<unknown>";
    if (message_type <= k_network_message_type_count)
    {
        switch (message_type)
        {
        case _network_message_type_ping:
            name = "ping";
            break;
        case _network_message_type_pong:
            name = "pong";
            break;
        case _network_message_type_connect_request:
            name = "connect_request";
            break;
        case _network_message_type_connect_refuse:
            name = "connect_refuse";
            break;
        case _network_message_type_connect_establish:
            name = "connect_establish";
            break;
        case _network_message_type_connect_closed:
            name = "connect_closed";
            break;
        case _network_message_type_join_request:
            name = "join_request";
            break;
        case _network_message_type_peer_connect:
            name = "peer_connect";
            break;
        case _network_message_type_join_abort:
            name = "join_abort";
            break;
        case _network_message_type_join_refuse:
            name = "join_refuse";
            break;
        case _network_message_type_leave_session:
            name = "leave_session";
            break;
        case _network_message_type_leave_acknowledge:
            name = "leave_acknowledge";
            break;
        case _network_message_type_session_disband:
            name = "session_disband";
            break;
        case _network_message_type_session_boot:
            name = "session_boot";
            break;
        case _network_message_type_host_decline:
            name = "host_decline";
            break;
        case _network_message_type_peer_establish:
            name = "peer_establish";
            break;
        case _network_message_type_time_synchronize:
            name = "time_synchronize";
            break;
        case _network_message_type_membership_update:
            name = "membership_update";
            break;
        case _network_message_type_peer_properties:
            name = "peer_properties";
            break;
        case _network_message_type_delegate_leadership:
            name = "delegate_leadership";
            break;
        case _network_message_type_boot_machine:
            name = "boot_machine";
            break;
        case _network_message_type_player_add:
            name = "player_add";
            break;
        case _network_message_type_player_refuse:
            name = "player_refuse";
            break;
        case _network_message_type_player_remove:
            name = "player_remove";
            break;
        case _network_message_type_player_properties:
            name = "player_properties";
            break;
        case _network_message_type_parameters_update:
            name = "parameters_update";
            break;
        case _network_message_type_parameters_request:
            name = "parameters_request";
            break;
        case _network_message_type_view_establishment:
            name = "view_establishment";
            break;
        case _network_message_type_player_acknowledge:
            name = "player_acknowledge";
            break;
        case _network_message_type_synchronous_update:
            name = "synchronous_update";
            break;
        case _network_message_type_synchronous_playback_control:
            name = "synchronous_playback_control";
            break;
        case _network_message_type_synchronous_actions:
            name = "synchronous_actions";
            break;
        case _network_message_type_synchronous_acknowledge:
            name = "synchronous_acknowledge";
            break;
        case _network_message_type_synchronous_gamestate:
            name = "synchronous_gamestate";
            break;
        case _network_message_type_synchronous_client_ready:
            name = "synchronous_client_ready";
            break;
        case _network_message_type_game_results:
            name = "game_results";
            break;
        case _network_message_type_test:
            name = "test";
            break;
        }
    }
    return name;
}