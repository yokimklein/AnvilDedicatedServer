#include "transport_dns_winsock.h"
#include "networking\transport\transport.h"
#include "memory\byte_swapping.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <cseries\cseries_events.h>

// Returns the transport addresses of a max of 8 network adapters from a given hostname
// $NOTE: This function is modified to filter out undesirable adapters
bool __fastcall transport_dns_name_to_address(dns_result* result)
{
	//return INVOKE(0x6DE0, transport_dns_name_to_address, result);

	bool success = false;
	if (transport_available())
	{
		ASSERT(result != NULL);

		const ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
		const ULONG family = AF_UNSPEC; // Search for both IPV4 and IPV6 associated adapters
		ULONG buffer_size = 0;
		if (ULONG err = GetAdaptersAddresses(family, flags, NULL, NULL, &buffer_size); err != ERROR_BUFFER_OVERFLOW) // Get buffer size for adapters
		{
			event(_event_error, "transport: GetAdaptersAddresses failed to retrieve adapters buffer size with return code %d", err);
			return false;
		}

		IP_ADAPTER_ADDRESSES* adapters = (IP_ADAPTER_ADDRESSES*)malloc(buffer_size);
		if (!adapters)
		{
			event(_event_error, "transport: failed to allocate memory for adapters buffer!");
			return false;
		}

		if (ULONG err = GetAdaptersAddresses(family, flags, NULL, adapters, &buffer_size); err != NO_ERROR)
		{
			event(_event_error, "transport: GetAdaptersAddresses failed to retrieve adapters with return code %d", err);
			return false;
		}

		ulong adapter_index = 0;
		for (IP_ADAPTER_ADDRESSES* adapter = adapters; adapter; adapter = adapter->Next)
		{
			if (adapter->OperStatus != IfOperStatusUp)
			{
				continue;
			}

			// If neither IPV4 or IPV6 are enabled, ignore this adapter
			if (!adapter->Ipv4Enabled && !adapter->Ipv6Enabled)
			{
				continue;
			}

			if (adapter->PhysicalAddressLength == 0)
			{
				continue;
			}

			// Skip adapters w/o a gateway, this should filter us to LAN adapters
			// This is unreliable?
			if (!adapter->FirstGatewayAddress)
			{
				continue;
			}

			for (IP_ADAPTER_UNICAST_ADDRESS_LH* ua = adapter->FirstUnicastAddress; ua; ua = ua->Next)
			{
				LPSOCKADDR sockaddr = ua->Address.lpSockaddr;

				// skip non IPV4 && IPV6 addresses
				if (sockaddr->sa_family != AF_INET && sockaddr->sa_family != AF_INET6)
				{
					continue;
				}

				if (sockaddr->sa_family == AF_INET6)
				{
					auto* sin6 = reinterpret_cast<sockaddr_in6*>(sockaddr);

					// Filter link-local IPV6 (0000::/00)
					if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr))
					{
						continue;
					}

					// Filter temporary IPV6 addresses
					if (ua->SuffixOrigin == IpSuffixOriginRandom)
					{
						continue;
					}

					const ushort* ip = reinterpret_cast<const ushort*>(&sin6->sin6_addr.u.Word);
					result->address[adapter_index].ipv6_address[0] = bswap_uns16(ip[0]);
					result->address[adapter_index].ipv6_address[1] = bswap_uns16(ip[1]);
					result->address[adapter_index].ipv6_address[2] = bswap_uns16(ip[2]);
					result->address[adapter_index].ipv6_address[3] = bswap_uns16(ip[3]);
					result->address[adapter_index].ipv6_address[4] = bswap_uns16(ip[4]);
					result->address[adapter_index].ipv6_address[5] = bswap_uns16(ip[5]);
					result->address[adapter_index].ipv6_address[6] = bswap_uns16(ip[6]);
					result->address[adapter_index].ipv6_address[7] = bswap_uns16(ip[7]);
					result->address[adapter_index].address_length = IPV6_ADDRESS_LENGTH;
				}
				else
				{
					const sockaddr_in* sin = reinterpret_cast<const sockaddr_in*>(sockaddr);
					result->address[adapter_index].ipv4_address = bswap_uns32(sin->sin_addr.s_addr);
					result->address[adapter_index].address_length = IPV4_ADDRESS_LENGTH;
				}

				if (transport_address_valid(&result->address[adapter_index]))
				{
					result->adapter_names[adapter_index].set(adapter->FriendlyName);
					success = true;
					adapter_index++;
				}
			}
		}
		free(adapters);
	}
	return success;
}