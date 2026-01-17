#include "transport_address.h"
#include <networking\transport\transport_security.h>
#include <stdio.h>
#include <cseries\cseries_events.h>

enum
{
	k_maxmum_ipv4_string_length = 16
};

char const* transport_address_get_string(transport_address const* address)
{
	static char address_str[0x100]{};
	transport_address_to_string(address, nullptr, address_str, 0x100, false, true);
	return address_str;
}

char* transport_address_to_string(transport_address const* address, s_transport_secure_address const* secure_address, char* string, short maximum_string_length, bool include_port, bool include_extra)
{
	ASSERT(address);
	ASSERT(string);
	ASSERT(maximum_string_length > 0);

	static char static_string[256]{};
	s_transport_secure_identifier secure_identifier_from_address{};
	s_transport_secure_address secure_address_from_address{};

	csstrnzcpy(string, "", maximum_string_length);
	switch (address->address_length)
	{
		case IPV4_ADDRESS_LENGTH:
		{
			csnzprintf(string, maximum_string_length, "%hd.%hd.%hd.%hd",
				address->ina.bytes[3],
				address->ina.bytes[2],
				address->ina.bytes[1],
				address->ina.bytes[0]);

			if (include_port)
			{
				csnzappendf(string, maximum_string_length, ":%hd", address->port);
			}

			if (include_extra)
			{
				if (secure_address)
				{
					transport_secure_address_to_string(secure_address, static_string, NUMBEROF(static_string), false, false);
					csstrnzcat(string, static_string, maximum_string_length);
				}
				else if (transport_secure_identifier_retrieve(address, _transport_platform_windows, &secure_identifier_from_address, &secure_address_from_address))
				{
					transport_secure_address_to_string(&secure_address_from_address, static_string, NUMBEROF(static_string), false, false);
					csstrnzcat(string, static_string, maximum_string_length);
					csnzappendf(string, maximum_string_length, "[%s]", transport_secure_identifier_get_string(&secure_identifier_from_address));
				}
				else
				{
					csnzappendf(string, maximum_string_length, " (insecure)");
				}
			}
		}
		break;
		case IPV6_ADDRESS_LENGTH:
		{
			csnzprintf(string, maximum_string_length, "%04X.%04X.%04X.%04X.%04X.%04X.%04X.%04X",
				address->ipv6_address[0],
				address->ipv6_address[1],
				address->ipv6_address[2],
				address->ipv6_address[3],
				address->ipv6_address[4],
				address->ipv6_address[5],
				address->ipv6_address[6],
				address->ipv6_address[7]);

			if (include_port)
			{
				csnzappendf(string, maximum_string_length, ":%hd", address->port);
			}
		}
		break;
		case -1i16:
		{
			csstrnzcpy(string, address->file_socket_name, maximum_string_length);
		}
		break;
		case 0:
		{
			csstrnzcpy(string, "NULL_ADDRESS", maximum_string_length);
		}
		break;
	}

	return string;
}

bool __fastcall transport_address_equivalent(transport_address const* a, transport_address const* b)
{
	ASSERT(a != NULL);
	ASSERT(b != NULL);
	return INVOKE(0x77D0, transport_address_equivalent, a, b);
}

bool transport_address_valid(transport_address const* address)
{
	bool valid = false;
	if (address)
	{
		switch (address->address_length)
		{
			case -1:
				valid = address->ipv4_address != 0;
				break;
			case 4:
				valid = address->ipv4_address != 0;
				if (!address->ipv4_address)
				{
					event(_event_warning, "networking:transport:transport_address_valid: the IPV4 address is NOT valid\n");
				}
				break;
			case 16:
				for (long i = 0; i < NUMBEROF(address->ina6.words); ++i)
				{
					if (address->ina6.words[i])
					{
						valid = true;
						break;
					}
				}
				if (!valid)
				{
					event(_event_warning, "networking:transport:transport_address_valid: the IPV6 address is NOT valid\n");
				}
				break;
		}
	}
	return valid;
}