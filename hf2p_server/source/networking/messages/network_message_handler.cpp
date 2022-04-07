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

// inlined in the ms29 client
void c_network_message_handler::handle_ping(s_transport_address const* outgoing_address, s_network_message_ping const* message) // untested
{
    const char* address_string = "(null)"; // transport_address_get_string(&outgoing_address); TODO
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_ping: ping #%d received from '%s' at local %dms\n", message->id, address_string, timeGetTime());
    s_network_message_pong response;
    response.id = message->id;
    response.timestamp = message->timestamp;
    response.request_qos = message->request_qos;
    this->m_message_gateway->send_message_directed(outgoing_address, _network_message_type_pong, sizeof(s_network_message_pong), &response);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_pong(s_transport_address const* outgoing_address, s_network_message_pong const* message) // untested
{
    uint32_t time;

    if (network_time_locked)
        time = *g_network_locked_time;
    else
        time = timeGetTime();
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_pong: ping #%d returned from '%s' at local %dms (latency %dms)\n", 
        message->id, "(null)"/*transport_address_get_string(&outgoing_address)*/, timeGetTime(), time - message->timestamp);
}

void c_network_message_handler::handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message) // tested - seems to work
{
    typedef long(__fastcall* handle_connect_refuse_ptr)(c_network_channel* channel, s_network_message_connect_refuse const* message);
    auto handle_connect_refuse = reinterpret_cast<handle_connect_refuse_ptr>(module_base + 0x25AC0);
    handle_connect_refuse(channel, message);
}

void c_network_message_handler::handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message) // untested
{
    typedef long(__fastcall* handle_connect_establish_ptr)(c_network_channel* channel, s_network_message_connect_establish const* message);
    auto handle_connect_establish = reinterpret_cast<handle_connect_establish_ptr>(module_base + 0x25B10);
    handle_connect_establish(channel, message);
}

// inlined in the ms29 client
void c_network_message_handler::handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message) // untested
{
    if (channel && channel->m_state > _network_channel_state_closed && channel->m_identifier == message->identifier)
        channel->close(_network_channel_reason_remote_closure);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_join_request(s_transport_address const* outgoing_address, s_network_message_join_request const* message)
{
    if (message->protocol_version == 9)
    {
        // TEMP HACK - TODO
        c_network_session* session = this->m_session_manager->session[0]; //this->m_session_manager->get_session(&message->session_id); // get_session is returning null! TODO - investigate why
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
    const char* transport_address_string = "(null)"; // transport_address_get_string();
    const char* secure_identifier_string = "(null)"; // transport_secure_identifier_get_string((int)(message + 6));
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: can't handle join-request for '%s' from '%s'\n", secure_identifier_string, transport_address_string);
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: failed to handle incomming join request\n");
}

void c_network_message_handler::handle_peer_connect(s_transport_address const* outgoing_address, s_network_message_peer_connect const* message) // untested
{
    if (message->protocol_version == 9)
    {
        auto* session = this->m_session_manager->get_session(&message->session_id);
        if (session)
            session->handle_peer_connect(outgoing_address, message);
    }
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_join_abort(s_transport_address const* outgoing_address, s_network_message_join_abort const* message)
{
    
}

void c_network_message_handler::handle_join_refuse(s_transport_address const* outgoing_address, s_network_message_join_refuse const* message) // untested
{
    typedef void(__fastcall* handle_join_refuse_ptr)(c_network_message_handler* message_handler, s_transport_address const* outgoing_address, s_network_message_join_refuse const* message);
    auto handle_join_refuse = reinterpret_cast<handle_join_refuse_ptr>(module_base + 0x25660);
    handle_join_refuse(this, outgoing_address, message);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_leave_session(s_transport_address const* outgoing_address, s_network_message_leave_session const* message)
{
    
}

void c_network_message_handler::handle_leave_acknowledge(s_transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message) // untested
{
    typedef void(__fastcall* handle_leave_acknowledge_ptr)(c_network_message_handler* message_handler, s_transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message);
    auto handle_leave_acknowledge = reinterpret_cast<handle_leave_acknowledge_ptr>(module_base + 0x256E0);
    return handle_leave_acknowledge(this, outgoing_address, message);
}

void c_network_message_handler::handle_session_disband(s_transport_address const* outgoing_address, s_network_message_session_disband const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
        session->handle_session_disband(outgoing_address, message);
}

void c_network_message_handler::handle_session_boot(s_transport_address const* outgoing_address, s_network_message_session_boot const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
        session->handle_session_boot(outgoing_address, message);
}

void c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
        session->handle_host_decline(channel, message);
}

void c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message) // untested
{
    typedef void(__fastcall* handle_peer_establish_ptr)(c_network_message_handler* message_handler, c_network_channel* channel, s_network_message_peer_establish const* message);
    auto handle_peer_establish = reinterpret_cast<handle_peer_establish_ptr>(module_base + 0x25730);
    return handle_peer_establish(this, channel, message);
}

void c_network_message_handler::handle_time_synchronize(s_transport_address const* outgoing_address, s_network_message_time_synchronize const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session)
        session->handle_time_synchronize(outgoing_address, message);
}

void c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
        session->handle_membership_update(message);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
    
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
    
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
    
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
    
}

