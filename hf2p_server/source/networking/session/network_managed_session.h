#pragma once
#include "../transport/transport_security.h"

bool managed_session_get_security_information(long managed_session_index, bool unknown, s_transport_session_description* session_description, e_transport_platform* transport_platform);