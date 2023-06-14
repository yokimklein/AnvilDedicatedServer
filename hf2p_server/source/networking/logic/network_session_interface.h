#pragma once

class c_network_session;
struct s_network_session_peer;
void __fastcall network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer);
void network_session_check_properties(c_network_session* session);