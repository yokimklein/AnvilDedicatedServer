#pragma once

struct s_transport_session_description;
enum e_transport_platform : long;
bool managed_session_get_security_information(long managed_session_index, bool unknown, s_transport_session_description* session_description, e_transport_platform* transport_platform);
const char* managed_session_get_id_string(long managed_session_index);