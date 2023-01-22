#include "transport_security.h"
#include "..\..\dllmain.h"
#include "transport_shim.h"
#include <stdio.h>
#include "..\..\anvil\server_tools.h"
#include <winsock.h>

char const* transport_secure_nonce_get_string(qword secure_nonce)
{
    byte* nonce_bytes = (byte*)&secure_nonce;
    static char nonce_str[0x13]{};
    csnzprintf(nonce_str, 0x13,
        "%02X%02X%02X%02X-%02X%02X%02X%02X",
        nonce_bytes[7],
        nonce_bytes[6],
        nonce_bytes[5],
        nonce_bytes[4],
        nonce_bytes[3],
        nonce_bytes[2],
        nonce_bytes[1],
        nonce_bytes[0]);
    return nonce_str;
}

char const* transport_secure_address_get_string(s_transport_secure_address const* secure_address)
{
    static char secure_addr_str[0x26]{};
    return transport_secure_address_to_string(secure_address, secure_addr_str, 0x26, false, false);
    return secure_addr_str;
}

char const* transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
    static char secure_id_str[0x26]{};
    csnzprintf(secure_id_str, 0x26,
        "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        secure_identifier->part0,
        secure_identifier->part4[0],
        secure_identifier->part4[1],
        secure_identifier->part8[0],
        secure_identifier->part8[1],
        secure_identifier->part8[2],
        secure_identifier->part8[3],
        secure_identifier->part8[4],
        secure_identifier->part8[5],
        secure_identifier->part8[6],
        secure_identifier->part8[7]);
    return secure_id_str;
}

char const* transport_session_description_get_string(s_transport_session_description const* session_description)
{
    static char session_desc_str[0x64]{};
    csnzprintf(session_desc_str, 0x64, "%s-%s", transport_secure_identifier_get_string(&session_description->session_id), transport_secure_address_get_string(&session_description->host_address));
    return session_desc_str;
}

bool transport_secure_address_get(s_transport_secure_address* secure_address)
{
    if (secure_address != nullptr)
        *secure_address = transport_security_globals->secure_address;
    return transport_security_globals->address_resolved;
}

const s_transport_unique_identifier* transport_unique_identifier_get()
{
    return &transport_security_globals->local_unique_identifier;
}

void transport_secure_address_build_from_identifier(s_transport_unique_identifier const* unique_identifier, s_transport_secure_address* secure_address)
{
    csmemset(secure_address, 0, sizeof(s_transport_secure_address));
    csmemcpy(&secure_address, &unique_identifier, sizeof(s_transport_secure_address));
}

char* transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* string, long maximum_string_length, bool include_online, bool include_mac)
{
    csnzprintf(string,
        maximum_string_length,
        "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        secure_address->part0,
        secure_address->part4[0],
        secure_address->part4[1],
        secure_address->part8[0],
        secure_address->part8[1],
        secure_address->part8[2],
        secure_address->part8[3],
        secure_address->part8[4],
        secure_address->part8[5],
        secure_address->part8[6],
        secure_address->part8[7]);
    return string;
}

bool transport_secure_identifier_retrieve(s_transport_address const* usable_address, e_transport_platform transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
    if (usable_address->address_size == 4 && usable_address != nullptr && transport_platform != _transport_platform_xnet)
        return xnet_shim_inaddr_to_xnaddr2(usable_address, secure_address, secure_identifier);
    else
        return false;
}

const char* transport_secure_address_get_mac_string(s_transport_secure_address const* secure_address)
{
    s_transport_unique_identifier unique_id;
    transport_secure_address_extract_identifier(secure_address, &unique_id);
    return transport_unique_identifier_get_string(&unique_id);
}

void transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_id)
{
    memcpy(unique_id, secure_address, sizeof(s_transport_unique_identifier));
}

char const* transport_unique_identifier_get_string(s_transport_unique_identifier const* unique_id)
{
    static char unique_id_str[0x26]{};
    csnzprintf(unique_id_str, 0x26,
        "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        unique_id->part0,
        unique_id->part4[0],
        unique_id->part4[1],
        unique_id->part8[0],
        unique_id->part8[1],
        unique_id->part8[2],
        unique_id->part8[3],
        unique_id->part8[4],
        unique_id->part8[5],
        unique_id->part8[6],
        unique_id->part8[7]);
    return unique_id_str;
}

void transport_secure_address_generate(s_transport_secure_address* secure_address)
{
    if (game_is_dedicated_server())
        anvil_get_dedicated_secure_address(secure_address);
    else
        get_session_secure_address(secure_address);
}

// rewrote this for a hook, but xnet_shim_get_title_xnaddr doesn't play nice with it, so this goes unused
// the mid section was reused however
bool transport_secure_address_resolve()
{
    if (!transport_security_globals->address_resolved)
    {
        s_transport_address transport_addresses[8];
        memset(transport_addresses, 0, sizeof(transport_addresses));
        char host_name[256];
        gethostname(host_name, 256);
        host_name[255] = 0;
        if (xnet_shim_get_title_xnaddr(transport_addresses))
        {
            transport_security_globals->address = transport_addresses[0];
            transport_security_globals->address.port = *g_game_port;
            memset(&transport_security_globals->secure_address, 0, sizeof(s_transport_secure_address));
            transport_secure_address_generate(&transport_security_globals->secure_address);
            transport_security_globals->address_resolved = true;
            transport_secure_address_resolve();
            memcpy(&transport_security_globals->local_unique_identifier, &transport_security_globals->secure_address, sizeof(s_transport_unique_identifier));
        }
        else
        {
            printf("MP/NET/TRANSPORT,SECURE: %s: Address resolution failed, networking is unavailable\n", __FUNCTION__);
            transport_security_globals->address_resolved = false;
        }
    }
    return transport_security_globals->address_resolved;
}

s_transport_secure_address* get_session_secure_address(s_transport_secure_address* out_address)
{
    *out_address = *g_session_secure_address;
    return out_address;
}

bool transport_secure_key_create(s_transport_session_description* session_description)
{
    if (session_description != (s_transport_session_description*)0xFFFFFFF0) // what's up with this?
        session_description->host_address = transport_security_globals->secure_address;

    if (transport_security_globals->address_resolved)
    {
        xnet_shim_create_key(session_description);
        // register the host address description to the xnet shim table on session creation
        xnet_shim_register(&transport_security_globals->address, &session_description->host_address, &session_description->session_id);
        return true;
    }
    else
    {
        printf("MP/NET/TRANSPORT,SEC: %s: failed to get IP address.\n", __FUNCTION__);
        return false;
    }
}