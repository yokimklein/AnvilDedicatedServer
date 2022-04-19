#pragma once
#include <windows.h>
#include <cstdint>

struct s_transport_secure_address
{
	byte data[16];
};

struct s_transport_secure_identifier
{
	byte data[16];
};

struct s_transport_secure_key
{
	byte data[16];
};

struct s_transport_session_description
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_session_description) == 0x30);

long transport_secure_address_get(GUID* transport_secure_address);
const char* transport_secure_nonce_get_string(const uint64_t* secure_nonce);
const char* transport_secure_address_get_string(const s_transport_secure_address* secure_address);