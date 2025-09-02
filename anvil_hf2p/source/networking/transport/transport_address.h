#pragma once
#include <cseries\cseries.h>

// wherever this is used seems to come from a precompiler macro specifying the build platform
enum e_transport_platform
{
	_transport_platform_xenon = 0,
	_transport_platform_windows,

	k_transport_platform_count,
	k_transport_platform_bits = 2,
	k_transport_platform_none = NONE,
};

struct transport_address
{
	union
	{
		ulong ipv4_address;

		// in_addr
		// address_length == 0x4
		union
		{
			byte bytes[4];
			word words[2];
		} ina;

		// in6_addr
		// address_length == 0x10
		union
		{
			byte bytes[16];
			word words[8];
		} ina6;

		// address_length == 0xFFFF
		const char* str;
	};

	ushort port;
	ushort address_size; // 4 for IPv4, 16 for IPv6
};
static_assert(sizeof(transport_address) == 0x14);

struct s_transport_secure_address;
char const* transport_address_get_string(transport_address const* address);
char* transport_address_to_string(transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool append_port_info, bool append_security_info);
bool __fastcall transport_address_equivalent(transport_address const* address1, transport_address const* address2);
bool transport_address_valid(transport_address const* address);