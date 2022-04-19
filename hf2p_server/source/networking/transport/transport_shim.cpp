#include "transport_shim.h"
#include "../../dllmain.h"

// had to rewrite this function due to call issues (original: module_base + 0x2B4A70)
void xnet_shim_table_add(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier)
{
	long row_index = xnet_shim_table_get_row(address, secure_address, false);
	if (row_index != -1)
	{
		g_xnet_shim_table->rows[row_index].valid = true;
		g_xnet_shim_table->rows[row_index].inaddr = *address;
		g_xnet_shim_table->rows[row_index].xnaddr = *secure_address;
		g_xnet_shim_table->rows[row_index].xnkid = *secure_identifier;
	}
}

// had to rewrite this function due to call issues (original: module_base + 0x2B4920)
// returns the index of the row with either a matching address, secure address, or the first invalid/available row if ignore_invalid_rows is false and no address matches
long xnet_shim_table_get_row(const s_transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid_rows)
{
	long row_index = -1;
	for (long i = 0; i < sizeof(s_xnet_shim_table) / sizeof(s_xnet_shim_table_row); i++)
	{
		if (ignore_invalid_rows && !g_xnet_shim_table->rows[i].valid) // skip invalid rows if we're ignoring them
			continue;
		if (address && transport_address_equivalent(&g_xnet_shim_table->rows[i].inaddr, address))
			return i;
		if (secure_address)
		{
			bool secure_addr_match = true;
			for (long j = 0; j < sizeof(s_transport_secure_address); j++)
			{
				if (g_xnet_shim_table->rows[i].xnaddr.data[j] != secure_address->data[j])
				{
					secure_addr_match = false;
					break;
				}
			}
			if (secure_addr_match)
				return i;
		}
		if (!ignore_invalid_rows && !g_xnet_shim_table->rows[i].valid) // get first invalid/available row
		{
			{
				long i2 = i;
				if (row_index != -1)
					i2 = row_index;
				row_index = i2;
			}
		}
	}
	return row_index;
}