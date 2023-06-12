#include "network_link.h"

c_network_channel* c_network_link::get_associated_channel(s_transport_address const* address)
{
	typedef c_network_channel*(__fastcall* get_associated_channel_ptr)(s_transport_address const* address);
	static auto get_associated_channel = reinterpret_cast<get_associated_channel_ptr>(module_base + 0x6590);
	return get_associated_channel(address);
}