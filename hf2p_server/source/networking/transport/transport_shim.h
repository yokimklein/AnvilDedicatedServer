#pragma once
#include "..\..\dllmain.h"
#include "transport_address.h"
#include "..\network_globals.h"
#include "..\..\simulation\simulation.h"

// 51 rows, it being sessions * peers is assumed and not confirmed
constexpr long k_xnet_shim_table_rows = k_network_maximum_machines_per_session * k_network_maximum_sessions;

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
	s_xnet_shim_table_row rows[k_xnet_shim_table_rows];
};
static_assert(sizeof(s_xnet_shim_table) == 0xB28);

static s_xnet_shim_table* g_xnet_shim_table = (s_xnet_shim_table*)(module_base + 0x49C0260);

long xnet_shim_table_find_row(const s_transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows);
void xnet_shim_register(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);
void xnet_shim_unregister(const s_transport_address* address);
bool xnet_shim_inaddr_to_xnaddr(const s_transport_address* address, s_transport_secure_address* secure_address);
bool xnet_shim_inaddr_to_xnaddr2(const s_transport_address* address, s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier);
bool xnet_shim_xnaddr_to_inaddr(const s_transport_secure_address* secure_address, s_transport_address* address);