#pragma once
#include <atomic>
#include <networking\transport\transport_security.h>
#include <anvil\backend\backend.h>

struct s_lobby_info : s_request_info
{
    s_lobby_info();
    void clear_lobby_identifier();

    bool valid;
    s_transport_secure_identifier lobby_identifier;
};

void anvil_get_server_identifier(s_transport_secure_address* secure_address);
void anvil_get_lobby_identifier(s_transport_secure_identifier* secure_identifier);

extern s_lobby_info g_lobby_info;