#include <windows.h>
#include "transport_security.h"
#include "..\..\dllmain.h"

long transport_secure_address_get(s_transport_secure_address* secure_address)
{
    if (secure_address) // why check the pointer?
    {
        s_transport_secure_address* unknown_secure_address = (s_transport_secure_address*)(module_base + 0x4EBE9D2);
        memcpy(secure_address, unknown_secure_address, sizeof(s_transport_secure_address));
    }
    return *(long*)(module_base + 0x4EBE9D1);
}

// FUNC TODO
const char* transport_secure_nonce_get_string(uint64_t const* secure_nonce)
{
    const char* secure_nonce_string = "00000000-00000000";
    return secure_nonce_string;
}

// FUNC TODO
const char* transport_secure_address_get_string(s_transport_secure_address const* secure_address)
{
    const char* secure_address_string = "00000000-00000-0000-0000-000000000000";
    return secure_address_string;
}

// FUNC TODO
const char* transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
    const char* secure_identifier_string = "00000000-00000-0000-0000-000000000000";
    return secure_identifier_string;
}

const s_transport_unique_identifier* transport_unique_identifier_get()
{
    return (s_transport_unique_identifier*)(module_base + 0x4EBEA08);
}

void transport_secure_address_build_from_identifier(s_transport_unique_identifier const* unique_identifier, s_transport_secure_address* secure_address)
{
    memcpy(&secure_address->data, &unique_identifier->data, 16);
}

// FUNC TODO
const char* transport_session_description_get_string(s_transport_session_description const* session_description)
{
    const char* session_description_str = "00000000-00000-0000-0000-000000000000-00000000-00000-0000-0000-000000000000";
    return session_description_str;
}