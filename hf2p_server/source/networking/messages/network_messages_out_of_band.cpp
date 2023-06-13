#include <networking\messages\network_message_handler.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\session\network_observer.h>
#include <networking\delivery\network_channel.h>
#include <networking\delivery\network_link.h>
#include <networking\messages\network_message_gateway.h>
#include <networking\transport\transport_address.h>
#include <iostream>

void c_network_message_handler::handle_out_of_band_message(s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    c_network_channel* channel;
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s received out-of-band from '%s'\n",
        message_type,
        this->m_message_type_collection->get_message_type_name(message_type),
        transport_address_get_string(address));
    
    switch (message_type)
    {
        case _network_message_type_ping:
            this->handle_ping(address, (s_network_message_ping*)message);
            break;
        case _network_message_type_pong:
            this->handle_pong(address, (s_network_message_pong*)message);
            break;
        case _network_message_type_connect_request:
            this->m_observer->handle_connect_request(address, (s_network_message_connect_request*)message);
            break;
        case _network_message_type_connect_refuse:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(address);
            if (channel)
                this->handle_connect_refuse(channel, (s_network_message_connect_refuse*)message);
            break;
        case _network_message_type_connect_establish:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(address);
            if (channel)
                this->handle_connect_establish(channel, (s_network_message_connect_establish*)message);
            break;
        case _network_message_type_connect_closed:
            channel = this->get_message_gateway()->get_network_link()->get_associated_channel(address);
            if (channel)
                this->handle_connect_closed(channel, (s_network_message_connect_closed*)message);
            break;
        case _network_message_type_join_request:
            this->handle_join_request(address, (s_network_message_join_request*)message);
            break;
        case _network_message_type_peer_connect:
            this->handle_peer_connect(address, (s_network_message_peer_connect*)message);
            break;
        case _network_message_type_join_abort:
            this->handle_join_abort(address, (s_network_message_join_abort*)message);
            break;
        case _network_message_type_join_refuse:
            this->handle_join_refuse(address, (s_network_message_join_refuse*)message);
            break;
        case _network_message_type_leave_session:
            this->handle_leave_session(address, (s_network_message_leave_session*)message);
            break;
        case _network_message_type_leave_acknowledge:
            this->handle_leave_acknowledge(address, (s_network_message_leave_acknowledge*)message);
            break;
        case _network_message_type_session_disband:
            this->handle_session_disband(address, (s_network_message_session_disband*)message);
            break;
        case _network_message_type_session_boot:
            this->handle_session_boot(address, (s_network_message_session_boot*)message);
            break;
        case _network_message_type_time_synchronize:
            this->handle_time_synchronize(address, (s_network_message_time_synchronize*)message);
            break;
        default:
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s from '%s' cannot be handled out-of-band, discarding\n",
                message_type,
                this->m_message_type_collection->get_message_type_name(message_type),
                transport_address_get_string(address));
            break;
    }
}