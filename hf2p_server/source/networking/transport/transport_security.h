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

struct s_transport_unique_identifier
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

long transport_secure_address_get(s_transport_secure_address* secure_address);
const char* transport_secure_nonce_get_string(uint64_t const* secure_nonce);
const char* transport_secure_address_get_string(s_transport_secure_address const* secure_address);
const char* transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier);
const s_transport_unique_identifier* transport_unique_identifier_get();
void transport_secure_address_build_from_identifier(s_transport_unique_identifier const* unique_identifier, s_transport_secure_address* secure_address);
const char* transport_session_description_get_string(s_transport_session_description const* session_description);