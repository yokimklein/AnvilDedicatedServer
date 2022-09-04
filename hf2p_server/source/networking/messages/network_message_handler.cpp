#include <iostream>
#include <windows.h>
#include "network_message_handler.h"
#include "network_message_type_collection.h"
#include "network_message_gateway.h"
#include "..\delivery\network_link.h"
#include "..\session\network_session_manager.h"
#include "..\session\network_session.h"
#include "..\transport\transport_address.h"
#include "..\network_globals.h"
#include "..\network_utilities.h"

void c_network_message_handler::handle_ping(s_transport_address const* outgoing_address, s_network_message_ping const* message)
{
    const char* address_string = transport_address_get_string(outgoing_address);
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_ping: ping #%d received from '%s' at local %dms\n", message->id, address_string, timeGetTime());
    s_network_message_pong* response = new s_network_message_pong();
    response->id = message->id;
    response->timestamp = message->timestamp;
    response->request_qos = message->request_qos;
    this->m_message_gateway->send_message_directed(outgoing_address, _network_message_type_pong, sizeof(s_network_message_pong), response);

    delete response;
}

void c_network_message_handler::handle_pong(s_transport_address const* outgoing_address, s_network_message_pong const* message)
{
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_pong: ping #%d returned from '%s' at local %dms (latency %dms)\n", 
        message->id, transport_address_get_string(outgoing_address), timeGetTime(), network_get_time() - message->timestamp);
}

void c_network_message_handler::handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message)
{
    typedef long(__fastcall* handle_connect_refuse_ptr)(c_network_channel* channel, s_network_message_connect_refuse const* message);
    auto handle_connect_refuse = reinterpret_cast<handle_connect_refuse_ptr>(module_base + 0x25AC0);
    handle_connect_refuse(channel, message);
}

void c_network_message_handler::handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message)
{
    if (channel->closed())
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: ignoring connect establish from '%s'/%d (currently closed)\n",
            channel->get_name(),
            message->remote_identifier);
        return;
    }
    if (channel->get_identifier() != message->identifier)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: ignoring connect establish from '%s'/%d (establishment identifier %d != local identifier %d)\n",
            channel->get_name(),
            message->remote_identifier,
            message->identifier,
            channel->get_identifier());
        return;
    }
    if (channel->established() && channel->get_remote_identifier() != message->remote_identifier)
    {
        long channel_identifier = -1;
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d but we are already established to %d\n",
            channel->get_name(),
            message->remote_identifier,
            channel->get_remote_identifier());
        if (!channel->m_message_queue.has_channel_been_used())
            channel_identifier = channel->get_identifier();
        s_transport_address* remote_address = nullptr;
        channel->get_remote_address(remote_address);
        channel->close(_network_channel_reason_connect_reinitiate);
        channel->open(remote_address, false, channel_identifier);
    }
    if (channel->closed())
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d for local %d but the channel closed before we could establish\n",
            channel->get_name(),
            message->remote_identifier,
            channel->get_identifier());
        return;
    }
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d for local %d\n",
        channel->get_name(),
        message->remote_identifier,
        channel->get_identifier());
    channel->send_connection_established(message->remote_identifier);
}

void c_network_message_handler::handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message)
{
    if (channel->closed())
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' ignoring remote closure (already closed)\n",
            channel->get_name());
        return;
    }
    if (channel->get_identifier() != message->identifier)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' ignoring remote closure (closed identifier %d != identifier %d)\n",
            channel->get_name(),
            message->identifier,
            channel->get_identifier());
        return;
    }
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' remotely closed (reason #%d: '%s')\n",
        channel->get_name(),
        message->closure_reason,
        channel->get_closure_reason_string(message->closure_reason));
    channel->close(_network_channel_reason_remote_closure);
}

void c_network_message_handler::handle_join_request(s_transport_address const* outgoing_address, s_network_message_join_request const* message)
{
    if (message->protocol_version == 9)
    {
        c_network_session* session = this->m_session_manager->get_session(&message->session_id);
        if (session)
        {
            if (session->m_local_state == _network_session_state_host_established || session->m_local_state == _network_session_state_host_disband)
            {
                session->handle_join_request(outgoing_address, message);
                return;
            }
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: received message with incorrect protocol version [%d!=%d]\n", message->protocol_version, 9);
    }

    // if you get this error, it's likely caused by the client trying to connect with the wrong secure address/id, usually because the API server hasn't updated with the new server information
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: can't handle join-request for '%s' from '%s'\n",
        transport_secure_identifier_get_string(&message->session_id),
        transport_address_get_string(outgoing_address));
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: failed to handle incoming join request\n");
}

