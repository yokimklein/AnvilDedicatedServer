#pragma once
#include "..\..\cseries\cseries.h"
#include "network_message_type_collection.h"

class c_network_out_of_band_consumer
{
public:
	virtual bool __cdecl receive_out_of_band_packet(s_transport_address const* address, c_bitstream* packet);
};
static_assert(sizeof(c_network_out_of_band_consumer) == 0x4);

struct s_network_message_ping : s_network_message
{
	ulong id;
	ulong timestamp;
	ulong request_qos;
};

struct s_network_message_pong : s_network_message
{
	ulong id;
	ulong timestamp;
	ulong request_qos;
};
static_assert(sizeof(s_network_message_pong) == 0xC);