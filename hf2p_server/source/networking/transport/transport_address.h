#pragma once
#include <Windows.h>

struct s_transport_secure_address
{
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};

struct s_transport_secure_identifier
{
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};

struct s_transport_secure_key
{
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};

struct s_transport_address
{
	union {
		unsigned long ipv4;
		byte data[16];
	} address;
	unsigned short port;
	unsigned short address_size; // 4 for IPv4, 16 for IPv6
};

//char* transport_address_get_string(s_transport_address const* address);
//char* transport_address_to_string(s_transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool append_port_info, bool append_security_info);