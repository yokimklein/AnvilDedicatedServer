#pragma once
#include "transport_address.h"
#include "transport_security.h"

void xnet_shim_table_add(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_session_description* session_description);