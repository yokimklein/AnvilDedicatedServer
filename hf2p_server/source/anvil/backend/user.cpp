#include "user.h"
#include <networking\transport\transport_security.h>
#include <anvil\backend\services\private_service.h>
#include <anvil\backend\cache.h>
#include <stdio.h>

s_lobby_session_data::s_lobby_session_data()
    : valid(false)
    , users()
{
    reset_user_data();
}

void s_lobby_session_data::reset_user_data()
{
    valid = false;
    csmemset(users, 0, sizeof(users));
}

//s_user_session::s_user_session()
//{
//    valid = false;
//    user_id = USER_INVALID;
//    csmemset(&session_id, 0, sizeof(session_id));
//}

qword user_xuid_from_secure_address(s_transport_secure_address const* secure_address)
{
    s_transport_secure_address address = *secure_address;

    // set system userid if local secure address
    if (transport_security_globals.local_secure_address == *secure_address)
    {
        return USER_SYSTEM;
    }

    // search for matching sessionID in user sessions cache and return userID
    for (ulong user_session_index = 0; user_session_index < k_network_maximum_players_per_session; user_session_index++)
    {
        s_user_session& user_session = g_backend_data_cache.lobby_session.users[user_session_index];
        if (user_session.valid && user_session.session_id == *secure_address)
        {
            return user_session.user_id;
        }
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_xuid_from_secure_address: could not find userID associated with sessionID [%s]!\n",
        transport_secure_address_get_string(secure_address));
    return USER_INVALID;
}

bool user_sessions_request_for_lobby()
{
    if (c_backend::private_service::retrieve_lobby_members::m_status.status != _request_status_none)
    {
        printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_sessions_request_for_lobby: tried to send user sessions request before previous request has received a response!\n");
        return false;
    }

    s_transport_secure_address secure_address;
    s_transport_secure_identifier lobby_identifier;
    if (!transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_windows, &lobby_identifier, &secure_address))
    {
        printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_sessions_request_for_lobby: failed to retrieve lobby identifier for request!\n");
        return false;
    }

    c_backend::private_service::retrieve_lobby_members::request(transport_secure_identifier_get_string(&lobby_identifier));

    return true;
}
