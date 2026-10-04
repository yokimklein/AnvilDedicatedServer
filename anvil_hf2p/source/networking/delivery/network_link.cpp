#include "network_link.h"

c_network_channel* __fastcall c_network_link::get_associated_channel(transport_address const* address)
{
	return DECLFUNC(ADDRESS_C_NETWORK_LINK_GET_ASSOCIATED_CHANNEL, c_network_channel*, __fastcall, transport_address const*)(address);
}

void c_network_link::send_out_of_band(const c_bitstream* game_data, const transport_address* address, long* out_size_on_wire)
{
	ASSERT(game_data);
	ASSERT(address);

	INVOKE_CLASS_MEMBER(ADDRESS_C_NETWORK_LINK_SEND_OUT_OF_BAND, c_network_link, send_out_of_band, game_data, address, out_size_on_wire);
}