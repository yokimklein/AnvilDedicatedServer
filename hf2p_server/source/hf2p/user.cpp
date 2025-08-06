#include "user.h"
#include <networking\transport\transport_security.h>
#include <stdio.h>

qword user_xuid_from_secure_address(s_transport_secure_address const* secure_address)
{
    s_transport_secure_address address = *secure_address;

    // set system userid if local secure address
    if (transport_security_globals.local_secure_address == *secure_address)
    {
        return USER_SYSTEM;
    }

    s_userid_sessionid_pair user_info[] =
    {
        { 1725920991, L"{11761bb3-39ac-4458-a3a8-c1e8026a5833}" }, // zzVertigo
        { 1725921312, L"{ecb9a859-da69-4c5d-ad81-59e446fd4342}" }, // Yokim
    };
    s_transport_secure_address addresses[NUMBEROF(user_info)];

    for (long i = 0; i < NUMBEROF(addresses); i++)
    {
        //RPC_STATUS result = UuidFromStringA((RPC_CSTR)user_info[i].session_id, (UUID*)&addresses[i]);
        HRESULT result = CLSIDFromString(user_info[i].session_id, (LPCLSID)&addresses[i]);
        ASSERT(result == S_OK);
    }

    for (long i = 0; i < NUMBEROF(addresses); i++)
    {
        if (addresses[i] == *secure_address)
        {
            return user_info[i].user_id;
        }
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_xuid_from_secure_address: could not find userID associated with sessionID [%s]!\n",
        transport_secure_address_get_string(secure_address));
    return USER_INVALID;
}