#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_address.h>
#include <networking\network_statistics.h>
#include <networking\transport\transport_endpoint_winsock.h>

long const k_network_link_maximum_game_data_size = 0x4B0;
long const k_network_link_maximum_voice_data_size = 0x200;

enum e_network_packet_mode
{
	_network_packet_mode_none = -1,
	_network_packet_mode_channel,
	_network_packet_mode_out_of_band,
	_network_packet_mode_voice,

	k_network_packet_mode_count
};

class c_network_channel;
class c_network_out_of_band_consumer;
class c_network_link
{
public:
	c_network_channel* __fastcall get_associated_channel(s_transport_address const* address);

	struct s_link_packet
	{
		c_enum<e_network_packet_mode, long, k_network_packet_mode_count> mode;
		bool __unknown4;
		byte __pad5[0x3];
		s_transport_address address;
		long game_data_length;
		byte game_data[k_network_link_maximum_game_data_size];
		long voice_data_length;
		byte voice_data[k_network_link_maximum_voice_data_size];
	};

	bool m_initialized;
	long __unknown4;
	long __unknown8;
	s_transport_endpoint* m_endpoint;
	c_network_out_of_band_consumer* m_out_of_band;
	long __unknown14;
	c_network_time_statistics m_time_statistics[4];
};
static_assert(sizeof(c_network_link) == 0x378);