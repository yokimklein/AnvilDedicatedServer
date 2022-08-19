#pragma once
#include <windows.h>
#include <stdint.h>
#include "..\..\memory\sliding_window.h"

struct c_network_channel_client
{
	struct c_network_channel_client_vtbl* __vftable /*VFT*/;
	byte m_allocated;
	byte m_read_simulation_data_expected;
	byte __unknown6;
	byte m_channel_has_been_used;
};
static_assert(sizeof(struct c_network_channel_client) == 0x8);

class c_network_connection : c_network_channel_client
{
	struct s_connection_incoming_packet
	{
		uint16_t flags;
	};
	static_assert(sizeof(struct s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		long timestamp;
		long size_on_wire;
		long round_trip_msec;
		int16_t unretired_window_size;
		uint16_t flags;
	};
	static_assert(sizeof(struct s_connection_outgoing_packet) == 0x10);

	const char* n_name;
	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;
	long m_incoming_window_size_bytes;
	long m_outgoing_window_size_bytes;
	s_connection_incoming_packet m_incoming_packets[0x80];
	s_connection_outgoing_packet m_outgoing_packets[0x80];
	byte __data94C[0x10];
	long m_packet_timeout;
};
static_assert(sizeof(c_network_connection) == 0x960);