void c_network_message_handler::handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message)
{
    if (message->protocol_version == 9)
    {
        auto* session = this->m_session_manager->get_session(&message->session_id);
        if (session)
        {
            session->handle_peer_connect(outgoing_address, message);
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_connect: no session, ignoring peer connect from '%s'\n",
                transport_address_get_string(outgoing_address));
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_connect: received message with incorrect protocol version [%d!=%d]\n",
            message->protocol_version,
            9);
    }
}

void c_network_message_handler::handle_join_abort(s_transport_address const* outgoing_address, s_network_message_join_abort const* message)
{
    auto session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->established() && session->is_host())
    {
        e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
        if (session->join_abort(outgoing_address, message->join_nonce))
        {
            refuse_reason = _network_join_refuse_reason_aborted;
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: ignoring unknown abort from '%s'\n",
                transport_address_get_string(outgoing_address));
            refuse_reason = _network_join_refuse_reason_abort_ignored;
        }
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: received abort, sending join-refusal (%s) to '%s'\n",
            network_message_join_refuse_get_reason_string(refuse_reason),
            transport_address_get_string(outgoing_address));
        auto message_gateway = this->get_message_gateway();
        s_network_message_join_refuse* refuse_message = new s_network_message_join_refuse();
        refuse_message->session_id = message->session_id;
        refuse_message->reason = refuse_reason;
        message_gateway->send_message_directed(outgoing_address, _network_message_type_join_refuse, sizeof(s_network_message_join_refuse), refuse_message);

        delete refuse_message;
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: no session, ignoring join abort from '%s'\n",
            transport_address_get_string(outgoing_address));
    }
}

void c_network_message_handler::handle_join_refuse(s_transport_address const* outgoing_address, s_network_message_join_refuse const* message)
{
    typedef void(__thiscall* handle_join_refuse_ptr)(c_network_message_handler* message_handler, s_transport_address const* outgoing_address, s_network_message_join_refuse const* message);
    auto handle_join_refuse = reinterpret_cast<handle_join_refuse_ptr>(module_base + 0x25660);
    handle_join_refuse(this, outgoing_address, message);
}

// TODO
void c_network_message_handler::handle_leave_session(s_transport_address const* outgoing_address, s_network_message_leave_session const* message)
{
    
}

void c_network_message_handler::handle_leave_acknowledge(s_transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message)
{
    typedef void(__thiscall* handle_leave_acknowledge_ptr)(c_network_message_handler* message_handler, s_transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message);
    auto handle_leave_acknowledge = reinterpret_cast<handle_leave_acknowledge_ptr>(module_base + 0x256E0);
    return handle_leave_acknowledge(this, outgoing_address, message);
}

