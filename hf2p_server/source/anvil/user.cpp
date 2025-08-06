#include "user.h"
#include <networking\transport\transport_security.h>
#include <stdio.h>

ulong g_user_sessions_count = 0;
s_user_sessions* g_user_sessions = nullptr;
e_request_status g_userid_request_status = _request_status_none;

qword user_xuid_from_secure_address(s_transport_secure_address const* secure_address)
{
    s_transport_secure_address address = *secure_address;

    // set system userid if local secure address
    if (transport_security_globals.local_secure_address == *secure_address)
    {
        return USER_SYSTEM;
    }

    // search for matching sessionID in user sessions cache and return userID
    for (ulong user_session_index = 0; user_session_index < g_user_sessions_count; user_session_index++)
    {
        if (g_user_sessions[user_session_index].session_id == *secure_address)
        {
            return g_user_sessions[user_session_index].user_id;
        }
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_xuid_from_secure_address: could not find userID associated with sessionID [%s]!\n",
        transport_secure_address_get_string(secure_address));
    return USER_INVALID;
}

bool user_sessions_for_lobby_request()
{
    if (g_userid_request_status != _request_status_none)
    {
        printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_sessions_for_lobby_request: tried to send user sessions request before previous request has received a response!\n");
        return false;
    }

    s_transport_secure_address secure_address;
    s_transport_secure_identifier lobby_identifier;
    if (!transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_windows, &lobby_identifier, &secure_address))
    {
        return false;
    }

    // $TODO: do request here
    // API request should provide the sessionID userID pairs for all EXPECTED members in the lobby to update local cache with
    // bare in mind sessionIDs are displayed on a user's screen so they are not a secure method of authentication
    g_userid_request_status = _request_status_waiting;

    return true;
}

void user_sessions_for_lobby_response()
{
    if (g_userid_request_status != _request_status_waiting)
    {
        return;
    }

    g_userid_request_status = _request_status_response_received;
    g_user_sessions_count = 0;
    if (g_user_sessions)
    {
        delete[] g_user_sessions;
    }

    // $TODO: fill out response data here
    
    // $TEMP: fake response data just to test this idea works
    s_userid_sessionid_pair user_info[] =
    {
        { 1725920991, L"{11761bb3-39ac-4458-a3a8-c1e8026a5833}" }, // zzVertigo
        { 1725921312, L"{21f3795b-2b6d-44c1-a56a-4311d7a0db83}" }, // Yokim
    };
    g_user_sessions_count = NUMBEROF(user_info);
    g_user_sessions = new s_user_sessions[g_user_sessions_count];

    for (ulong user_session_index = 0; user_session_index < g_user_sessions_count; user_session_index++)
    {
        //RPC_STATUS result = UuidFromStringA((RPC_CSTR)user_info[user_session_index].session_id, (UUID*)&g_user_sessions[user_session_index].session_id);

        g_user_sessions[user_session_index].user_id = user_info[user_session_index].user_id;
        HRESULT result = CLSIDFromString(user_info[user_session_index].session_id, (LPCLSID)&g_user_sessions[user_session_index].session_id);
        ASSERT(result == S_OK);
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_sessions_for_lobby_response: received [%d] user sessions\n", g_user_sessions_count);
}