#pragma once
#include <cstdint>
#include "..\..\dllmain.h"
#include "transport_address.h"

struct s_xnet_shim_table_row
{
	bool valid;
	s_transport_address inaddr; // insecure_address
	s_transport_secure_address xnaddr; // xnet_address
	s_transport_secure_identifier xnkid; // xnet_identifier
};
static_assert(sizeof(s_xnet_shim_table_row) == 0x38);

struct s_xnet_shim_table
{
	s_xnet_shim_table_row rows[51]; // k_network_maximum_machines_per_session * 3? 3 rows per peer?
};
static_assert(sizeof(s_xnet_shim_table) == 0xB28);

static s_xnet_shim_table* g_xnet_shim_table = (s_xnet_shim_table*)(module_base + 0x49C0260);
void xnet_shim_table_add(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);
long xnet_shim_table_get_row(const s_transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows);