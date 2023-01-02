#include "transport_shim.h"
#include "..\..\dllmain.h"

// returns the index of the row with a matching transport address or secure address
// returns -1 if no rows match & ignore_invalid_rows is true, otherwise it returns the first invalid/unused row
long xnet_shim_table_find_row(const s_transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows)
{
	long first_invalid_row = -1;
	for (long i = 0; i < k_xnet_shim_table_rows; i++)
	{
		// skip invalid rows if we're ignoring them
		if (ignore_invalid_rows && !g_xnet_shim_table->rows[i].valid)
			continue;
		// if address matches
		if (address && transport_address_equivalent(&g_xnet_shim_table->rows[i].inaddr, address))
			return i;
		// if secure address matches
		if (secure_address && memcmp(&g_xnet_shim_table->rows[i].xnaddr, secure_address, sizeof(s_transport_secure_address)) == 0)
			return i;
		// get first invalid/available to use row if we're not ignoring them
		if (!ignore_invalid_rows && !g_xnet_shim_table->rows[i].valid)
		{
			if (first_invalid_row == -1)
				first_invalid_row = i;
		}
	}
	return first_invalid_row;
}

// XNetRegisterKey
void xnet_shim_register(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier)
{
	long row_index = xnet_shim_table_find_row(address, secure_address, false);
	if (row_index != -1)
	{
		g_xnet_shim_table->rows[row_index].valid = true;
		g_xnet_shim_table->rows[row_index].inaddr = *address;
		g_xnet_shim_table->rows[row_index].xnaddr = *secure_address;
		g_xnet_shim_table->rows[row_index].xnkid = *secure_identifier;
	}
}

// XNetUnregisterKey
void xnet_shim_unregister(const s_transport_address* address)
{
	long row_index = xnet_shim_table_find_row(address, nullptr, true);
	if (row_index != -1)
		g_xnet_shim_table->rows[row_index].valid = false;
}

// XNetInAddrToXnAddr
bool xnet_shim_inaddr_to_xnaddr(const s_transport_address* address, s_transport_secure_address* secure_address)
{
	long row_index = xnet_shim_table_find_row(address, nullptr, true);
	if (row_index == -1)
		return false;
	memcpy(secure_address, &g_xnet_shim_table->rows[row_index].xnaddr, sizeof(s_transport_secure_address));
	return true;
}

// XNetInAddrToXnAddr variant which also returns a secure_identifier
bool xnet_shim_inaddr_to_xnaddr2(const s_transport_address* address, s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier)
{
	long row_index = xnet_shim_table_find_row(address, nullptr, true);
	if (row_index == -1)
		return false;
	memcpy(secure_address, &g_xnet_shim_table->rows[row_index].xnaddr, sizeof(s_transport_secure_address));
	memcpy(secure_identifier, &g_xnet_shim_table->rows[row_index].xnkid, sizeof(s_transport_secure_identifier));
	return true;
}

// XNetXnAddrToInAddr
bool xnet_shim_xnaddr_to_inaddr(const s_transport_secure_address* secure_address, s_transport_address* address)
{
	long row_index = xnet_shim_table_find_row(nullptr, secure_address, true);
	if (row_index == -1)
		return false;
	memcpy(address, &g_xnet_shim_table->rows[row_index].inaddr, sizeof(s_transport_address));
	return true;
}