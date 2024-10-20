#pragma once
#include <networking\transport\transport_address.h>
#include <networking\transport\transport_security.h>
#include <networking\network_globals.h>
#include <simulation\simulation.h>

// 51 rows, it being sessions * peers is assumed and not confirmed
constexpr long k_xnet_shim_table_rows = k_network_maximum_machines_per_session * k_network_maximum_sessions;

struct s_xnet_shim_table_row
{
	bool valid;
	byte : 8;
	byte : 8;
	byte : 8;
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

extern s_xnet_shim_table& g_xnet_shim_table;

struct s_xnet_address
{
	s_transport_address addresses[8];
	char hostname[0x100];
};

long XNetFindEntry(const s_transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows);
void XNetAddEntry(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);
void XNetUnregisterKey(const s_transport_address* address);
bool XNetInAddrToXnAddr(const s_transport_address* address, s_transport_secure_address* secure_address);
bool XNetInAddrToXnAddr(const s_transport_address* address, s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier);
bool XNetXnAddrToInAddr(const s_transport_secure_address* secure_address, s_transport_address* address);
bool __fastcall XNetGetTitleXnAddr(s_xnet_address* out_addresses);
void XNetCreateKey(s_transport_secure_identifier* out_secure_identifier);