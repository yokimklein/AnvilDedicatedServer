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

    s_transport_secure_address addresses[3];
    // aed61e55-632d-4ea5-9bf9-ead4322b640f ZZVERTIGO
    byte temp_address1[16] = { 0x55, 0x1e, 0xd6, 0xae, 0x2d, 0x63, 0xa5, 0x4e, 0x9b, 0xf9, 0xea, 0xd4, 0x32, 0x2b, 0x64, 0x0f };
    // FFC6472C-AD90-4393-AD53-0741123F29CE ILIKENAME
    byte temp_address2[16] = { 0x2c, 0x47, 0xc6, 0xff, 0x90, 0xad, 0x93, 0x43, 0xad, 0x53, 0x07, 0x41, 0x12, 0x3f, 0x29, 0xce };
    // c073068b-5ca5-4c53-84cf-c720dfaa2f40 YOKIM

    // NEW: 0363777e-f9af-46b9-a26e-3c7bf1d42ce4
    // 0x7e, 0x77, 0x63, 0x03
    //                               reversed                reversed          reversed         forward                   forward
    byte temp_address3[16] = { 0x56, 0xf0, 0xea, 0xba, /**/ 0xbd, 0xc2, /**/ 0x76, 0x47, /**/ 0xac, 0xbf, /**/ 0xe8, 0x0c, 0xe0, 0x40, 0xbe, 0x68};

    memcpy(&addresses[0], temp_address1, sizeof(s_transport_secure_address));
    memcpy(&addresses[1], temp_address2, sizeof(s_transport_secure_address));
    memcpy(&addresses[2], temp_address3, sizeof(s_transport_secure_address));

    qword user_ids[] = { 1725920991, 1725921323, 1725921312 };

    for (long i = 0; i < NUMBEROF(addresses); i++)
    {
        if (addresses[i] == *secure_address)
        {
            return user_ids[i];
        }
    }

    printf("ONLINE/CLIENT/STUB_LOG_PATH,STUB_LOG_FILTER: user_xuid_from_secure_address: could not find userID associated with secure address [%s]!\n", transport_secure_address_get_string(secure_address));
    return USER_INVALID;
}