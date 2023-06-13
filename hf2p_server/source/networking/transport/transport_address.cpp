#include "transport_address.h"
#include <assert.h>
#include "transport_security.h"
#include <stdio.h>

enum
{
	k_maxmum_ipv4_string_length = 16
};

char const* transport_address_get_string(s_transport_address const* address)
{
	static char address_str[0x100]{};
	transport_address_to_string(address, nullptr, address_str, 0x100, false, true);
	return address_str;
}

char* transport_address_to_string(s_transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool include_port, bool include_security)
{
	assert(address);
	assert(string);
	assert(maximum_string_length > 0);

	char secure_address_string[0x100]{};
	s_transport_secure_identifier secure_identifier_from_address;
	s_transport_secure_address secure_address_from_address;

	csstrnzcpy(string, "", maximum_string_length);
	switch (address->address_size)
	{
	case 4:
		csnzprintf(string, maximum_string_length, "%hd.%hd.%hd.%hd",
			address->address.ipv4[3],
			address->address.ipv4[2],
			address->address.ipv4[1],
			address->address.ipv4[0]);

		if (include_port)
			csnzappendf(string, maximum_string_length, ":%hd", address->port);

		if (include_security)
		{
			if (secure_address != nullptr)
			{
				transport_secure_address_to_string(secure_address, secure_address_string, 0x100, false, false);
				csnzappendf(string, maximum_string_length, " %s", secure_address_string);
			}
			else if (transport_secure_identifier_retrieve(address, _transport_platform_pc, &secure_identifier_from_address, &secure_address_from_address))
			{
				transport_secure_address_to_string(&secure_address_from_address, secure_address_string, 0x100, false, false);
				csnzappendf(string, maximum_string_length, " %s [%s]", secure_address_string, transport_secure_identifier_get_string(&secure_identifier_from_address));
			}
			else
			{
				csnzappendf(string, maximum_string_length, " (insecure)");
			}
		}
		break;
	case 0x10:
		csnzprintf(string, maximum_string_length, "%04X.%04X.%04X.%04X.%04X.%04X.%04X.%04X",
			address->address.ipv6[0],
			address->address.ipv6[1],
			address->address.ipv6[2],
			address->address.ipv6[3],
			address->address.ipv6[4],
			address->address.ipv6[5],
			address->address.ipv6[6],
			address->address.ipv6[7]);

		if (include_port)
			csnzappendf(string, maximum_string_length, ":%hd", address->port);
		break;
	case 0xFFFF:
		csstrnzcpy(string, address->address.str, maximum_string_length);
		break;
	case 0:
		csstrnzcpy(string, "NULL_ADDRESS", maximum_string_length);
		break;
	}

	return string;
}

bool transport_address_equivalent(s_transport_address const* address1, s_transport_address const* address2)
{
	bool(__fastcall * transport_address_equivalent_call)(s_transport_address const* address1, s_transport_address const* address2) = reinterpret_cast<decltype(transport_address_equivalent_call)>(base_address(0x77D0));
	return transport_address_equivalent_call(address1, address2);
}

bool transport_address_valid(s_transport_address const* address)
{
	bool valid = false;
	if (address)
	{
		switch (address->address_size)
		{
			case -1:
				valid = address->address.ipv4 != 0;
				break;
			case 4:
				valid = address->address.ipv4 != 0;
				if (!address->address.ipv4)
					printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: transport_address_valid: the IPV4 address is NOT valid\n\n");
				break;
			case 16:
				for (long i = 0; i < 8; ++i)
				{
					if (*((ushort*)address + i))
					{
						valid = true;
						break;
					}
				}
				if (!valid)
					printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: transport_address_valid: the IPV6 address is NOT valid\n\n");
				break;
		}
	}
	return valid;
}