void c_network_message_handler::handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
    {
        if (!session->handle_session_disband(outgoing_address, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_disband: session (%s) failed to handle session-disband from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                transport_address_get_string(outgoing_address));
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_disband: ignoring message from '%s' (no %s session)\n",
            transport_address_get_string(outgoing_address),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

void c_network_message_handler::handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
    {
        if (!session->handle_session_boot(outgoing_address, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_boot: session (%s) failed to handle session-boot from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                transport_address_get_string(outgoing_address));
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_boot: ignoring session-boot from '%s' (no %s session)\n",
            transport_address_get_string(outgoing_address),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

void c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
    {
        if (!session->handle_host_decline(channel, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_host_decline: session failed to handle host-decline (%s)\n",
                transport_secure_identifier_get_string(&message->session_id));
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_host_decline: channel '%s' ignoring host-decline (%s) (can not find session)\n",
            channel->get_name(),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

void c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (!session || !session->handle_peer_establish(channel, message))
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_establish: channel '%s' failed to handle peer-establish (%s)\n",
            channel->get_name(),
            transport_secure_identifier_get_string(&message->session_id));
    
        s_network_message_host_decline* decline_message = new s_network_message_host_decline();
        memset(decline_message, 0, sizeof(s_network_message_host_decline));
        decline_message->session_id = message->session_id;
        channel->send_message(_network_message_type_host_decline, sizeof(s_network_message_host_decline), decline_message);
        
        delete decline_message;
    }
}

void c_network_message_handler::handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
    {
        if (!session->handle_time_synchronize(outgoing_address, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_time_synchronize: session failed to handle time-synchronize (%s %d) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                message->synchronization_stage,
                transport_address_get_string(outgoing_address));
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_time_synchronize: session doesn't exist to handle time-synchronize (%s %d) from '%s'\n",
            transport_secure_identifier_get_string(&message->session_id), 
            message->synchronization_stage,
            transport_address_get_string(outgoing_address));
    }
}

void c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
    {
        if (!session->handle_membership_update(message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_membership_update: failed to handle authoritative membership-update (%s update %d) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                message->update_number,
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_membership_update: channel '%s' ignoring membership-update (channel is not authoritative)\n",
            channel->get_name());
    }
}

void c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
    auto session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->is_host())
    {
        if (!session->handle_peer_properties(channel, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_properties: session failed to handle peer-properties (%s) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_properties: channel '%s' ignoring peer-properties (%s) (not host)\n",
            channel->get_name(),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

// TODO
void c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
    
}

// TODO
void c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
    
}

// TODO
void c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
    
}

void c_network_message_handler::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
    {
        if (!session->handle_player_refuse(channel, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_refuse: session failed to handle player-refuse (%s) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_refuse: channel '%s' ignoring player-refuse (%s) (channel not authoritative)\n",
            channel->get_name(),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

// TODO
void c_network_message_handler::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
    
}

void c_network_message_handler::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
    auto session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->is_host())
    {
        if (!session->handle_player_properties(channel, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_properties: session failed to handle player-properties (%s) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_properties: channel '%s' ignoring player-properties (%s) (not host)\n",
            channel->get_name(),
            transport_secure_identifier_get_string(&message->session_id));
    }
}

void c_network_message_handler::handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
    {
        if (!session->handle_parameters_update(message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_update: session failed to handle authoritative parameters-update (%s) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_update: channel '%s' ignoring parameters-update (channel is not authoritative)\n",
            channel->get_name());
    }
}

void c_network_message_handler::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->is_host())
    {
        if (!session->handle_parameters_request(channel, message))
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_request: session failed to handle parameters-request (%s) from '%s'\n",
                transport_secure_identifier_get_string(&message->session_id),
                channel->get_name());
        }
    }
    else
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_request: channel '%s' ignoring parameters-request (we are not the host)\n",
            channel->get_name());
    }
}

void c_network_message_handler::handle_view_establishment(c_network_channel* channel, s_network_message_view_establishment const* message)
{
    // non original but useful log
    //printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_view_establishment: establishment %d received\n", message->establishment_mode);

    typedef void(__thiscall* handle_view_establishment_ptr)(c_network_channel* channel/*c_network_message_handler* message_handler, c_network_channel* channel, s_network_message_view_establishment const* message*/);
    auto handle_view_establishment = reinterpret_cast<handle_view_establishment_ptr>(module_base + 0x257B0);
    return handle_view_establishment(channel/*this, channel, message*/);
}

void c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message)
{
    typedef void(__thiscall* handle_player_acknowledge_ptr)(c_network_channel* channel/*c_network_message_handler* message_handler, c_network_channel* channel, s_network_message_player_acknowledge const* message*/);
    auto handle_player_acknowledge = reinterpret_cast<handle_player_acknowledge_ptr>(module_base + 0x25810);
    return handle_player_acknowledge(channel/*this, channel, message*/);
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, s_network_message_synchronous_playback_control const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, s_network_message_synchronous_acknowledge const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_gamestate(c_network_channel* channel, s_network_message_synchronous_gamestate const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_synchronous_client_ready(c_network_channel* channel, s_network_message_synchronous_client_ready const* message)
{
    
}

// FUNC TODO
void c_network_message_handler::handle_game_results(c_network_channel* channel, s_network_message_game_results const* message)
{
    
}

void log_received_over_closed_channel(c_network_channel* channel, e_network_message_type message_type)
{
    const char* message_type_name = channel->m_type_collection->get_message_type_name(message_type);
    char* channel_name = channel->get_name();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over CLOSED channel '%s'\n",
        message_type, message_type_name, channel_name);
}

void log_received_over_non_connected_channel(c_network_channel* channel, e_network_message_type message_type)
{
    const char* message_type_name = channel->m_type_collection->get_message_type_name(message_type);
    char* channel_name = channel->get_name();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over a non-connected channel '%s', discarding\n",
        message_type, message_type_name, channel_name);
}

