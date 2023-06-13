#pragma once
#include <cseries\cseries.h>

// wherever this is used seems to come from a precompiler macro specifying the build platform
enum e_transport_platform : long
{
	// assumed names
	_transport_platform_xnet = 0,
	_transport_platform_pc = 1,

	k_transport_platform_count = 2
};

struct s_transport_address // just transport_address in original source
{
	union {
		byte data[0x10];
		byte ipv4[0x4];
		word ipv6[0x8];
		char const* str;
	} address;
	ushort port;
	ushort address_size; // 4 for IPv4, 16 for IPv6
};
static_assert(sizeof(s_transport_address) == 0x14);

struct s_transport_secure_address;

char const* transport_address_get_string(s_transport_address const* address);
char* transport_address_to_string(s_transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool append_port_info, bool append_security_info);
bool transport_address_equivalent(s_transport_address const* address1, s_transport_address const* address2);
bool transport_address_valid(s_transport_address const* address);