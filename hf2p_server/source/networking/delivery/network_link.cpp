#include "network_link.h"
#include "..\transport\transport_address.h"
#include "..\delivery\network_channel.h"

c_network_channel* c_network_link::get_associated_channel(s_transport_address const* address)
{
	typedef c_network_channel*(__thiscall* c_network_link__get_associated_channel_ptr)(c_network_link* link, s_transport_address const* address);
	static auto c_network_link__get_associated_channel = reinterpret_cast<c_network_link__get_associated_channel_ptr>(module_base + 0x6590);
	return c_network_link__get_associated_channel(this, address);
}