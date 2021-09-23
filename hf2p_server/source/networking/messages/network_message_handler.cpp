#include "network_message_handler.h"
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\transport\transport_address.h"
#include <iostream>
#include <windows.h>

int c_network_message_handler::handle_ping(s_transport_address outgoing_address, s_network_message_ping* message_ping)
{
    char* address_string = NULL; // transport_address_get_string(&outgoing_address);
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_ping: ping #%d received from '%s' at local %dms", 
        message_ping->channel_identifier, address_string, timeGetTime());
    
    c_network_message_gateway* message_gateway = this->m_message_gateway;
    return c_network_message_gateway__send_message_directed(message_gateway, outgoing_address, _network_message_type_ping, sizeof(s_network_message_ping), message_ping);
}

int c_network_message_handler::handle_pong(s_transport_address outgoing_address, s_network_message_pong* message)
{
    bool* network_time_locked = (bool*)(module_base + 0x1038344);
    DWORD* g_network_locked_time = (DWORD*)(module_base + 0x1038348);
    DWORD time;

    if (network_time_locked)
        time = *g_network_locked_time; // ms23: 19E8D50 (0x400000 base)
    else
        time = timeGetTime();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_pong: ping #%d returned from '%s' at local %dms (latency %dms)", 
        message->channel_identifier, NULL/*transport_address_get_string(&outgoing_address)*/, timeGetTime(), time - message->next_update_number);
    return 1;
}
/*
int c_network_message_handler::handle_broadcast_search(s_transport_address outgoing_address, s_network_message_broadcast_search* message)
{

}

int c_network_message_handler::handle_broadcast_reply(s_transport_address outgoing_address, s_network_message_broadcast_reply* message)
{

}
*/
int c_network_message_handler::handle_connect_request(s_transport_address outgoing_address, s_network_message_connect_request* message)
{
    // message_handler->m_observer->handle_connect_request(transport_address, message);
    //c_network_observer__handle_connect_request(*(char**)(message_handler + 16), transport_address, (int*)message);
    return 0;
}

int c_network_message_handler::handle_connect_refuse(c_network_channel* outgoing_address, s_network_message_connect_refuse* message)
{
    return 0;
}

int c_network_message_handler::handle_connect_establish(c_network_channel* outgoing_address, s_network_message_connect_establish* message)
{
    return 0;
}

int c_network_message_handler::handle_connect_closed(c_network_channel* outgoing_address, s_network_message_connect_closed* message)
{
    return 0;
}

int c_network_message_handler::handle_join_request(s_transport_address outgoing_address, s_network_message_join_request* message)
{
    return 0;
}

int c_network_message_handler::handle_peer_connect(s_transport_address outgoing_address, s_network_message_peer_connect* message)
{
    return 0;
}

int c_network_message_handler::handle_join_abort(s_transport_address outgoing_address, s_network_message_join_abort* message)
{
    return 0;
}

int c_network_message_handler::handle_join_refuse(s_transport_address outgoing_address, s_network_message_join_refuse* message)
{
    return 0;
}

int c_network_message_handler::handle_leave_session(s_transport_address outgoing_address, s_network_message_leave_session* message)
{
    return 0;
}

int c_network_message_handler::handle_leave_acknowledge(s_transport_address outgoing_address, s_network_message_leave_acknowledge* message)
{
    return 0;
}

int c_network_message_handler::handle_session_disband(s_transport_address outgoing_address, s_network_message_session_disband* message)
{
    return 0;
}

int c_network_message_handler::handle_session_boot(s_transport_address outgoing_address, s_network_message_session_boot* message)
{
    return 0;

}

int c_network_message_handler::handle_time_synchronize(s_transport_address outgoing_address, s_network_message_time_synchronize* message)
{
    return 0;
}

void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message_connect_establish* message)
{
    switch (message_type)
    {
    case _network_message_type_connect_establish:

        break;
    case _network_message_type_leave_session:

        break;
    case _network_message_type_session_disband:

        break;
    case _network_message_type_session_boot:

        break;
    case _network_message_type_host_decline:

        break;
    case _network_message_type_peer_establish:

        break;
    case _network_message_type_membership_update:

        break;
    case _network_message_type_peer_properties:

        break;
    case _network_message_type_delegate_leadership:

        break;
    case _network_message_type_boot_machine:

        break;
    case _network_message_type_player_add:

        break;
        default:
            break;
    }
}


c_network_message_gateway* c_network_message_handler::get_message_gateway()
{
    return this->m_message_gateway;
}