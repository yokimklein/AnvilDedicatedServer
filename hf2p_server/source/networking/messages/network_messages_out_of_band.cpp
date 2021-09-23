#include "network_message_handler.h"
#include "network_message_type_collection.h"
#include "..\delivery\network_channel.h"
#include <iostream>

int c_network_message_handler::handle_out_of_band_message(s_transport_address transport_address, e_network_message_type message_type, long message_storage_size, s_network_message* message)
{
    int retcode = 0;
    c_network_channel* channel;
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s received out-of-band from '%s'", message_type, "<unknown>", (char*)0); // TODO: unknown print args

    switch (message_type)
    {
        case _network_message_type_ping:
            retcode = this->handle_ping(transport_address, (s_network_message_ping*)message);
            break;
        case _network_message_type_pong:
            retcode = this->handle_pong(transport_address, (s_network_message_pong*)message);
            break;
        /*
        case _network_message_type_broadcast_search:
            retcode = this->handle_broadcast_search(transport_address, (s_network_message_broadcast_search*)message);
            break;
        case _network_message_type_broadcast_reply:
            retcode = this->handle_broadcast_reply(transport_address, (s_network_message_broadcast_reply*)message);
            break;
        */
        case _network_message_type_connect_request:
            retcode = this->handle_connect_request(transport_address, (s_network_message_connect_request*)message);
            break;
        case _network_message_type_connect_refuse:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(transport_address); // get_associated_channel is sub_43BA40 in ms23
            if (channel)
                retcode = this->handle_connect_refuse(channel, (s_network_message_connect_refuse*)message);
            break;
        case _network_message_type_connect_establish:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(transport_address);
            if (channel)
                retcode = this->handle_connect_establish(channel, (s_network_message_connect_establish*)message);
            break;
        case _network_message_type_connect_closed:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(transport_address);
            if (channel)
                this->handle_connect_closed(channel, (s_network_message_connect_closed*)message);
            break;
        case _network_message_type_join_request:
            retcode = this->handle_join_request(transport_address, (s_network_message_join_request*)message);
            break;
        case _network_message_type_peer_connect:
            retcode = this->handle_peer_connect(transport_address, (s_network_message_peer_connect*)message);
            break;
        case _network_message_type_join_abort:
            retcode = this->handle_join_abort(transport_address, (s_network_message_join_abort*)message);
            break;
        case _network_message_type_join_refuse:
            retcode = this->handle_join_refuse(transport_address, (s_network_message_join_refuse*)message);
            break;
        case _network_message_type_leave_session:
            retcode = this->handle_leave_session(transport_address, (s_network_message_leave_session*)message);
            break;
        case _network_message_type_leave_acknowledge:
            retcode = this->handle_leave_acknowledge(transport_address, (s_network_message_leave_acknowledge*)message);
            break;
        case _network_message_type_session_disband:
            retcode = this->handle_session_disband(transport_address, (s_network_message_session_disband*)message);
            break;
        case _network_message_type_session_boot:
            retcode = this->handle_session_boot(transport_address, (s_network_message_session_boot*)message);
            break;
        case _network_message_type_time_synchronize:
            retcode = this->handle_time_synchronize(transport_address, (s_network_message_time_synchronize*)message);
            break;
        default:
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s from '%s' cannot be handled out-of-band, discarding", message_type, "<unknown>", (char*)0); // TODO: print unknowns
            break;
    }
    return retcode;
}