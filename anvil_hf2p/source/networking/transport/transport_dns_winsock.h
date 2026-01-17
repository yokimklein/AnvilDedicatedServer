#pragma once
#include "networking\transport\transport_address.h"
#include "text\unicode.h"

struct dns_result
{
	c_static_wchar_string<256> adapter_names[8];
	transport_address address[8];
	//char name[256];
};
//static_assert(sizeof(dns_result) == 0x1A0);

extern bool __fastcall transport_dns_name_to_address(dns_result* result);