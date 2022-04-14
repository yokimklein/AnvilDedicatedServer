#include "transport_shim.h"
#include "../../dllmain.h"

void xnet_shim_table_add(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier)
{
	typedef void(__cdecl* xnet_shim_table_add_ptr)(const s_transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);
	auto xnet_shim_table_add = reinterpret_cast<xnet_shim_table_add_ptr>(module_base + 0x2B4A70);
	xnet_shim_table_add(address, secure_address, secure_identifier);
}