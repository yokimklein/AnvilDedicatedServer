#include "lobby.h"

s_lobby_info g_lobby_info;

s_lobby_info::s_lobby_info()
    : s_request_info()
    , valid(false)
    , lobby_identifier()
{
}

void s_lobby_info::clear_lobby_identifier()
{
    g_lobby_info.valid = false;
    csmemset(&g_lobby_info.lobby_identifier, 0, sizeof(g_lobby_info.lobby_identifier));
}

void anvil_get_server_identifier(s_transport_secure_address* secure_address)
{
    //const wchar_t* serverid = L"{cdbeb834-c1a4-9a4d-8dda-f540f378d644}";
    //HRESULT result = CLSIDFromString(serverid, (LPCLSID)secure_address);
    //ASSERT(result == S_OK);
    get_session_secure_address(secure_address);
}

void anvil_get_lobby_identifier(s_transport_secure_identifier* secure_identifier)
{
    *secure_identifier = g_lobby_info.lobby_identifier;
}