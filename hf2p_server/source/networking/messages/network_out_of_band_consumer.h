#pragma once

struct s_transport_address;
class c_bitstream;
class c_network_out_of_band_consumer
{
public:
	virtual bool __cdecl receive_out_of_band_packet(s_transport_address const* incoming_address, c_bitstream* packet) = 0;

	c_network_out_of_band_consumer();
};
static_assert(sizeof(c_network_out_of_band_consumer) == 0x4);