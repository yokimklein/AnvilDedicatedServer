#pragma once
#include <networking\transport\transport_security.h>

struct s_lobby_info
{
    s_lobby_info();
    void clear_lobby_identifier();

    bool valid;
    s_transport_secure_identifier lobby_identifier;
};

void anvil_get_server_identifier(s_transport_secure_address* secure_address);
void anvil_get_lobby_identifier(s_transport_secure_identifier* secure_identifier);
