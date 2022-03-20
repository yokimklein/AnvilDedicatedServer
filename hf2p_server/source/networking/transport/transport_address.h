#pragma once
#include <cstdint>
#include <windows.h>

enum e_transport_platform : long
{

};

struct s_transport_address
{
	union {
		uint32_t ipv4;
		byte data[16];
	} address;
	uint16_t port;
	uint16_t address_size; // 4 for IPv4, 16 for IPv6
};
static_assert(sizeof(s_transport_address) == 0x14);

//char* transport_address_get_string(s_transport_address const* address);
//char* transport_address_to_string(s_transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool append_port_info, bool append_security_info);
