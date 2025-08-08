#include "user.h"
#include <networking\transport\transport_security.h>
#include <anvil\backend\private_service.h>
#include <combaseapi.h>
#include <stdio.h>

s_lobby_session_data g_lobby_session_data;

s_lobby_session_data::s_lobby_session_data()
    : status(_request_status_none)
    , users()
{
    reset_user_data();
}

void s_lobby_session_data::reset_user_data()
{
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
        s_user_session& user_session = g_lobby_session_data.users[user_session_index];
        if (user_session.valid && user_session.session_id == *secure_address)
        {
            return user_session.user_id;
        }
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_xuid_from_secure_address: could not find userID associated with sessionID [%s]!\n",
        transport_secure_address_get_string(secure_address));
    return USER_INVALID;
}

bool user_sessions_for_lobby_request()
{
    if (g_lobby_session_data.status != _request_status_none)
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
    g_lobby_session_data.status = _request_status_waiting;

    return true;
}

void user_sessions_for_lobby_response()
{
    if (g_lobby_session_data.status != _request_status_waiting)
    {
        return;
    }

    // reset user sessions
    g_lobby_session_data.status = _request_status_received;
    g_lobby_session_data.reset_user_data();

    // $TODO: fill out response data here
    
    // $TEMP: fake response data just to test this idea works
    s_userid_sessionid_pair user_infos[] =
    {
        { 1754553205, L"{11761bb3-39ac-4458-a3a8-c1e8026a5833}" }, // zzVertigo
        { 1754553261, L"{9193f62b-ad49-407e-9e0d-1431de13c862}" }, // Yokim
    };
    ulong user_sessions_count = NUMBEROF(user_infos);

    for (ulong user_session_index = 0; user_session_index < user_sessions_count; user_session_index++)
    {
        s_user_session& user_session = g_lobby_session_data.users[user_session_index];
        s_userid_sessionid_pair& user_info = user_infos[user_session_index];

        user_session.valid = true;
        user_session.user_id = user_info.user_id;
        HRESULT result = CLSIDFromString(user_info.session_id, (LPCLSID)&user_session.session_id);
        ASSERT(result == S_OK);

        //RPC_STATUS result = UuidFromStringA((RPC_CSTR)user_info[user_session_index].session_id, (UUID*)&g_user_sessions[user_session_index].session_id);
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_sessions_for_lobby_response: received [%d] user sessions\n", user_sessions_count);
}
