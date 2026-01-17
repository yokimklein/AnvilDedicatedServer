#include "transport_shim.h"
#include <anvil\backend\lobby.h>
#include <game\game.h>
#include <combaseapi.h>

REFERENCE_DECLARE(0x49C0260, s_xnet_shim_table, g_xnet_shim_table);

// returns the index of the row with a matching transport address or secure address
// returns NONE if no rows match & ignore_invalid_rows is true, otherwise it returns the first invalid/unused row
long XNetFindEntry(const transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows)
{
	long first_invalid_row = NONE;
	for (long i = 0; i < k_xnet_shim_table_rows; i++)
	{
		// skip invalid rows if we're ignoring them
		if (ignore_invalid_rows && !g_xnet_shim_table.rows[i].valid)
		{
			continue;
		}
		// if address matches
		if (address && transport_address_equivalent(&g_xnet_shim_table.rows[i].inaddr, address))
		{
			return i;
		}
		// if secure address matches
		if (secure_address && g_xnet_shim_table.rows[i].xnaddr == *secure_address)
		{
			return i;
		}
		// get first invalid/available to use row if we're not ignoring them
		if (!ignore_invalid_rows && !g_xnet_shim_table.rows[i].valid)
		{
			if (first_invalid_row == NONE)
			{
				first_invalid_row = i;
			}
		}
	}
	return first_invalid_row;
}

void XNetAddEntry(const transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier)
{
	long row_index = XNetFindEntry(address, secure_address, false);
	if (row_index != NONE)
	{
		g_xnet_shim_table.rows[row_index].valid = true;
		g_xnet_shim_table.rows[row_index].inaddr = *address;
		g_xnet_shim_table.rows[row_index].xnaddr = *secure_address;
		g_xnet_shim_table.rows[row_index].xnkid = *secure_identifier;
	}
}

void XNetUnregisterKey(const transport_address* address)
{
	long row_index = XNetFindEntry(address, nullptr, true);
	if (row_index != NONE)
	{
		g_xnet_shim_table.rows[row_index].valid = false;
	}
}

bool XNetInAddrToXnAddr(const transport_address* address, s_transport_secure_address* secure_address)
{
	long row_index = XNetFindEntry(address, nullptr, true);
	if (row_index == NONE)
	{
		return false;
	}
	*secure_address = g_xnet_shim_table.rows[row_index].xnaddr;
	return true;
}

// XNetInAddrToXnAddr variant which also returns a secure_identifier
bool XNetInAddrToXnAddr(const transport_address* address, s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier)
{
	long row_index = XNetFindEntry(address, nullptr, true);
	if (row_index == NONE)
	{
		return false;
	}
	*secure_address = g_xnet_shim_table.rows[row_index].xnaddr;
	*secure_identifier = g_xnet_shim_table.rows[row_index].xnkid;
	return true;
}

bool XNetXnAddrToInAddr(const s_transport_secure_address* secure_address, transport_address* address)
{
	long row_index = XNetFindEntry(nullptr, secure_address, true);
	if (row_index == NONE)
	{
		return false;
	}
	*address = g_xnet_shim_table.rows[row_index].inaddr;
	return true;
}

void XNetCreateKey(s_transport_secure_identifier* out_secure_identifier)
{
	if (game_is_dedicated_server())
	{
		anvil_get_lobby_identifier(out_secure_identifier);
	}
	else
	{
		GUID session_id;
		CoCreateGuid(&session_id);
		csmemcpy(out_secure_identifier, &session_id, sizeof(session_id));
	}
}