#include "transport_address.h"
#include "..\..\dllmain.h"
#include <assert.h>

enum
{
	k_maxmum_ipv4_string_length = 16
};
char* transport_address_get_string(s_transport_address const* address)
{
	char* address_string = nullptr;
	transport_address_to_string(address, 0, address_string, 256, false, true);
	return address_string;
}

// FUNC TODO
const char* transport_address_to_string(s_transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool append_port_info, bool append_security_info)
{
	const char* address_str = "(null)";
	return address_str;
	/*
	char secure_address_string[256];
	char secure_identifier_string[256];
	s_transport_secure_address secure_address_2;
	s_transport_secure_identifier secure_identifier;
#ifdef _DEBUG
	assert(!address);
	assert(!string);
	assert(maximum_string_length <= 0);
#endif
	csstrnzcpy(string, &"", maximum_string_length);
	if (address->address_size == 4) // ipv4
	{
		csnzprintf(string, maximum_string_length, "%hd.%hd.%hd.%hd", (uint8_t)(address->address.ipv4 & 0xFF), (uint8_t)((address->address.ipv4 >> 8) & 0xFF), (uint8_t)((address->address.ipv4 >> 16) & 0xFF), (uint8_t)((address->address.ipv4 >> 24) & 0xFF));
		if (append_port_info)
			csnzappendf(string, maximum_string_length, ":%hd", address->port);
		if (append_security_info)
		{
			csnzappendf(string, maximum_string_length, ":");
				if (secure_address)
				{
					transport_secure_address_to_string(secure_address, secure_address_string, 256, false, false);
					csstrnzcat(string, secure_address_string, maximum_string_length);
				}
				else if (transport_secure_identifier_retrieve(address, 0, secure_identifier, secure_address_2)) // 0 is an enum of e_transport_platform & seems to be the only one that's used
				{
					transport_secure_address_to_string(secure_address, secure_address_string, 256, false, false);
					csstrnzcat(string, secure_address_string, maximum_string_length);
					secure_identifier_string = transport_secure_identifier_get_string((const struct s_transport_secure_identifier*)secure_identifier);
					v42 = secure_identifier_string;
					csnzappendf(string, maximum_string_length, "[%s]", secure_identifier_string);
				}
				else
				{
					csnzappendf(string, maximum_string_length, " (insecure)");
				}
		}
	}
	else if (address->address_size == 16) // ipv6
	{
		ushort ipv6_blocks[8];
		memcpy(&ipv6_blocks[7], &address->address.data[14], 2);
		memcpy(&ipv6_blocks[6], &address->address.data[12], 2);
		memcpy(&ipv6_blocks[5], &address->address.data[10], 2);
		memcpy(&ipv6_blocks[4], &address->address.data[8], 2);
		memcpy(&ipv6_blocks[3], &address->address.data[6], 2);
		memcpy(&ipv6_blocks[2], &address->address.data[4], 2);
		memcpy(&ipv6_blocks[1], &address->address.data[2], 2);
		memcpy(&ipv6_blocks[0], &address->address.data[0], 2);
		csnzprintf(string, maximum_string_length, "%04X.%04X.%04X.%04X.%04X.%04X.%04X.%04X", ipv6_blocks[7], ipv6_blocks[6], ipv6_blocks[5], ipv6_blocks[4], ipv6_blocks[3], ipv6_blocks[2], ipv6_blocks[1], ipv6_blocks[0]);
		if (append_port_info)
			csnzappendf(string, maximum_string_length, ":%hd", address->port);
	}
	else if (address->address_size == 0xFFFF)
		csstrnzcpy(string, *(const char**)address, maximum_string_length);
	else if (address->address_size)
		display_assert(&"invalid transport address", __FILE__, __LINE__, true);
	else
		csstrnzcpy(string, "NULL_ADDRESS", maximum_string_length);

	return string;
	*/
}

bool transport_address_equivalent(s_transport_address const* address1, s_transport_address const* address2)
{
	bool(__fastcall * transport_address_equivalent_call)(s_transport_address const* address1, s_transport_address const* address2) = reinterpret_cast<decltype(transport_address_equivalent_call)>(module_base + 0x77D0);
	return transport_address_equivalent_call(address1, address2);
}