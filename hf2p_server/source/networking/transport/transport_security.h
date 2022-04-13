#pragma once
#include <windows.h>
#include <cstdint>

struct s_transport_secure_address
{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	byte data4[8];
};

struct s_transport_secure_identifier
{
	uint32_t  data1;
	uint16_t data2;
	uint16_t data3;
	byte data4[8];
};

struct s_transport_secure_key
{
	char data[16];
};

struct s_transport_session_description
{
	char data[16];
};

long transport_secure_address_get(GUID* transport_secure_address);
const char* transport_secure_nonce_get_string(const uint64_t* secure_nonce);
const char* transport_secure_address_get_string(const s_transport_secure_address* secure_address);