void c_network_message_handler::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
        session->handle_player_refuse(channel, message);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
    
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
    
}

void c_network_message_handler::handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message) // untested
{
    auto* session = this->m_session_manager->get_session(&message->session_id);
    if (session && session->channel_is_authoritative(channel))
        session->handle_parameters_update(message);
}

// MISSING FROM MS29 CLIENT
void c_network_message_handler::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
    
}

void c_network_message_handler::handle_view_establishment(c_network_channel* channel, s_network_message_view_establishment const* message) // untested
{
    typedef void(__fastcall* handle_view_establishment_ptr)(c_network_message_handler* message_handler, c_network_channel* channel, s_network_message_view_establishment const* message);
    auto handle_view_establishment = reinterpret_cast<handle_view_establishment_ptr>(module_base + 0x257B0);
    return handle_view_establishment(this, channel, message);
}

void c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message) // untested
{
    typedef void(__fastcall* handle_player_acknowledge_ptr)(c_network_message_handler* message_handler, c_network_channel* channel, s_network_message_player_acknowledge const* message);
    auto handle_player_acknowledge = reinterpret_cast<handle_player_acknowledge_ptr>(module_base + 0x25810);
    return handle_player_acknowledge(this, channel, message);
}

void c_network_message_handler::handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message)
{
    // TODO
}

void c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, s_network_message_synchronous_playback_control const* message)
{
    // TODO
}

void c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message)
{
    // TODO
}

void c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, s_network_message_synchronous_acknowledge const* message)
{
    // TODO
}

void c_network_message_handler::handle_synchronous_gamestate(c_network_channel* channel, s_network_message_synchronous_gamestate const* message)
{
    // TODO
}

void c_network_message_handler::handle_synchronous_client_ready(c_network_channel* channel, s_network_message_synchronous_client_ready const* message)
{
    // TODO
}