void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    // non-original log but its useful to know when channel messages arrive
    s_transport_address* remote_address_log = nullptr;
    channel->get_remote_address(remote_address_log);
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received channel message from '%s'\n",
        message_type,
        this->m_message_type_collection->get_message_type_name(message_type),
        transport_address_get_string(remote_address_log));

    s_transport_address* remote_address = nullptr;
    switch (message_type)
    {
        case _network_message_type_connect_establish:
            this->handle_connect_establish(channel, (s_network_message_connect_establish*)message);
            break;

        case _network_message_type_leave_session:
            if (channel->connected() && channel->get_remote_address(remote_address))
                this->handle_leave_session(remote_address, (s_network_message_leave_session*)message); // TODO
            else
                log_received_over_closed_channel(channel, _network_message_type_leave_session);
            break;

        case _network_message_type_session_disband:
            if (channel->connected() && channel->get_remote_address(remote_address))
                this->handle_session_disband(remote_address, (s_network_message_session_disband*)message);
            else
                log_received_over_closed_channel(channel, _network_message_type_session_disband);
            break;

        case _network_message_type_session_boot:
            if (channel->connected() && channel->get_remote_address(remote_address))
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
                this->handle_delegate_leadership(channel, (s_network_message_delegate_leadership*)message); // TODO (or don't, we won't need this)
            else
                log_received_over_non_connected_channel(channel, _network_message_type_delegate_leadership);
            break;

        case _network_message_type_boot_machine:
            if (channel->connected())
                this->handle_boot_machine(channel, (s_network_message_boot_machine*)message); // TODO (or don't, we won't need this)
            else
                log_received_over_non_connected_channel(channel, _network_message_type_boot_machine);
            break;

        case _network_message_type_player_add:
            if (channel->connected())
                this->handle_player_add(channel, (s_network_message_player_add*)message); // TODO (or don't, we won't need this)
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_add);
            break;

        case _network_message_type_player_refuse:
            if (channel->connected())
                this->handle_player_refuse(channel, (s_network_message_player_refuse*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_refuse);
            break;

        case _network_message_type_player_remove:
            if (channel->connected())
                this->handle_player_remove(channel, (s_network_message_player_remove*)message); // TODO (or don't, we won't need this)
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_remove);
            break;

        case _network_message_type_player_properties:
            if (channel->connected())
                this->handle_player_properties(channel, (s_network_message_player_properties*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_properties);
            break;

        case _network_message_type_parameters_update:
            if (channel->connected())
                this->handle_parameters_update(channel, (s_network_message_parameters_update*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_parameters_update);
            break;

        case _network_message_type_parameters_request:
            if (channel->connected())
                this->handle_parameters_request(channel, (s_network_message_parameters_request*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_parameters_request);
            break;

        case _network_message_type_view_establishment:
            if (channel->connected())
                this->handle_view_establishment(channel, (s_network_message_view_establishment*)message); // TODO - verify view establishment
            else
                log_received_over_non_connected_channel(channel, _network_message_type_view_establishment);
            break;

        case _network_message_type_player_acknowledge:
            if (channel->connected())
                this->handle_player_acknowledge(channel, (s_network_message_player_acknowledge*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_acknowledge);
            break;
        // UNIMPLEMENTED! TODO
        /*
        case _network_message_type_synchronous_update:
            if (channel->connected())
                this->handle_synchronous_update(channel, (s_network_message_synchronous_update*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_update);
            break;

        case _network_message_type_synchronous_playback_control:
            if (channel->connected())
                this->handle_synchronous_playback_control(channel, (s_network_message_synchronous_playback_control*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_playback_control);
            break;

        case _network_message_type_synchronous_actions:
            if (channel->connected())
                this->handle_synchronous_actions(channel, (s_network_message_synchronous_actions*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_actions);
            break;

        case _network_message_type_synchronous_acknowledge:
            if (channel->connected())
                this->handle_synchronous_acknowledge(channel, (s_network_message_synchronous_acknowledge*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_acknowledge);
            break;

        case _network_message_type_synchronous_gamestate:
            if (channel->connected())
                this->handle_synchronous_gamestate(channel, (s_network_message_synchronous_gamestate*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_gamestate);
            break;

        case _network_message_type_synchronous_client_ready:
            if (channel->connected())
                this->handle_synchronous_client_ready(channel, (s_network_message_synchronous_client_ready*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_synchronous_client_ready);
            break;

        case _network_message_type_game_results:
            if (channel->connected())
                this->handle_game_results(channel, (s_network_message_game_results*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_game_results);
            break;
        */
    }
}

c_network_message_gateway* c_network_message_handler::get_message_gateway()
{
    return this->m_message_gateway;
}