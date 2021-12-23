#include <iostream>
#include <windows.h>
#include "network_message_handler.h"
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\transport\transport_address.h"

int c_network_message_handler::handle_ping(s_transport_address outgoing_address, s_network_message_ping* message_ping)
{
    char* address_string = NULL; // transport_address_get_string(&outgoing_address);
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_ping: ping #%d received from '%s' at local %dms", 
        message_ping->channel_identifier, address_string, timeGetTime());
    
    c_network_message_gateway* message_gateway = this->m_message_gateway;
    return message_gateway->send_message_directed(message_gateway, outgoing_address, _network_message_type_ping, sizeof(s_network_message_ping), message_ping);
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
        message->channel_identifier, "(null)"/*transport_address_get_string(&outgoing_address)*/, timeGetTime(), time - message->next_update_number);
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
    typedef int(__fastcall* handle_connect_establish_ptr)(c_network_channel* outgoing_address, s_network_message_connect_establish* message);
    auto handle_connect_establish = reinterpret_cast<handle_connect_establish_ptr>(module_base + 0xD5B10);

    return handle_connect_establish(outgoing_address, message);
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
    //auto session = c_network_session_manager::get_session(this->m_session_manager, &message);
    //if (session)
    //    return c_network_session::handle_session_disband(session, &outgoing_address, &message);
    // TODO - include additional logging strings
    return 0;
}

int c_network_message_handler::handle_session_boot(s_transport_address outgoing_address, s_network_message_session_boot* message)
{
    return 0;
}

int c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline* message)
{
    return 0;
}

int c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish* message)
{
    return 0;
}

int c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update* message)
{
    return 0;
}

int c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties* message)
{
    return 0;
}

int c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership* message)
{
    return 0;
}

int c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine* message)
{
    return 0;
}

int c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add* message)
{
    return 0;
}

int c_network_message_handler::handle_time_synchronize(s_transport_address outgoing_address, s_network_message_time_synchronize* message)
{
    return 0;
}

void log_received_over_closed_channel(c_network_channel* channel, e_network_message_type message_type)
{
    char* message_type_name = channel->get_message_type_name(message_type);
    char* channel_name = channel->get_name();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over CLOSED channel '%s'",
        message_type, message_type_name, channel_name);
}

void log_received_over_non_connected_channel(c_network_channel* channel, e_network_message_type message_type)
{
    char* message_type_name = channel->get_message_type_name(message_type);
    char* channel_name = channel->get_name();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over a non-connected channel '%s', discarding",
        message_type, message_type_name, channel_name);
}

// might need a return code?
void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message* message)
{
    s_transport_address remote_address;
    switch (message_type)
    {
        case _network_message_type_connect_establish:
            this->handle_connect_establish(channel, (s_network_message_connect_establish*)message);
            break;

        case _network_message_type_leave_session: // TODO - removed by saber in client builds
            if (channel->connected() && channel->get_remote_address(&remote_address))
                this->handle_leave_session(remote_address, (s_network_message_leave_session*)message);
            else
                log_received_over_closed_channel(channel, _network_message_type_leave_session);
            break;

        case _network_message_type_session_disband:
            if (channel->connected() && channel->get_remote_address(&remote_address))
                this->handle_session_disband(remote_address, (s_network_message_session_disband*)message);
            else
                log_received_over_closed_channel(channel, _network_message_type_session_disband);
            break;

        case _network_message_type_session_boot:
            if (channel->connected() && channel->get_remote_address(&remote_address))
                this->handle_session_boot(remote_address, (s_network_message_session_boot*)message);
            else
                log_received_over_closed_channel(channel, _network_message_type_session_boot);
            break;

        case _network_message_type_host_decline:
            if (channel->connected())
                this->handle_host_decline(channel, (s_network_message_host_decline*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_host_decline);
            break;

        case _network_message_type_peer_establish:
            if (channel->connected())
                this->handle_peer_establish(channel, (s_network_message_peer_establish*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_peer_establish);
            break;

        case _network_message_type_membership_update:
            if (channel->connected())
                this->handle_membership_update(channel, (s_network_message_membership_update*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_membership_update);
            break;

        case _network_message_type_peer_properties:
            if (channel->connected())
                this->handle_peer_properties(channel, (s_network_message_peer_properties*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_peer_properties);
            break;

        case _network_message_type_delegate_leadership:
            if (channel->connected())
                this->handle_delegate_leadership(channel, (s_network_message_delegate_leadership*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_delegate_leadership);
            break;

        case _network_message_type_boot_machine:
            if (channel->connected())
                this->handle_boot_machine(channel, (s_network_message_boot_machine*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_boot_machine);
            break;

        case _network_message_type_player_add:
            if (channel->connected())
                this->handle_player_add(channel, (s_network_message_player_add*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_add);
            break;

        default:

            break;
    }
}

c_network_message_gateway* c_network_message_handler::get_message_gateway()
{
    return this->m_message_gateway;
}