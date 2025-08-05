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
	static c_network_channel* __fastcall get_associated_channel(transport_address const* address);

	struct s_link_packet
	{
		c_enum<e_network_packet_mode, long, _network_packet_mode_none, k_network_packet_mode_count> packet_mode;
		bool simulate_packet;
		transport_address address;
		long game_data_length;
		byte game_data[k_network_link_maximum_game_data_size];
		long voice_data_length;
		byte voice_data[k_network_link_maximum_voice_data_size];
	};
	static_assert(sizeof(s_link_packet) == 0x6D4);

private:
	bool m_initialized;
	ulong m_next_channel_identifier;
	long m_next_first_channel_index;
	transport_endpoint* m_endpoint;
	c_network_out_of_band_consumer* m_out_of_band_consumer;
	c_network_time_statistics m_packets_transmitted;
	c_network_time_statistics m_packets_received;
	c_network_time_statistics m_upstream_bandwidth;
	c_network_time_statistics m_downstream_bandwidth;
};
static_assert(sizeof(c_network_link) == 0x378);