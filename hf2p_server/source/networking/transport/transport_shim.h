#pragma once
#include "transport_address.h"
#include "..\..\dllmain.h"

struct s_xnet_shim_table_row
{
	char valid;
	byte usable_address[0x10];
	byte xnaddr[0x10];
	byte xnkid[0x10];
};

static s_xnet_shim_table_row* g_xnet_shim_table = (s_xnet_shim_table_row*)(module_base + 0x49C0260);
void xnet_shim_table_add(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);