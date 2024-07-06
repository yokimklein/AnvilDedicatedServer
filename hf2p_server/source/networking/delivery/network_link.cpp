#include "network_link.h"

c_network_channel* __fastcall c_network_link::get_associated_channel(s_transport_address const* address)
{
	return DECLFUNC(0x6590, c_network_channel*, __fastcall, s_transport_address const*)(address);
}