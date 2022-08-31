#pragma once
#include "..\transport\transport_address.h"
#include "..\..\memory\bitstream.h"
#include "..\network_statistics.h"
#include "..\transport\transport_endpoint_winsock.h"

class c_network_channel;
class c_network_message_gateway;
class c_network_link
{
public:
	c_network_channel* get_associated_channel(s_transport_address const* address);

	struct s_link_packet
	{
		int packet_mode;
		byte __unknown4;
		byte __unknown5;
		uint16_t flags;
		s_transport_address address;
		int game_data_length;
		char game_data[1200];
		int voice_data_length;
		char voice_data[512];
	};

	bool m_initialized;
	byte __data1[3];
	int32_t m_channel_count;
	bool m_endpoints_created;
	s_transport_endpoint* m_endpoints[1];
	c_network_message_gateway* m_message_gateway;
	c_network_time_statistics m_time_statistics[4];
	byte __data[4];
};
static_assert(sizeof(c_network_link) == 0x378);