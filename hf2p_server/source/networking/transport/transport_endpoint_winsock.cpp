#include "transport_endpoint_winsock.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "IPHLPAPI.lib")

// TODO: nuke this shit 
/*
long netadapter_test()
{
    /*
    DWORD return_code = 0;
    DWORD buf_size = sizeof(IP_ADAPTER_INFO);;
    PIP_ADAPTER_INFO adapter_info = (IP_ADAPTER_INFO*)malloc(buf_size);
    if (adapter_info == NULL)
    {
        printf("failed to allocate memory for adapter_info\n");
        return 1;
    }

    // Initial call to get the buffer size
    if ((return_code = GetAdaptersInfo(adapter_info, &buf_size)) == ERROR_BUFFER_OVERFLOW)
    {
        free(adapter_info);
        adapter_info = (IP_ADAPTER_INFO*)malloc(buf_size);
        if (adapter_info == NULL) {
            printf("error allocating memory needed to call GetAdaptersinfo\n");
            return 1;
        }
    }
    if ((return_code = GetAdaptersInfo(adapter_info, &buf_size)) == NO_ERROR)
    {
        PIP_ADAPTER_INFO current_adapter = adapter_info;
        while (current_adapter)
        {
            if (current_adapter->GatewayList.IpAddress.String[0] != '\0' && // make sure there's actually a value
                strcmp(current_adapter->GatewayList.IpAddress.String, "0.0.0.0") != 0) // make sure the value is not '0.0.0.0'
            {
                printf("Adapter Name: %s\n", current_adapter->AdapterName);
                printf("IP Address: %s\n", current_adapter->IpAddressList.IpAddress.String);
                printf("Gateway: %s\n", current_adapter->GatewayList.IpAddress.String);
                break; // Found the adapter with a default gateway
            }
            current_adapter = current_adapter->Next;
        }
    }
    else
    {
        printf("GetAdaptersInfo failed with error: %u\n", return_code);
    }

    if (adapter_info)
    {
        free(adapter_info);
    }

    */
    /*
    ULONG outBufLen = 15000;
    ULONG Iterations = 0;
    DWORD dwRetVal = 0;
    do {

        PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
        if (pAddresses == NULL) {
            printf
            ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            free(pAddresses);
            pAddresses = NULL;
        }
        else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = NULL;
    IP_ADAPTER_PREFIX* pPrefix = NULL;
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    LPVOID lpMsgBuf = NULL;
    unsigned int i = 0;

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
                pCurrAddresses->Length);
            printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
            printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) {
                for (i = 0; pUnicast != NULL; i++)
                    pUnicast = pUnicast->Next;
                printf("\tNumber of Unicast Addresses: %d\n", i);
            }
            else
                printf("\tNo Unicast Addresses\n");

            pAnycast = pCurrAddresses->FirstAnycastAddress;
            if (pAnycast) {
                for (i = 0; pAnycast != NULL; i++)
                    pAnycast = pAnycast->Next;
                printf("\tNumber of Anycast Addresses: %d\n", i);
            }
            else
                printf("\tNo Anycast Addresses\n");

            pMulticast = pCurrAddresses->FirstMulticastAddress;
            if (pMulticast) {
                for (i = 0; pMulticast != NULL; i++)
                    pMulticast = pMulticast->Next;
                printf("\tNumber of Multicast Addresses: %d\n", i);
            }
            else
                printf("\tNo Multicast Addresses\n");

            pDnServer = pCurrAddresses->FirstDnsServerAddress;
            if (pDnServer) {
                for (i = 0; pDnServer != NULL; i++)
                    pDnServer = pDnServer->Next;
                printf("\tNumber of DNS Server Addresses: %d\n", i);
            }
            else
                printf("\tNo DNS Server Addresses\n");

            printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
            printf("\tDescription: %wS\n", pCurrAddresses->Description);
            printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

            if (pCurrAddresses->PhysicalAddressLength != 0) {
                printf("\tPhysical address: ");
                for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
                    i++) {
                    if (i == (pCurrAddresses->PhysicalAddressLength - 1))
                        printf("%.2X\n",
                            (int)pCurrAddresses->PhysicalAddress[i]);
                    else
                        printf("%.2X-",
                            (int)pCurrAddresses->PhysicalAddress[i]);
                }
            }
            printf("\tFlags: %ld\n", pCurrAddresses->Flags);
            printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
            printf("\tIfType: %ld\n", pCurrAddresses->IfType);
            printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
            printf("\tIpv6IfIndex (IPv6 interface): %u\n",
                pCurrAddresses->Ipv6IfIndex);
            printf("\tZoneIndices (hex): ");
            for (i = 0; i < 16; i++)
                printf("%lx ", pCurrAddresses->ZoneIndices[i]);
            printf("\n");

            printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
            printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

            pPrefix = pCurrAddresses->FirstPrefix;
            if (pPrefix) {
                for (i = 0; pPrefix != NULL; i++)
                    pPrefix = pPrefix->Next;
                printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
            }
            else
                printf("\tNumber of IP Adapter Prefix entries: 0\n");

            printf("\n");

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else {
        printf("Call to GetAdaptersAddresses failed with error: %d\n",
            dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            printf("\tNo addresses were found for the requested parameters\n");
        else {

            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                // Default language
                (LPTSTR)&lpMsgBuf, 0, NULL)) {
                printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    free(pAddresses);
                exit(1);
            }
        }
    }

    if (pAddresses) {
        free(pAddresses);
    }

    return 0;
}
*/