void c_network_message_handler::handle_game_results(c_network_channel* channel, s_network_message_game_results const* message)
{
    // TODO
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

// might need a return code?
void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    s_transport_address remote_address;
    switch (message_type)
    {
        case _network_message_type_connect_establish:
            this->handle_connect_establish(channel, (s_network_message_connect_establish*)message);
            break;

        //case _network_message_type_leave_session: // TODO - removed by saber in client builds
        //    if (channel->connected() && channel->get_remote_address(&remote_address))
        //        this->handle_leave_session(&remote_address, (s_network_message_leave_session*)message);
        //    else
        //        log_received_over_closed_channel(channel, _network_message_type_leave_session);
        //    break;

        case _network_message_type_session_disband:
            if (channel->connected() && channel->get_remote_address(&remote_address))
                this->handle_session_disband(&remote_address, (s_network_message_session_disband*)message);
            else
                log_received_over_closed_channel(channel, _network_message_type_session_disband);
            break;

        case _network_message_type_session_boot:
            if (channel->connected() && channel->get_remote_address(&remote_address))
                this->handle_session_boot(&remote_address, (s_network_message_session_boot*)message);
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

        //case _network_message_type_peer_properties:
        //    if (channel->connected())
        //        this->handle_peer_properties(channel, (s_network_message_peer_properties*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_peer_properties);
        //    break;

        //case _network_message_type_delegate_leadership:
        //    if (channel->connected())
        //        this->handle_delegate_leadership(channel, (s_network_message_delegate_leadership*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_delegate_leadership);
        //    break;

        //case _network_message_type_boot_machine:
        //    if (channel->connected())
        //        this->handle_boot_machine(channel, (s_network_message_boot_machine*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_boot_machine);
        //    break;

        //case _network_message_type_player_add:
        //    if (channel->connected())
        //        this->handle_player_add(channel, (s_network_message_player_add*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_player_add);
        //    break;

        case _network_message_type_player_refuse:
            if (channel->connected())
                this->handle_player_refuse(channel, (s_network_message_player_refuse*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_player_refuse);
            break;

        //case _network_message_type_player_remove:
        //    if (channel->connected())
        //        this->handle_player_remove(channel, (s_network_message_player_remove*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_player_remove);
        //    break;

        //case _network_message_type_player_properties:
        //    if (channel->connected())
        //        this->handle_player_properties(channel, (s_network_message_player_properties*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_player_properties);
        //    break;

        case _network_message_type_parameters_update:
            if (channel->connected())
                this->handle_parameters_update(channel, (s_network_message_parameters_update*)message);
            else
                log_received_over_non_connected_channel(channel, _network_message_type_parameters_update);
            break;

        //case _network_message_type_parameters_request:
        //    if (channel->connected())
        //        this->handle_parameters_request(channel, (s_network_message_parameters_request*)message);
        //    else
        //        log_received_over_non_connected_channel(channel, _network_message_type_parameters_request);
        //    break;

        case _network_message_type_view_establishment:
            if (channel->connected())
                this->handle_view_establishment(channel, (s_network_message_view_establishment*)message);
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

// hooks
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_channel_message(channel, message_type, message_storage_size, message);
}
// DWORD *this, int network_channel, int network_message_type, int a4, int message
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unknown, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason reason)
{
    // TODO: original logic
    const char* name = "<unknown>";
    if (reason <= k_network_join_refuse_reason_count)
    {
        switch (reason)
        {
        case _network_join_refuse_reason_none:
            name = "no-reason-given";
            break;
        case _network_join_refuse_reason_tried_to_join_self:
            name = "tried-to-join-self";
            break;
        case _network_join_refuse_reason_could_not_connect:
            name = "could-not-connect";
            break;
        case _network_join_refuse_reason_join_timed_out:
            name = "join-timed-out";
            break;
        case _network_join_refuse_reason_not_found:
            name = "not-found";
            break;
        case _network_join_refuse_reason_privacy_mode:
            name = "privacy-mode";
            break;
        case _network_join_refuse_reason_not_joinable:
            name = "not-joinable";
            break;
        case _network_join_refuse_reason_session_full:
            name = "session-full";
            break;
        case _network_join_refuse_reason_alpha_split_screen:
            name = "alpha-split-screen";
            break;
        case _network_join_refuse_reason_session_disband:
            name = "session-disband";
            break;
        case _network_join_refuse_reason_session_booted:
            name = "session-booted";
            break;
        case _network_join_refuse_reason_address_invalid:
            name = "address-invalid";
            break;
        case _network_join_refuse_reason_address_failed:
            name = "address-failed";
            break;
        case _network_join_refuse_reason_too_many_observers:
            name = "too-many-observers";
            break;
        case _network_join_refuse_reason_aborted:
            name = "aborted";
            break;
        case _network_join_refuse_reason_abort_ignored:
            name = "abort-ignored";
            break;
        case _network_join_refuse_reason_wrong_payload_type:
            name = "wrong-payload-type";
            break;
        case _network_join_refuse_reason_no_reservation:
            name = "no-reservation";
            break;
        case _network_join_refuse_reason_in_matchmaking:
            name = "in-matchmaking";
            break;
        case _network_join_refuse_reason_player_count_zero:
            name = "player-count-zero";
            break;
        case _network_join_refuse_reason_player_not_online_enabled:
            name = "player-not-online-enabled";
            break;
        case _network_join_refuse_reason_player_add_pending:
            name = "player-add-pending";
            break;
        case _network_join_refuse_reason_player_add_failed:
            name = "player-add-failed";
            break;
        case _network_join_refuse_reason_host_time_out:
            name = "host-time-out";
            break;
        case _network_join_refuse_reason_rejected_by_host:
            name = "rejected-by-host";
            break;
        case _network_join_refuse_reason_peer_version_too_low:
            name = "peer-version-too-low";
            break;
        case _network_join_refuse_reason_host_version_too_low:
            name = "host-version-too-low";
            break;
        case _network_join_refuse_reason_holding_in_queue:
            name = "holding-in-queue";
            break;
        case _network_join_refuse_reason_film_in_progress:
            name = "film-in-progress";
            break;
        case _network_join_refuse_reason_campaign_in_progress:
            name = "campaign-in-progress";
            break;
        case _network_join_refuse_reason_user_content_not_allowed:
            name = "user-content-not-allowed";
            break;
        case _network_join_refuse_reason_survival_in_progress:
            name = "survival-in-progress";
            break;
        case _network_join_refuse_reason_executable_type_mismatch:
            name = "executable-type-mismatch";
            break;
        }
    }
    return name;
}