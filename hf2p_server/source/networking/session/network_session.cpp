#include <iostream>
#include "..\..\dllmain.h"
#include "network_session.h"
#include "..\messages\network_message_gateway.h"

bool c_network_session::acknowledge_join_request(s_transport_address const* address, e_network_join_refuse_reason reason)
{
    typedef char(__fastcall* managed_session_get_id_ptr)(int session_index, s_transport_secure_identifier* secure_identifier);
    auto managed_session_get_id = reinterpret_cast<managed_session_get_id_ptr>(module_base + 0x28AE0);

    const char* transport_address_string = "(null)"; // (const char*)transport_address_get_string();
    const char* join_refuse_reason_string = network_message_join_refuse_get_reason_string(reason);
    const char* managed_session_index_string = "(null)"; // managed_session_get_id_string(session[618236]);
    printf("MP/NET/SESSION,CTRL: c_network_session::acknowledge_join_request: [%s] join failed, sending refusal (%s - possibly inaccurate) to '%s'\n", managed_session_index_string, join_refuse_reason_string, transport_address_string);
    s_network_message_join_refuse message;
    managed_session_get_id(this->m_managed_session_index, &message.session_id);
    message.reason = reason;
    return this->m_message_gateway->send_message_directed(address, _network_message_type_join_refuse, 20, &message);
}

char c_network_session::handle_join_request(s_transport_address const* address, s_network_message_join_request const* message)
{
    // temporary - TODO
    // log the packet send
    this->acknowledge_join_request(address, _network_join_refuse_reason_holding_in_queue);
    return 1;
}