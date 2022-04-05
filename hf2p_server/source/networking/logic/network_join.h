#pragma once

class c_network_session;
struct s_transport_address;
struct s_network_session_join_request;

void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request);
bool network_join_process_joins_from_queue();
void network_join_flush_join_queue();